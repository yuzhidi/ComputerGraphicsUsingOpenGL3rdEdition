from math import *

def toRad(deg):
    return deg*pi/180.0

class Vector:

    def __init__(self, x, y, z=0.0):
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):
        return "Vector: <%f %f %f>" %(self.x, self.y, self.z)

    def getNormal(self):
        return Vector(-self.y, self.x, self.z)

    def normalize(self):
        self/self.mag()

    def mag(self):
        return sqrt(self.x*self.x + self.y*self.y + self.z*self.z)

    def __div__(self, scale):
        self.x /= float(scale)
        self.y /= float(scale)
        self.z /= float(scale)

    def __mul__(self, scale):
        self.x *= float(scale)
        self.y *= float(scale)
        self.z *= float(scale)

    def __add__(self, v):
        return Vector(self.x+v.x, self.y+v.y, self.z+v.z)

class Point:

    def __init__(self, x, y, z=0.0):
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):
        return "Point: <%f %f %f>" %(self.x, self.y, self.z)

    def __sub__(self, p):
        return Vector(self.x-p.x, self.y-p.y, self.z-p.z)

    def __mul__(self, scale):
        self.x *= float(scale)
        self.y *= float(scale)
        self.z *= float(scale)

    def __add__(self, p):
        return Point(self.x+p.x, self.y+p.y, self.z+p.z)

    def lerp(self, p, t):
        return Point(self.x+(p.x-self.x)*t, self.y+(p.y-self.y)*t, self.z+(p.z-self.z)*t)

    def translate(self, delX, delY):
        self.x += delX
        self.y += delY

class Face:

    def __init__(self):
        self.vertexIdList = []
        self.normalIdList = []

    def write(self, f):
        f.write(str(len(self.vertexIdList))+" ")
        for vid in self.vertexIdList:
            f.write(str(vid)+" ")
        for nid in self.normalIdList:
            f.write(str(nid)+" ")
        f.write("\n")
        
class Mesh:

    def __init__(self):
        self.vertexList = []
        self.normalList = []
        self.faceList = []

    def write(self, fileName):
        f = open(fileName, "w")
        f.write("%d %d %d\n" %(len(self.vertexList), len(self.normalList), len(self.faceList)))
        for vertex in self.vertexList:
            f.write("%f %f %f\n" %(vertex.x, vertex.y, vertex.z))
        for normal in self.normalList:
            f.write("%f %f %f\n" %(normal.x, normal.y, normal.z))
        for face in self.faceList:
            face.write(f)
        f.close()

class Line:

    def __init__(self, p0, p1, num):
        self.p0 = p0
        self.p1 = p1
        self.num = num

    def translate(self, delX, delY):
        self.p0.translate(delX, delY)
        self.p1.translate(delX, delY)
        
    def getPointsAndNormals(self):
        normal = (self.p1-self.p0).getNormal()
        normal.normalize()
        normalList = self.num*[normal]
        t = 0
        del_t = 1.0/(self.num-1)
        vertexList = []
        for i in range(self.num):
            vertexList.append(self.p0.lerp(self.p1, t))
            t += del_t
        return (vertexList, normalList)

class BezierCurve:

    def __init__(self, p0, p1, p2, p3, num):
        self.p0 = p0
        self.p1 = p1
        self.p2 = p2
        self.p3 = p3
        self.num = num

    def translate(self, delX, delY):
        self.p0.translate(delX, delY)
        self.p1.translate(delX, delY)
        self.p2.translate(delX, delY)
        self.p3.translate(delX, delY)

    def evalBezier(self, p0, p1, p2, p3, t):
        return p0*pow(1-t, 3) + \
               3.0*p1*pow(1-t, 2)*t + \
               3.0*p2*(1-t)*t*t + p3*pow(t,3)
    
    def getPointsAndNormals(self):
        t = 0.0
        del_t = 1.0/(self.num-1)
        vertexList = []
        normalList = []
        for i in range(self.num):
            x = self.evalBezier(self.p0.x, self.p1.x, self.p2.x, self.p3.x, t)
            y = self.evalBezier(self.p0.y, self.p1.y, self.p2.y, self.p3.y, t)
            vertexList.append(Point(x, y))
            t += del_t
        normal = (vertexList[1]-vertexList[0]).getNormal()
        normal.normalize()
        normalList.append(normal)
        for i in range(1, self.num-1):
            n1 = (vertexList[i]-vertexList[i-1]).getNormal()
            n2 = (vertexList[i+1]-vertexList[i]).getNormal()
            n1.normalize()
            n2.normalize()
            normal = (n1+n2)
            normal/2.0
            normal.normalize()
            normalList.append(normal)
        normal = (vertexList[-1]-vertexList[-2]).getNormal()
        normal.normalize()
        normalList.append(normal)
        return (vertexList, normalList)

class Arc:

    def __init__(self, center, radius, theta1, theta2, num):
        self.theta1 = toRad(theta1)
        self.theta2 = toRad(theta2)
        self.radius = radius
        self.center = center
        self.num = num

    def translate(self, delX, delY):
        self.center.translate(delX, delY)

    def diffTheta(self, theta1, theta2):
        if theta1 < theta2:
            return theta2-theta1
        else:
            return 2*pi-(theta1-theta2)

    def getPointsAndNormals(self):
        del_theta = self.diffTheta(self.theta1, self.theta2)/(self.num-1)
        theta = self.theta1
        vertexList = []
        normalList = []
        for i in range(self.num):
            pt = Point(self.radius*cos(theta)+self.center.x,
                       self.radius*sin(theta)+self.center.y)
            normal = pt-self.center
            normal.normalize()
            normalList.append(normal)
            vertexList.append(pt)
            theta += del_theta
        return (vertexList, normalList)

class Profile:

    def __init__(self, curveList=None):
        if curveList is None:
            self.curveList = []
        else:
            self.curveList = curveList

    def getPointsAndNormals(self):
        vertexList = []
        normalList = []
        for curve in self.curveList:
            curveVertexList, curveNormalList = curve.getPointsAndNormals()
            vertexList.extend(curveVertexList)
            normalList.extend(curveNormalList)
        return (vertexList, normalList)

    def rotY(self, point, theta):
        theta = toRad(theta)
        cls = point.__class__
        return cls(point.x*cos(theta), point.y, point.x*sin(theta))

    def getMeshOfRevolution(self, nSlices):
        vertexList, normalList = self.getPointsAndNormals()
        mesh = Mesh()
        mesh.vertexList.extend(vertexList)
        mesh.normalList.extend(normalList)
        del_theta = 360.0/nSlices
        theta = del_theta
        numVertices = len(vertexList)
        for i in range(1, nSlices):
            for vertex in vertexList:
                mesh.vertexList.append(self.rotY(vertex, theta))
            for j in range(numVertices-1):
                face = Face()
                for l in (face.vertexIdList, face.normalIdList):
                    currNodeId = i*numVertices+j
                    l.append(currNodeId)
                    l.append(currNodeId+1)
                    l.append(currNodeId-numVertices+1)
                    l.append(currNodeId-numVertices)
                mesh.faceList.append(face)
            for normal in normalList:
                mesh.normalList.append(self.rotY(normal, theta))
            theta += del_theta
        for j in range(numVertices-1):
            face = Face()
            for l in (face.vertexIdList, face.normalIdList):
                currNodeId = (nSlices-1)*numVertices+j
                l.append(j)
                l.append(j+1)
                l.append(currNodeId+1)
                l.append(currNodeId)
            mesh.faceList.append(face)
        return mesh

class Pillar:

    def __init__(self, r1, r2, l, nDisks=3, scale=0.2):
        self.topRadius = float(r1)
        self.bottomRadius = float(r2)
        self.length = float(l)
        self.nDisks = nDisks
        self.scale = scale

    def getRadiusAtHeight(self, h):
        return (self.bottomRadius - (self.bottomRadius-self.topRadius)*h/self.length)

    def fillDiskAtHeight(self, profile, h):
        rad1 = self.getRadiusAtHeight(h)
        delta = rad1*self.scale
        rad2 = self.getRadiusAtHeight(h-2*delta)
        profile.curveList.append(Line(Point(rad1, h),
                                      Point(rad1+delta, h), 2))
        profile.curveList.append(Line(Point(rad1+delta, h),
                                      Point(rad1+delta, h-delta), 2))
        profile.curveList.append(Line(Point(rad1+delta, h-delta),
                                      Point(rad2, h-2*delta), 2))
        return rad2

    def getProfile(self):
        profile = Profile()
        profile.curveList.append(Line(Point(0.0, self.length),
                                      Point(self.topRadius, self.length), 5))
        h = self.length
        if self.nDisks:
            del_h = self.length/self.nDisks
        for i in range(self.nDisks):
            rad1 = self.fillDiskAtHeight(profile, h)
            rad2 = self.getRadiusAtHeight(h-del_h)
            profile.curveList.append(Line(Point(rad1, h),
                                          Point(rad2, h-del_h), 5))
            h -= del_h
        return profile

class Arch:

    def __init__(self, curve, height, width, rearWidth=None):
        self.curve = curve
        self.height = height
        self.width = width
        if rearWidth is None:
            self.rearWidth = width
        else:
            self.rearWidth = rearWidth

    def getEdgeVertexList(self, vertexList):
        edgeVertexList = []
        numCurveVertices = len(vertexList)-2
        del_width = 2.0*self.width/(numCurveVertices-1)
        width = self.width
        edgeVertexList.append(Point(self.width, 0.0))
        for i in range(numCurveVertices):
            edgeVertexList.append(Point(width, self.height))
            width -= del_width
        edgeVertexList.append(Point(-self.width, 0.0))
        return edgeVertexList

    def fillSymmetryVerticesAndNormals(self, vertexList, normalList):
        nVertices = len(vertexList)
        for i in range(2, nVertices+1):
            vertex = vertexList[nVertices-i]
            vertexList.append(Point(-vertex.x, vertex.y, vertex.z))
        nNormals = len(normalList)
        for i in range(2, nNormals+1):
            normal = normalList[nNormals-i]
            normalList.append(Vector(-normal.x, normal.y, normal.z))

    def getExtrudedMesh(self, depth):
        vertexList, normalList = self.curve.getPointsAndNormals()
        vertexList.insert(0, Point(vertexList[0].x, 0.0))
        self.fillSymmetryVerticesAndNormals(vertexList, normalList)
        edgeVertexList = self.getEdgeVertexList(vertexList)
        mesh = Mesh()
        mesh.vertexList.extend(vertexList)
        mesh.vertexList.extend(edgeVertexList)
        widthRatio = self.rearWidth/float(self.width)
        for vertex in vertexList:
            mesh.vertexList.append(Point(vertex.x*widthRatio, vertex.y, depth))
        for vertex in edgeVertexList:
            mesh.vertexList.append(Point(vertex.x*widthRatio, vertex.y, depth))
        normalId = 0
        mesh.normalList.extend(normalList)
        frontNormalId = len(normalList)
        rearNormalId = frontNormalId+1
        rightNormalId = frontNormalId+2
        leftNormalId = frontNormalId+3
        topNormalId = frontNormalId+4
        mesh.normalList.append(Vector(0.0, 0.0, 1.0)) #front
        mesh.normalList.append(Vector(0.0, 0.0, -1.0)) #rear
        mesh.normalList.append(Vector(1.0, 0.0, 0.0)) #right
        mesh.normalList.append(Vector(-1.0, 0.0, 0.0)) #left
        mesh.normalList.append(Vector(0.0, 1.0, 0.0)) #top
        N = len(vertexList)
        nSegments = len(vertexList)-1
        for i in range(nSegments):
            rearFace = Face()
            rearFace.vertexIdList = [i, i+N, i+N+1, i+1]
            for j in range(4):
                rearFace.normalIdList.append(rearNormalId)

            frontFace = Face()
            frontFace.vertexIdList = [i+3*N+1, i+3*N, i+2*N, i+2*N+1]
            for j in range(4):
                frontFace.normalIdList.append(frontNormalId)    

            rightFace = Face()
            rightFace.vertexIdList = [i+N+1, i+N, i+3*N, i+3*N+1]
            if (i == 0):
                rightNormal = rightNormalId
            elif (i == nSegments-1):
                rightNormal = leftNormalId
            else:
                rightNormal = topNormalId
            for j in range(4):
                rightFace.normalIdList.append(rightNormal)
                
            leftFace = Face()
            leftFace.vertexIdList = [i+2*N+1, i+2*N, i, i+1]
            if (i == 0):
                for j in range(4):
                    leftFace.normalIdList.append(leftNormalId)
            elif (i == nSegments-1):
                for j in range(4):
                    leftFace.normalIdList.append(rightNormalId)
            else:
                leftFace.normalIdList = [i, i-1, i-1, i]
                
            for face in [frontFace, rearFace, rightFace, leftFace]:
                mesh.faceList.append(face)
        return mesh

def getSpire(base):
    spire = Profile()
    spireHeight = base+9.0
    spireRad = 0.2
    spire.curveList.append(Line(Point(0.0, spireHeight),
                                Point(0.5, spireHeight-1.5), 3))
    spire.curveList.append(Line(Point(0.5, spireHeight-1.5),
                                Point(spireRad, spireHeight-2), 3))
    spire.curveList.append(Arc(Point(spireRad, spireHeight-3), 1.0, 270.0, 90.0, 15))
    spire.curveList.append(Line(Point(spireRad, spireHeight-4),
                                Point(spireRad, spireHeight-4.5), 3))
    spire.curveList.append(Arc(Point(spireRad, spireHeight-5.5), 1.0, 270.0, 90.0, 15))
    spire.curveList.append(Line(Point(spireRad, spireHeight-6.5),
                                Point(spireRad, spireHeight-7), 3))
    spire.curveList.append(Line(Point(spireRad, spireHeight-7),
                                Point(spireRad+1.0, spireHeight-9), 3))
    return spire

def getDome():
    dome = getSpire(25.0)
    p0 = Point(1.2, 25.0)
    p1 = Point(2.7, 23.0)
    p2 = Point(4.2, 20.0)
    p3 = Point(7.2, 19.5)
    dome.curveList.append(BezierCurve(p0, p1, p2, p3, 20))
    dome.curveList.append(Line(Point(7.2, 19.5),
                               Point(6.0, 19.0), 5))
    p0 = Point(6.0, 19.0)
    p1 = Point(8.0, 15.0)
    p2 = Point(14.0, 12.5)
    p3 = Point(12.0, 7.0)
    dome.curveList.append(BezierCurve(p0, p1, p2, p3, 20))
    dome.curveList.append(Line(Point(12.0, 7.0),
                               Point(12.0, 2.0), 10))
    return dome

def getCentralDome():
    dome = getDome()
    dome.curveList.append(Line(Point(12.0, 2.0),
                               Point(12.0, 0.0), 10))
    return dome

def getSmallerDome():
    dome = getDome()
    dome.curveList.append(Line(Point(12.0, 2.0),
                               Point(16.0, 2.0), 3))
    dome.curveList.append(Line(Point(16.0, 2.0),
                               Point(16.0, 0.0), 3))
    dome.curveList.append(Line(Point(16.0, 0.0),
                               Point(0.0, 0.0), 3))
    return dome

def getColumn():
    pillar = Pillar(1.0, 1.0, 75, 10).getProfile()
    spire = getSpire(75)
    for curve in spire.curveList:
        pillar.curveList.append(curve)
    return pillar

def getBase():
    mesh = Mesh()
    mesh.vertexList.append(Vector(-75.0, 0.0, 75.0))
    mesh.vertexList.append(Vector(75.0, 0.0, 75.0))
    mesh.vertexList.append(Vector(75.0, 0.0, -75.0))
    mesh.vertexList.append(Vector(-75.0, 0.0, -75.0))
    mesh.vertexList.append(Vector(-75.0, 15.0, 75.0))
    mesh.vertexList.append(Vector(75.0, 15.0, 75.0))
    mesh.vertexList.append(Vector(75.0, 15.0, -75.0))
    mesh.vertexList.append(Vector(-75.0, 15.0, -75.0))
    
    mesh.normalList.append(Vector(0.0, 0.0, 1.0)) #front
    mesh.normalList.append(Vector(0.0, 0.0, -1.0)) #rear
    mesh.normalList.append(Vector(1.0, 0.0, 0.0)) #right
    mesh.normalList.append(Vector(-1.0, 0.0, 0.0)) #left
    mesh.normalList.append(Vector(0.0, 1.0, 0.0)) #top
    mesh.normalList.append(Vector(0.0, -1.0, 0.0)) #top

    frontFace = Face()
    frontFace.vertexIdList = [0, 1, 5, 4]
    frontFace.normalIdList = [0, 0, 0, 0]
    mesh.faceList.append(frontFace)

    rearFace = Face()
    rearFace.vertexIdList = [2, 3, 7, 6]
    rearFace.normalIdList = [1, 1, 1, 1]
    mesh.faceList.append(rearFace)

    rightFace = Face()
    rightFace.vertexIdList = [1, 2, 6, 5]
    rightFace.normalIdList = [2, 2, 2, 2]
    mesh.faceList.append(rightFace)

    leftFace = Face()
    leftFace.vertexIdList = [3, 0, 4, 7]
    leftFace.normalIdList = [3, 3, 3, 3]
    mesh.faceList.append(leftFace)

    topFace = Face()
    topFace.vertexIdList = [7, 4, 5, 6]
    topFace.normalIdList = [4, 4, 4, 4]
    mesh.faceList.append(topFace)

    bottomFace = Face()
    bottomFace.vertexIdList = [0, 3, 2, 1]
    bottomFace.normalIdList = [5, 5, 5, 5]
    mesh.faceList.append(bottomFace)

    return mesh

archCurve = BezierCurve(
    Point(10.0, 30.0),
    Point(10.0, 36.5),
    Point(4.0, 37.0),
    Point(0.0, 40.0),
    20)

Arch(archCurve, 50.0, 15.0).getExtrudedMesh(10.0).write("arch.3vn")
Arch(archCurve, 50.0, 15.0, 0.0).getExtrudedMesh(10.0).write("sideArch.3vn")
getCentralDome().getMeshOfRevolution(30).write("dome1.3vn")
getSmallerDome().getMeshOfRevolution(30).write("dome2.3vn")
Pillar(4, 8, 60).getProfile().getMeshOfRevolution(30).write("pillar.3vn")
getColumn().getMeshOfRevolution(30).write("column.3vn")
getBase().write("base.3vn")
