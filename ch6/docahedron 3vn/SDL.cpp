//SDL.cpp, fsh, 11/4/99 adjusted 1/30/2000 for ECE661 to admit simple raytracing
// support code for the classes in SDL.h
// some pieces not implemented - left for students to do

#include "RGBpixmap.h"
#include "SDL.h"

#define SQR(x) ((x) * (x))

//<<<<<<<<<<<<<<<<<< dot3D >>>>>>>>>>>>>>>>
float dot3D(Vector3& a, Vector3& b){return a.x * b.x + a.y * b.y + a.z * b.z;}
float dot3D(Point3&  a, Vector3& b){return a.x * b.x + a.y * b.y + a.z * b.z;}
float dot3D(Point3&  a, Point3& b) {return a.x * b.x + a.y * b.y + a.z * b.z;}

//<<<<<<<<<<<<<<<<<<<< rayPos >>>>>>>>>>>>>>>>>
Point3 rayPos(Point3& s, Vector3& v, float t)
{
	Point3 p(s.x + v.x * t,
		s.y + v.y * t,
		s.z + v.z * t);
	return p;
}

//<<<<<<<<<<<<<<<< rayPos >>>>>>>>>>>>>>>>>>>>>..
Point3 rayPos(Ray&r, float t)
{
	Point3 p(r.start.x + r.dir.x * t,
		r.start.y + r.dir.y * t,
		r.start.z + r.dir.z * t);
	return p;
}

//<<<<<<<<<<<<<<<<<<<< xfrmPoint >>>>>>>>>>>...
void xfrmPoint(Point3& res, Affine4& aff,Point3& pt)
{ // form res = aff * pt - include translation part
	res.x = aff.m[0] * pt.x + aff.m[4] * pt.y + aff.m[8]  * pt.z + aff.m[12];
	res.y = aff.m[1] * pt.x + aff.m[5] * pt.y + aff.m[9]  * pt.z + aff.m[13];
	res.z = aff.m[2] * pt.x + aff.m[6] * pt.y + aff.m[10] * pt.z + aff.m[14];
}

//<<<<<<<<<<<<<<<<<<<< xfrmVect >>>>>>>>>>>
void xfrmVect(Vector3& res, Affine4& aff,Vector3& v)
{ // form res = aff * v
	res.x = aff.m[0] * v.x + aff.m[4] * v.y + aff.m[8]  * v.z;
	res.y = aff.m[1] * v.x + aff.m[5] * v.y + aff.m[9]  * v.z;
	res.z = aff.m[2] * v.x + aff.m[6] * v.y + aff.m[10] * v.z;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<< xfrmRay >>>>>>>>>>>>>>>>>>>>>
void xfrmRay(Ray& g, Affine4& aff, Ray& r)
{// g = aff * r // only the start part gets the translation component
	xfrmPoint(g.start, aff, r.start);
	xfrmVect(g.dir, aff, r.dir);
}

//<<<<<<<<<<<<<<<<<<<< xfrmNormal >>>>>>>>>>>...
void xfrmNormal(Vector3& res, Affine4& aff,Vector3& v)
{ // form res = transpose(aff) * v
	res.x = aff.m[0] * v.x + aff.m[1] * v.y + aff.m[2]  * v.z;
	res.y = aff.m[4] * v.x + aff.m[5] * v.y + aff.m[6]  * v.z;
	res.z = aff.m[8] * v.x + aff.m[9] * v.y + aff.m[10] * v.z;
}

// Point3 methods
void Point3::set(float dx, float dy, float dz)
{
	x = dx; y = dy; z = dz;
}

void Point3::set(Point3& p)
{
	x = p.x; y = p.y; z = p.z;
}

Point3::Point3(float xx, float yy, float zz)
{
	x = xx; y = yy; z = zz;
}

Point3::Point3()
{
	x = y = z = 0;
}

void Point3::build4tuple(float v[])
{// load 4-tuple with this color: v[3] = 1 for homogeneous
	v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
}

// Vector3 methods
Vector3::Vector3(float xx, float yy, float zz)
{
	x = xx; y = yy; z = zz;
}

Vector3::Vector3(Vector3& v)
{
	x = v.x; y = v.y; z = v.z;
}

Vector3::Vector3()
{
	x = y = z = 0;
} //default constructor

void Vector3::set(float dx, float dy, float dz)
{
	x = dx; y = dy; z = dz;
}
	
void Vector3::set(Vector3& v)
{ 
	x = v.x; y = v.y; z = v.z;
}
	
void Vector3::flip()
{
	x = -x; y = -y; z = -z;
} // reverse this vector

void Vector3::setDiff(Point3& a, Point3& b)//set to difference a - b
{ 
	x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;
}

Vector3 Vector3 :: cross(Vector3 b) //return this cross b
{
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}

float Vector3 :: dot(Vector3 b) // return this dotted with b
{
	return x * b.x + y * b.y + z * b.z;
}

void Vector3 :: normalize()//adjust this vector to unit length
{
	double sizeSq = x * x + y * y + z * z;

	if(sizeSq < 0.0000001)
	{
//		cerr << "\nnormalize() sees vector (0,0,0)!!!";
		x = 1; // lousy cope - can you do better?
		return; // returns (1,0,0) - yuck!
	}
	float scaleFactor = 1.0/(float)sqrt(sizeSq);
	x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
}

// Light methods
void Light::setPosition(Point3 p)
{
	pos.set(p);
}

void Light::setColor(Color3 c)
{
	color.set(c);
}

Light::Light()
{
	next = NULL;
}

//Affine4 methods
Affine4::Affine4(){ // make identity transform
	m[0] = m[5]  = m[10] = m[15] = 1.0;
	m[1] = m[2]  = m[3]  = m[4]  = 0.0;
	m[6] = m[7]  = m[8]  = m[9]  = 0.0;
	m[11]= m[12] = m[13] = m[14] = 0.0;
}

void Affine4 :: setIdentityMatrix(){ // make identity transform
	m[0] = m[5]  = m[10] = m[15] = 1.0;
	m[1] = m[2]  = m[3]  = m[4]  = 0.0;
	m[6] = m[7]  = m[8]  = m[9]  = 0.0;
	m[11]= m[12] = m[13] = m[14] = 0.0;
}

void Affine4 ::set(Affine4 a)// set this matrix to a
{
	for(int i = 0; i < 16; i++)
		m[i]=a.m[i];
}

//<<<<<<<<<<<<<< preMult >>>>>>>>>>>>
void Affine4 ::preMult(Affine4 n)
{// postmultiplies this with n
	float sum;
	Affine4 tmp;
	tmp.set(*this); // tmp copy
	// following mult's : this = tmp * n
	for(int c = 0; c < 4; c++)
		for(int r = 0; r <4 ; r++)
		{
			sum = 0;
			for(int k = 0; k < 4; k++)
				sum += n.m[4 * k + r]* tmp.m[4 * c + k];
			m[4 * c + r] = sum;
		}// end of for loops
}// end of preMult()

//<<<<<<<<<<<< postMult >>>>>>>>>>>
void Affine4 ::postMult(Affine4 n){// postmultiplies this with n
	float sum;
	Affine4 tmp;
	tmp.set(*this); // tmp copy
	for(int c = 0; c < 4; c++)// form this = tmp * n
		for(int r = 0; r <4 ; r++)
		{
			sum = 0;
			for(int k = 0; k < 4; k++)
				sum += tmp.m[4 * k + r]* n.m[4 * c + k];
			m[4 * c + r] = sum;
		}// end of for loops
}

// AffineNode methods
AffineNode::AffineNode()
{
	next = NULL;
	affn = new Affine4; // new affine with identity in it
	invAffn = new Affine4; // and for the inverse
}

AffineNode::~AffineNode() //destructor
{
	delete affn;
	delete invAffn;
}

// AffineStack methods
AffineStack::AffineStack()
{
	tos = new AffineNode; // node with identity in it
	tos->next = NULL;
}

//<<<<<<<<<<<<<<<<<<<< dup >>>>>>>>>>>>>>>>>
void AffineStack :: dup()
{
	AffineNode* tmp = new AffineNode;
	tmp->affn = new Affine4(*(tos->affn));
	tmp->invAffn = new Affine4(*(tos->invAffn));
	tmp->next = tos;
	tos = tmp;
}

//<<<<<<<<<<<<<<<<< setIdentity >>>>>>>>>>>>>>>>>>>
void AffineStack :: setIdentity() // make top item the identity matrix
{
	assert(tos != NULL);
	tos->affn->setIdentityMatrix();
	tos->invAffn->setIdentityMatrix();
}

//<<<<<<<<<<<<<<<<<< popAndDrop >>>>>>>>>>>>>>>>>>
void AffineStack :: popAndDrop()
{
	if(tos == NULL) return; // do nothing
	AffineNode *tmp = tos;
	tos = tos->next;
	delete tmp; // should call destructor, which deletes trices
}

//<<<<<<<<<<<<<<<<<<<, releaseAffines >>>>>>>>>>>>>
void AffineStack :: releaseAffines()
{ // pop and drop all remaining items
	while(tos) popAndDrop();
}

//<<<<<<<<<<<<<<<<<<< rotate >>>>>>>>>>>>>>>
void AffineStack :: rotate(float angle, Vector3 u)
{
	Affine4 rm; // make identity matrix
	Affine4 invRm;
	u.normalize(); // make the rotation axis unit length
	float ang = angle * 3.14159265/ 180; // deg to
	float c = cos(ang), s = sin(ang);
	float mc = 1.0 - c;
 	//fill the 3x3 upper left matrix
	rm.m[0] = c + mc * u.x * u.x;
	rm.m[1] = mc * u.x * u.y + s * u.z;
	rm.m[2] = mc * u.x * u.z - s * u.y;
	rm.m[4] = mc * u.y * u.x - s * u.z;
	rm.m[5] = c + mc * u.y * u.y;
	rm.m[6] = mc * u.y * u.z + s * u.x;
	rm.m[8] = mc * u.z * u.x + s * u.y;
	rm.m[9] = mc * u.z * u.y - s * u.x;
	rm.m[10] = c + mc * u.z * u.z;
	// same for inverse : just sign of s is changed
	invRm.m[0] = c + mc * u.x * u.x;
	invRm.m[1] = mc * u.x * u.y - s * u.z;
	invRm.m[2] = mc * u.x * u.z + s * u.y;
	invRm.m[4] = mc * u.y * u.x + s * u.z;
	invRm.m[5] = c + mc * u.y * u.y;
	invRm.m[6] = mc * u.y * u.z - s * u.x;
	invRm.m[8] = mc * u.z * u.x - s * u.y;
	invRm.m[9] = mc * u.z * u.y + s * u.x;
	invRm.m[10] = c + mc * u.z * u.z;
	tos->affn->postMult(rm);
	tos->invAffn->preMult(invRm);
}

//<<<<<<<<<<<<<<<<<<<<, scale >>>>>>>>>>>>>>>>>>>>>>
void AffineStack :: scale(float sx, float sy, float sz)
{ // post-multiply top item by scaling
	#define sEps 0.00001

	Affine4 scl;// make an identity
	Affine4 invScl;
	scl.m[0]  = sx;
	scl.m[5]  = sy;
	scl.m[10] = sz;// adjust it to a scaling matrix
	if(fabs(sx) < sEps || fabs(sy) < sEps || fabs(sz) < sEps)
	{
		cerr << "degenerate scaling transformation!\n";
	}
	invScl.m[0]  = 1/sx; invScl.m[5]  = 1/sy; invScl.m[10] = 1/sz;
	tos->affn->postMult(scl); //
	tos->invAffn->preMult(invScl);
}

//<<<<<<<<<<<<<<<<<<<<< translate >>>>>>>>>>>>>>>>>>>
void AffineStack :: translate(Vector3 d)
{
	Affine4 tr; // make identity matrix
	Affine4 invTr;

	tr.m[12] = d.x; tr.m[13] = d.y;	tr.m[14] = d.z;
	invTr.m[12] = -d.x;	invTr.m[13] = -d.y; invTr.m[14] = -d.z;
	tos->affn->postMult(tr);
	tos->invAffn->preMult(invTr);
}

// Material methods
//<<<<<<<<<<<<<<<<<<<< setDefault >>>>>>>>>>>>>>>>>
void Material :: setDefault(){
	textureType = 0; // for none
	numParams = 0;
	reflectivity = transparency = 0.0;
	speedOfLight = specularExponent = 1.0;
	specularFraction = 0.0;
	surfaceRoughness = 1.0;
	ambient.set(0.1f,0.1f,0.1f);
	diffuse.set(0.8f,0.8f,0.8f);
	specular.set(0,0,0);
	emissive.set(0,0,0);
}

//<<<<<<<<<<<<<<<<<<<< set >>>>>>>>>>>>>>>>>>>
void Material :: set(Material& m)
{
	textureType = m.textureType;
	numParams = m.numParams;
	for(int i = 0; i < numParams; i++) 
		params[i] = m.params[i];
	transparency = m.transparency;
	speedOfLight = m.speedOfLight;
	reflectivity = m.reflectivity;
	specularExponent = m.specularExponent;
	specularFraction = m.specularFraction;
	surfaceRoughness = m.surfaceRoughness;
	ambient.set(m.ambient);
	diffuse.set(m.diffuse);
	specular.set(m.specular);
	emissive.set(m.emissive);
}

// GeomObj methods
GeomObj::GeomObj(): next(NULL)
{
}

bool GeomObj::hit(Ray &r, Intersection &inter)
{
	return false;
}

void GeomObj::loadStuff()
{
}

void GeomObj::drawOpenGL()
{
}

void GeomObj::tellMaterialsGL()
{
}

// Boolean methods
Boolean::Boolean():left(NULL),right(NULL)
{
}

bool Boolean::hit(Ray &r, Intersection &inter)
{
	return false;
}

void Boolean::drawOpenGL()
{ // just draw its children
	if(left)
		left->drawOpenGL();
	if(right)
		right->drawOpenGL();
}

// UnionBool methods
UnionBool::UnionBool() //constructor
{
	Boolean();
}

bool UnionBool::hit(Ray &r, Intersection &inter)
{
	return false;
}

// IntersectionBool methods
IntersectionBool::IntersectionBool()
{
	Boolean();
}

bool IntersectionBool::hit(Ray &r, Intersection &inter)
{
	return false;
}

// DifferenceBool methods
DifferenceBool::DifferenceBool()
{
	Boolean();
}

bool DifferenceBool::hit(Ray &r, Intersection &inter)
{
	return false;
}

// Shape methods
Shape::Shape()
{
	mtrl.textureType = 0; 
	mtrl.numParams = 0;
}

void Shape::setMaterial(Material& mt)
{
	mtrl.set(mt);
}

//<<<<<<<<<<<<<<<<< tellMaterialsGL >>>>>>>>>>>>>>>>>>
void Shape :: tellMaterialsGL()
{
	float amb[4],diff[4],spec[4], emiss[4];
	float zero[] = {0,0,0,1};

	mtrl.ambient.build4tuple(amb); // fill the array
	mtrl.diffuse.build4tuple(diff);
	mtrl.specular.build4tuple(spec);
	mtrl.emissive.build4tuple(emiss);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_AMBIENT,amb);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_DIFFUSE,diff);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_SPECULAR,spec);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_EMISSION,emiss);
	glMaterialf(GL_FRONT/*_AND_BACK*/,GL_SHININESS,mtrl.specularExponent);
}

void Shape::drawOpenGL()
{
}

bool Shape::hit(Ray &r, Intersection &inter)
{
	return false;
}

Color3 Shape::imageTexture(RGBpixmap &p, Point3 w, int surf)
{
	Color3 t;
	
	t.set(0.0, 0.0, 0.0);

	return t;
}

// Cube methods
Cube::Cube()
{
}

void Cube::drawOpenGL()
{
	tellMaterialsGL(); glPushMatrix();
	glMultMatrixf(transf.m); //load affine
	glEnable(GL_NORMALIZE);
	glutSolidCube(2.0);	// a cube with vertices -1 to +1
	glPopMatrix();
}

// Hit helper method: cubeNormal
Vector3 cubeNormal(int surf)
{
	Vector3 temp;
	switch(surf)
	{
		case 0:
			temp.set(0.0, 1.0, 0.0);
			break;
		case 1:
			temp.set(0.0, -1.0, 0.0);
			break;
		case 2:
			temp.set(1.0, 0.0, 0.0);
			break;
		case 3:
			temp.set(-1.0, 0.0, 0.0);
			break;
		case 4:
			temp.set(0.0, 0.0, 1.0);
			break;
		case 5:
			temp.set(0.0, 0.0, -1.0);
			break;
	}
	return temp;	
}

bool Cube::hit(Ray& r, Intersection& inter)
{
	double tHit, numer, denom;
	double tIn = -100000.0, tOut = 100000.0;// plus-minus infinity
	Ray genRay;
	int inSurf, outSurf; // which of the six surfaces

	xfrmRay(genRay, invTransf, r);
	for (int i = 0; i < 6; i++)
	{
		switch(i) // which plane of cube to test
		{
		case 0:
			numer = 1.0 - genRay.start.y; 
			denom = genRay.dir.y; 
			break;
		case 1: 
			numer = 1.0 + genRay.start.y;
			denom = -genRay.dir.y;
			break;
		case 2:
			numer = 1.0 - genRay.start.x;
			denom = genRay.dir.x;
			break;
		case 3:
			numer = 1.0 + genRay.start.x;
			denom = -genRay.dir.x;
			break;
		case 4:
			numer = 1.0 - genRay.start.z;
			denom = genRay.dir.z;
			break;
		case 5: 
			numer = 1.0 + genRay.start.z; 
			denom = -genRay.dir.z; 
			break;
		}
		if(fabs(denom) < 0.00001) // ray is parallel
			if(numer < 0) 
				return false; //ray is out;
			else
				; // ray inside, no change to tIn,tOut
		else // ray is not parallel
		{
			tHit = numer / denom;
			if(denom > 0){ // exiting
				if(tHit < tOut){ // a new earlier exit
					tOut = tHit; outSurf = i;
				}
			}
			else { // denom is negative: entering
				if(tHit > tIn){ // a new later entrance
					tIn = tHit; inSurf = i;
				}
			}
		}
		if(tIn >= tOut)
			return false; // it's a miss - early out
	} // end of the for loop
	int num = 0; // no positive hits yet
	if(tIn > 0.00001) //is first hit in front of the eye?
	{
		inter.hit[0].hitTime = tIn;
		inter.hit[0].surface = inSurf;
		inter.hit[0].isEntering = 1; // is entering
		inter.hit[0].hitObject = this;
		inter.hit[0].hitPoint.set(rayPos(genRay.start, genRay.dir,tIn));
		inter.hit[0].hitNormal.set(cubeNormal(inSurf));
		num++; //have a hit
	}
	if(tOut > 0.00001)
	{
		inter.hit[num].hitTime = tOut;
		inter.hit[num].surface = outSurf;
		inter.hit[num].isEntering = 0; // is exiting
		inter.hit[num].hitObject = this;
		inter.hit[num].hitPoint.set(rayPos(genRay.start,genRay.dir,tOut));
		inter.hit[num].hitNormal.set(cubeNormal(outSurf));
		num++;
	}
	inter.numHits = num; //number of hits in front of eye
	return (num > 0);	
}

Color3 Cube::imageTexture(RGBpixmap &p, Point3 w, int surf)
{	
	Point3 t;
	float u, v;
	int r, c;

	xfrmPoint(t, invTransf, w); // get the point in generic coordinates

	switch(surf)
	{
		case 0:
		case 1:
			u = (t.x - mtrl.params[0]) / (mtrl.params[3] - mtrl.params[0]);
			v = (t.z - mtrl.params[1]) / (mtrl.params[2] - mtrl.params[1]);
			break;
		case 2:
		case 3:
			u = (t.z - mtrl.params[0]) / (mtrl.params[3] - mtrl.params[0]);
			v = (t.y - mtrl.params[1]) / (mtrl.params[2] - mtrl.params[1]);
			break;
		case 4:
		case 5:
			u = (t.x - mtrl.params[0]) / (mtrl.params[3] - mtrl.params[0]);
			v = (t.y - mtrl.params[1]) / (mtrl.params[2] - mtrl.params[1]);
			break;
	}	
	
	r = (int) (u * p.nRows);
	c = (int) (v * p.nCols);
	if(r >= p.nRows)
		r = 0;
	if(c >= p.nCols)
		c = 0;
	return p.getPixel(r, c);
}

// Sphere methods
Sphere::Sphere()
{
}

void Sphere::drawOpenGL()
{
	tellMaterialsGL(); 	
	glPushMatrix();
	glMultMatrixf(transf.m);
	glutSolidSphere(1.0,20,20);
	glPopMatrix();
}

//<<<<<<<<<<<<<<<<<< hit() for Sphere >>>>>>>>>>>>>>>>>>>>>>>
bool Sphere::hit(Ray &r, Intersection &inter)
{  // intersect the ray with the generic sphere
	Ray genRay;
	xfrmRay(genRay,invTransf,r);
	double A, B, C, discrim, disc_root, t1, t2;

	A = dot3D(genRay.dir, genRay.dir); 
	assert(A > 0); // fails if ray is sick
	B = dot3D(genRay.start, genRay.dir);
	C = dot3D(genRay.start, genRay.start) - 1.0;
	discrim = B * B - A * C;
	if(discrim < 0.0)
		return false;   // ray missed: numHits = 0
	disc_root = sqrt(discrim);
	inter.numHits = 0;	// no hits yet
	t1 = (-B - disc_root)/A;         // earlier hit
	if(t1 > 0.00001) // a pos hit time
	{
		inter.hit[0].hitTime = t1;
		Point3 P(rayPos(genRay, t1));// position of hit spot
		inter.hit[0].hitPoint.set(P);
		inter.hit[0].hitObject = this;
		inter.hit[0].isEntering = 1;
		inter.hit[0].surface = 0;
		inter.hit[0].hitNormal.set(P.x, P.y, P.z); // vector is same as hit spot
		inter.numHits++; // got one hit so far
	}
	t2 = (-B + disc_root)/A; 		// second hit
	if(t2 > 0.00001) // got a hit (first or second...)
	{
		inter.hit[inter.numHits].hitTime = t2;
		Point3 P(rayPos(genRay, t2));  // position of hit spot
		inter.hit[inter.numHits].hitPoint.set(P);
		inter.hit[inter.numHits].isEntering = 0;
		inter.hit[inter.numHits].hitObject = this;
		inter.hit[inter.numHits].surface = 0;
		inter.hit[inter.numHits].hitNormal.set(P.x, P.y, P.z); // vector is same as hit spot
		inter.numHits++;			// got another
	}
	return inter.numHits > 0;
}// end of hit() for Sphere

// Tapered Cylinder methods
TaperedCylinder::TaperedCylinder()
{
	pwall = gluNewQuadric(); //make a cylinder 
	pbase = gluNewQuadric(); //make a cylinder 
	pcap  = gluNewQuadric(); //make a cylinder 

	gluQuadricDrawStyle(pwall,GLU_FILL); 
	gluQuadricDrawStyle(pbase,GLU_FILL); 
	gluQuadricDrawStyle(pcap, GLU_FILL); 
}

void TaperedCylinder::drawOpenGL()
{
	tellMaterialsGL(); 
	glPushMatrix(); 
		glMultMatrixf(transf.m); 
		glEnable(GL_NORMALIZE); 
		gluCylinder(pwall,1.0,smallRadius,1.0, 30,4); // draw the cylinder wall 
		glPushMatrix(); 
			glScalef(1, 1, -1); //flip base so normals point outside 
			gluDisk(pbase, 0, 1, 30, 8); // draw base at z = 0             
		glPopMatrix(); 
		glTranslatef(0,0,1); 
		gluDisk(pcap, 0, smallRadius, 30, 8); //draw cap at z = 1 
	glPopMatrix(); 
}

bool TaperedCylinder::hit(Ray &r, Intersection &inter)
{
	Ray genRay;
	xfrmRay(genRay, invTransf, r);
	double A, B, C, discrim, disc_root, t1, t2, tb, tc;
	double sm = smallRadius - 1.0;
	double fDir = sm * genRay.dir.z;
	double fStart = (sm * genRay.start.z) + 1;
	int num = 0;

	A = (genRay.dir.x * genRay.dir.x) + (genRay.dir.y * genRay.dir.y) - 
		(fDir * fDir);
	B = (genRay.start.x * genRay.dir.x) + (genRay.start.y * genRay.dir.y) -
		(fDir * fStart);
	C = (genRay.start.x * genRay.start.x) + (genRay.start.y * genRay.start.y) -
		(fStart * fStart);
	discrim = (B * B) - (A * C);
	if(discrim > 0.0) //can take square root
	{		
		disc_root = (double)sqrt(double(discrim));
		t1 = (-B - disc_root)/A; // earlier hit
		float zHit = genRay.start.z + genRay.dir.z * t1; //z component of ray
		if(t1 > 0.00001 && zHit <= 1.0 && zHit >= 0)
		{
			inter.hit[num].hitTime = t1;
			inter.hit[num++].surface = 0; //hit is with wall
		}
		t2 = (-B + disc_root)/A; // second hit
		zHit = genRay.start.z + genRay.dir.z * t2;
		if(t2 > 0.00001 && zHit <= 1.0 && zHit >=0)
		{
			inter.hit[num].hitTime = t2;
			inter.hit[num++].surface = 0; //hit is with wall
		}
	} // end if(discrim > 0)
	//test the base at z = 0
	tb = -genRay.start.z/genRay.dir.z; //hit time at z = 0 plane
	if(tb > 0.00001 && SQR(genRay.start.x + genRay.dir.x * tb) +
		SQR(genRay.start.y + genRay.dir.y * tb) < 1) //within disc of base
	{
		inter.hit[num].hitTime = tb;
		inter.hit[num++].surface = 1; //1 for the base
	}
	//test the cap at z = 1
	tc = (1 - genRay.start.z)/genRay.dir.z; //hit time at z = 1 plane
	if(tc > 0.00001 && SQR(genRay.start.x + genRay.dir.x * tc) +
		SQR(genRay.start.y + genRay.dir.y * tc) < SQR(smallRadius)) //within disc
	{
		inter.hit[num].hitTime = tc;
		inter.hit[num++].surface = 2; // 2 for the cap
	}
	if(num == 0) return false; // missed everything, or behind the eye
	inter.numHits = num;
	if(num == 1) // eye inside cylinder, only have the exiting hit
	{
		inter.hit[0].isEntering = false;
		inter.hit[0].hitObject = this;
	}
	else // have two hits - first must be entering
	{ //now sort the two hits
		if(inter.hit[0].hitTime > inter.hit[1].hitTime) // must reverse them
		{// need only swap the hitTime and surface fields
			double tmpT = inter.hit[0].hitTime; //swap times
			inter.hit[0].hitTime = inter.hit[1].hitTime;
			inter.hit[1].hitTime = tmpT;
			int tmpS = inter.hit[0].surface; //swap surfaces
			inter.hit[0].surface = inter.hit[1].surface;
			inter.hit[1].surface = tmpS;
		}
		inter.hit[0].isEntering = true; inter.hit[1].isEntering = false;
		inter.hit[0].hitObject = inter.hit[1].hitObject = this;
	}
	// now set the hit point and normal for the hit or hits
	for(int i = 0; i < num; i++)
	{
		Point3 P0(rayPos(genRay, inter.hit[i].hitTime));// position of firsthit
		inter.hit[i].hitPoint.set(P0);
		int surf = inter.hit[i].surface;
		if(surf == 0) // wall
			inter.hit[i].hitNormal.set(P0.x, P0.y, -sm * (1 + sm * P0.z));
		else if(surf == 1) // base
			inter.hit[i].hitNormal.set(0,0,-1);
		else inter.hit[i].hitNormal.set(0,0,1); //cap
	}
	return true;
}

Color3 TaperedCylinder::imageTexture(RGBpixmap &p, Point3 w, int surf)
{
	Point3 t;
	float u, v, theta;
	int r, c;

	xfrmPoint(t, invTransf, w); // get the point in generic coordinates

	theta = atan2(w.y, w.x);

	u = (theta - mtrl.params[0]) / (mtrl.params[1] - mtrl.params[0]);
	v = (w.z - mtrl.params[2]) / (mtrl.params[3] - mtrl.params[2]);

	r = (int) (u * p.nRows);
	c = (int) (v * p.nCols);
	if(r >= p.nRows)
		r = 0;
	if(c >= p.nCols)
		c = 0;
	return p.getPixel(r, c);
}

// Tapered Tube methods
TaperedTube::TaperedTube()
{
	pwall = gluNewQuadric(); //make a cylinder 
	pbase = gluNewQuadric(); //make a cylinder 
	pcap  = gluNewQuadric(); //make a cylinder 

	gluQuadricDrawStyle(pwall,GLU_FILL); 
	gluQuadricDrawStyle(pbase,GLU_FILL); 
	gluQuadricDrawStyle(pcap, GLU_FILL);
}

void TaperedTube::drawOpenGL()
{ 
	tellMaterialsGL(); 
	glPushMatrix(); 
		glMultMatrixf(transf.m); 
		glEnable(GL_NORMALIZE); 
		gluCylinder(pwall,1.0,smallRadius,1.0, 30,4); // draw the cylinder wall 
		glPushMatrix(); 
			glScalef(1, 1, -1); //flip base so normals point outside 
			gluDisk(pbase, 1-thickness, 1, 30, 8); // draw base at z = 0             
		glPopMatrix(); 
		glTranslatef(0,0,1); 
		gluDisk(pcap, 0, smallRadius, 30, 8); //draw cap at z = 1 
	glPopMatrix();
}

bool TaperedTube::hit(Ray &r, Intersection &inter)
{
	Ray genRay;
	xfrmRay(genRay, invTransf, r);
	double A, B, C, discrim, disc_root, t1, t2, tb, tc;
	double sm = smallRadius - 1.0;
	double sm2;
	double fDir = sm * genRay.dir.z;
	double fStart = (sm * genRay.start.z) + 1;
	int num = 0;

	// Check outer wall:
	A = (genRay.dir.x * genRay.dir.x) + (genRay.dir.y * genRay.dir.y) - 
		(fDir * fDir);
	B = (genRay.start.x * genRay.dir.x) + (genRay.start.y * genRay.dir.y) -
		(fDir * fStart);
	C = (genRay.start.x * genRay.start.x) + (genRay.start.y * genRay.start.y) -
		(fStart * fStart);
	discrim = (B * B) - (A * C);
	if(discrim > 0.0) //can take square root
	{		
		disc_root = (double)sqrt(double(discrim));
		t1 = (-B - disc_root)/A; // earlier hit
		float zHit = genRay.start.z + genRay.dir.z * t1; //z component of ray
		if(t1 > 0.00001 && zHit <= 1.0 && zHit >= 0)
		{
			inter.hit[num].hitTime = t1;
			inter.hit[num++].surface = 0; //hit is with outer wall
		}
		t2 = (-B + disc_root)/A; // second hit
		zHit = genRay.start.z + genRay.dir.z * t2;
		if(t2 > 0.00001 && zHit <= 1.0 && zHit >=0)
		{
			inter.hit[num].hitTime = t2;
			inter.hit[num++].surface = 0; //hit is with wall
		}
	} 
	// Check inner wall:
	sm2 = (smallRadius - thickness) - 1.0;
	fDir = sm2 * genRay.dir.z;
	fStart = (sm2 * genRay.start.z) + 1;
	A = (genRay.dir.x * genRay.dir.x) + (genRay.dir.y * genRay.dir.y) - 
		(fDir * fDir);
	B = (genRay.start.x * genRay.dir.x) + (genRay.start.y * genRay.dir.y) -
		(fDir * fStart);
	C = (genRay.start.x * genRay.start.x) + (genRay.start.y * genRay.start.y) -
		(fStart * fStart);
	discrim = (B * B) - (A * C);
	if(discrim > 0.0) //can take square root
	{		
		disc_root = (double)sqrt(double(discrim));
		t1 = (-B - disc_root)/A; // earlier hit
		float zHit = genRay.start.z + genRay.dir.z * t1; //z component of ray
		if(t1 > 0.00001 && zHit <= 1.0 && zHit >= 0)
		{
			inter.hit[num].hitTime = t1;
			inter.hit[num++].surface = 1; //hit is with inner wall
		}
		t2 = (-B + disc_root)/A; // second hit
		zHit = genRay.start.z + genRay.dir.z * t2;
		if(t2 > 0.00001 && zHit <= 1.0 && zHit >=0)
		{
			inter.hit[num].hitTime = t2;
			inter.hit[num++].surface = 1; //hit is with inner wall
		}
	} 
	//test the base at z = 0
	tb = -genRay.start.z/genRay.dir.z; //hit time at z = 0 plane
	float temp = SQR(genRay.start.x + genRay.dir.x * tb) +
			   SQR(genRay.start.y + genRay.dir.y * tb);
	if((tb > 0.00001) &&  ((temp < 1) && (temp > (1 - thickness)))) //within disc of base
	{
		inter.hit[num].hitTime = tb;
		inter.hit[num++].surface = 2; //2 for the base
	}

	//test the cap at z = 1
	tc = (1 - genRay.start.z)/genRay.dir.z; //hit time at z = 1 plane
	temp = SQR(genRay.start.x + genRay.dir.x * tc) +
		 SQR(genRay.start.y + genRay.dir.y * tc);
	if(tc > 0.00001 &&  ((temp < SQR(smallRadius)) && 
				         (temp > (SQR(smallRadius) - thickness)))) //within disc
	{
		inter.hit[num].hitTime = tc;
		inter.hit[num++].surface = 3; // 3 for the cap
	}

	if(num == 0) 
		return false; // missed everything, or behind the eye
	inter.numHits = num;
	if(num < 4 ) // eye inside cylinder, only have the exiting hit
	{
		inter.hit[0].isEntering = false;
		inter.hit[0].hitObject = this;
	}
	else // have two hits - first must be entering
	{ //now sort the two hits
		if(inter.hit[0].hitTime > inter.hit[1].hitTime) // must reverse them
		{// need only swap the hitTime and surface fields
			double tmpT = inter.hit[0].hitTime; //swap times
			inter.hit[0].hitTime = inter.hit[1].hitTime;
			inter.hit[1].hitTime = tmpT;
			int tmpS = inter.hit[0].surface; //swap surfaces
			inter.hit[0].surface = inter.hit[1].surface;
			inter.hit[1].surface = tmpS;
		}
		inter.hit[0].isEntering = true; inter.hit[1].isEntering = false;
		inter.hit[0].hitObject = inter.hit[1].hitObject = this;
	}
	// now set the hit point and normal for the hit or hits
	for(int i = 0; i < num; i++)
	{
		Point3 P0(rayPos(genRay, inter.hit[i].hitTime));// position of firsthit
		inter.hit[i].hitPoint.set(P0);
		int surf = inter.hit[i].surface;
		if(surf == 0) // outer wall
			inter.hit[i].hitNormal.set(P0.x, P0.y, -sm * (1 + sm * P0.z));
		else if(surf == 1) // inner wall
			inter.hit[i].hitNormal.set(P0.x, P0.y, -sm2 * (1 + sm2 * P0.z));
		else if(surf == 2) // base
			inter.hit[i].hitNormal.set(0,0,-1);
		else inter.hit[i].hitNormal.set(0,0,1); //cap
	}
	return true;
}

// Square methods
Square::Square()
{
}

void Square::drawOpenGL()
{
	tellMaterialsGL();
	glPushMatrix();
	glMultMatrixf(transf.m); //load affine
	glEnable(GL_NORMALIZE);
	glScaled(1.0, 1.0, 0.01);
	glutSolidCube(2.0);
	glPopMatrix();
}

//<<<<<<<<<<<<<<<<< hit for Square >>>>>>>>>>>>>>>>.
bool Square:: hit(Ray &r, Intersection& inter)
{
	Ray genRay; // need to make the generic ray
	inter.numHits = 0; // initial assumption

	xfrmRay(genRay, invTransf, r);
	double denom = genRay.dir.z; //denominator
	
	if(fabs(denom) < 0.0001) 
		return false; //ray parallel to plane: miss
	double time = -genRay.start.z/denom; //hit time
	if(time <= 0.0) 
		return false; // it lies behind the eye
	double hx = genRay.start.x + genRay.dir.x * time;//x at hit
	double hy = genRay.start.y + genRay.dir.y * time;// y at hit
	if((hx > 1.0) || (hx < -1.0)) 
		return false; // misses in x-direction
	if((hy > 1.0) || (hy < -1.0)) 
		return false; // misses in y-direction
	inter.numHits = 1; // have a hit
	inter.hit[0].hitObject = this;
	inter.hit[0].hitTime = time;
	inter.hit[0].isEntering = true;
	inter.hit[0].surface = 0;
	inter.hit[0].hitPoint.set(hx,hy,0);
	inter.hit[0].hitNormal.set(0,0,1);
	return true;
}

Color3 Square::imageTexture(RGBpixmap &p, Point3 w, int surf)
{
	Point3 t;
	float u, v;

	xfrmPoint(t, invTransf, w);
	u = (t.x - mtrl.params[0]) / (mtrl.params[3] - mtrl.params[0]);
	v = (t.y - mtrl.params[1]) / (mtrl.params[2] - mtrl.params[1]);
	
	return p.getPixel((int) (u * p.nRows), (int) (v * p.nCols));
}

// Plane methods
Plane::Plane()
{
}

void Plane::drawOpenGL()
{ 
	tellMaterialsGL();
	glPushMatrix();
	glMultMatrixf(transf.m); //load affine
	glEnable(GL_NORMALIZE);
	glScaled(30.0, 30.0, 0.01);
	glutSolidCube(2.0);
	glPopMatrix();
}

bool Plane::hit(Ray &r, Intersection &inter)
{
	return false;
}

// Face methods
Face::Face()
{
	nVerts = 0; 
	vert = NULL;
}

Face::~Face()
{
	delete[] vert; 
	nVerts = 0;
}

//Mesh methods
//<<<<<<<<<<<<<<<<< constructor >>>>>>>>>>>>>>>>>>>
Mesh :: Mesh()
{
	numVerts = numFaces = numNorms = 0;
	pt = NULL; norm  =  NULL; face = NULL;
	lastVertUsed = lastNormUsed = lastFaceUsed = -1;
}

//<<<<<<<<<<<<<<<<<<<<<< freeMesh >>>>>>>>>>>>>>>>>>
void Mesh :: freeMesh()
{ // free up memory used by this mesh.
	delete [] pt; // release whole vertex list
	delete [] norm;
	for(int f = 0; f < numFaces; f++)
		delete[] face[f].vert; // delete the vert[] array of this face
	delete [] face;
}

//<<<<<<<<<<<<<<<<<<<< isEmpty >>>>>>>>>>>>>>>>>>>>
int Mesh :: isEmpty()
{
	return (numVerts == 0) || (numFaces == 0) || (numNorms == 0);
}

//<<<<<<<<<<<<<<<<<<<<< makeEmpty >>>>>>>>>>>>>>>>>>>>>.
void Mesh :: makeEmpty()
{
	numVerts = numFaces = numNorms = 0;
}

//<<<<<<<<<<<<<<<<<<< drawOpenGL >>>>>>>>>>>>>>>>>>>
void Mesh :: drawOpenGL()
{
	tellMaterialsGL(); 	
	glPushMatrix();
	glMultMatrixf(transf.m);
	drawMesh();
	glPopMatrix();
}

//<<<<<<<<<<<<<<<<<< constructor >>>>>>>>>>>>>>>>
Mesh :: Mesh(string fname)
{ // read this file to build mesh
	numVerts = numFaces = numNorms = 0;
	pt = NULL; norm  =  NULL; face = NULL;
	lastVertUsed = lastNormUsed = lastFaceUsed = -1;
	readMesh(fname);
}

//<<<<<<<<<<<<<<<<<<< newell4 >>>>>>>>>>>>>>>>>>>>
Vector3 Mesh :: newell4(int indx[])
{ /* return the normalized normal to face with vertices
     pt[indx[0]],...,pt[indx[3]]. i.e. indx[] contains the four indices
 	 into the vertex list to be used in the Newell calculation */
	Vector3 m;

	for(int i = 0; i < 4 ; i++)
	{
		int next = (i== 3) ? 0 : i + 1; // which index is next?
		int f = indx[i], n = indx[next]; // names for the indices in the pair
		m.x += (pt[f].y - pt[n].y) * (pt[f].z + pt[n].z);
		m.y += (pt[f].z - pt[n].z) * (pt[f].x + pt[n].x);
		m.z += (pt[f].x - pt[n].x) * (pt[f].y + pt[n].y);
	}
	m.normalize();
	return m;
}

//<<<<<<<<<<<<<<<<<<<<< readMesh >>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: readMesh(string fname)
{
	fstream inStream;

	inStream.open(fname.c_str(), ios ::in); //open needs a c-like string
	if(inStream.fail() || inStream.eof())
	{
		cout << "can't open file or eof: " << fname << endl;
		makeEmpty();
		return;
	}
	inStream >> numVerts >> numNorms >> numFaces;
	// make arrays for vertices, normals, and faces
	pt = new Point3[numVerts];        assert(pt != NULL);
	norm = new Vector3[numNorms];     assert(norm != NULL);
	face = new Face[numFaces];        assert(face != NULL);
	for(int i = 0; i < numVerts; i++) 	// read in the vertices
		inStream >> pt[i].x >> pt[i].y >> pt[i].z;
	for(int ii = 0; ii < numNorms; ii++) 	// read in the normals
		inStream >> norm[ii].x >> norm[ii].y >> norm[ii].z;
	for(int f = 0; f < numFaces; f++)   // read in face data
	{
		inStream >> face[f].nVerts;
		int n = face[f].nVerts;
		face[f].vert = new VertexID[n]; assert(face[f].vert != NULL);
		for(int k = 0; k < n; k++) 		// read vertex indices for this face
			inStream >> face[f].vert[k].vertIndex;
		for(int kk = 0; kk < n; kk++) 		// read normal indices for this face
			inStream >> face[f].vert[kk].normIndex;
	}
	inStream.close();
} // end of readMesh

//<<<<<<<<<<<<<<<<<<<<<< drawMesh >>>>>>>>>>>>>>>>>>>>

void Mesh::drawMesh()
{ // draw each face of this mesh using OpenGL: draw each polygon.
	if(isEmpty()) 
		return; // mesh is empty
	for(int f = 0; f < numFaces; f++)
	{
		int n = face[f].nVerts;
		glBegin(GL_POLYGON);
		for(int v = 0; v < n; v++)
		{
			int in = face[f].vert[v].normIndex;
			assert(in >= 0 && in < numNorms);
			glNormal3f(norm[in].x, norm[in].y, norm[in].z);
			int iv = face[f].vert[v].vertIndex; assert(iv >= 0 && iv < numVerts);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
	glFlush();
}

bool Mesh::hit(Ray &r, Intersection &inter)
{
	double tHit, numer, denom;
	double tIn = -100000.0, 
		   tOut = 100000.0;// plus-minus infinity
	Ray genRay;
	int inSurf, outSurf; // which of the six surfaces
	Point3 point;
	Vector3 diff, normal;

	xfrmRay(genRay, invTransf, r);
	for (int i = 0; i < numFaces; i++)
	{
		normal.set(norm[face[i].vert[0].normIndex]);
		point.set(pt[face[i].vert[0].vertIndex]);
		
		diff.setDiff(point, genRay.start);
		numer = dot3D(normal, diff);
		denom = dot3D(normal, genRay.dir);

		if(fabs(denom) < 0.00001) // ray is parallel
			if(numer < 0) 
				return false; //ray is out;
			else ; // ray inside, no change to tIn,tOut
		else // ray is not parallel
		{
			tHit = numer / denom;
			if(denom > 0){ // exiting
				if(tHit < tOut) // a new earlier exit
				{ 
					tOut = tHit; 
					outSurf = i;
				}
			}
			else { // denom is negative: entering
				if(tHit > tIn) // a new later entrance
				{ 
					tIn = tHit; 
					inSurf = i;
				}
			}
		}
		if(tIn >= tOut)
			return false; // it's a miss - early out
	} // end of the for loop
	int num = 0; // no positive hits yet
	if(tIn > 0.00001) //is first hit in front of the eye?
	{
		inter.hit[0].hitTime = tIn;
		inter.hit[0].surface = inSurf;
		inter.hit[0].isEntering = 1; // is entering
		inter.hit[0].hitObject = this;
		inter.hit[0].hitPoint.set(rayPos(genRay.start, genRay.dir,tIn));
		inter.hit[0].hitNormal.set(norm[face[inSurf].vert[0].normIndex]);
		num++; //have a hit
	}
	if(tOut > 0.00001)
	{
		inter.hit[num].hitTime = tOut;
		inter.hit[num].surface = outSurf;
		inter.hit[num].isEntering = 0; // is exiting
		inter.hit[num].hitObject = this;
		inter.hit[num].hitPoint.set(rayPos(genRay.start,genRay.dir,tOut));
		inter.hit[num].hitNormal.set(norm[face[outSurf].vert[0].normIndex]);
		num++;
	}
	inter.numHits = num; //number of hits in front of eye
	return (num > 0);
}

// Torus methods
void Torus::drawOpenGL()
{
	tellMaterialsGL();
	glPushMatrix();
	glMultMatrixf(transf.m);
	glutSolidTorus(0.2,1.0,10,12);
	glPopMatrix();
}

bool Torus::hit(Ray &r, Intersection &inter)
{
	return false;
}

// Teapot methods
void Teapot::drawOpenGL()
{ 
	tellMaterialsGL(); 
	glPushMatrix();
	glMultMatrixf(transf.m);
	glutSolidTeapot(1.0); 
	glPopMatrix();
}

bool Teapot::hit(Ray &r, Intersection &inter)
{
	return false;
}

// DefUnit methods
DefUnit::DefUnit(string n, string s) 
{
	stuff = s;
	name = n;
}

// Noise methods:
// a constructor
Noise::Noise()
{
	int i;
	index = new unsigned char[256];

	for(i = 0; i < 256; i++) 
		index[i] = i; //fill array with indices
	for(i = 0; i < 256; i++) // shuffle it
	{
		int which = rand() % 256; // choose random place in array
		unsigned char tmp = index[which]; // swap them
		index[which] = index[i];
		index[i] = tmp;
	}
	noiseTable = new float[256];
	for(i = 0; i < 256; i++) 
		noiseTable[i] = rand()/32767.99;
} // end of constructor
	
float Noise::noise(float x, float y, float z) 
{
	float d[2][2][2];	
	
	x += 10000; //offset avoids negative values
	y += 10000;
	z += 10000;
	long ix = (long)x; long iy = (long)y; long iz = (long)z;
	float tx,ty,tz, x0,x1,x2,x3, y0,y1;

	tx = x - ix; ty = y - iy; tz = z - iz; // fractional parts
	float mtx = 1.0 - tx, mty = 1.0 - ty, mtz = 1.0 - tz;

	for(int k = 0; k <= 1; k++) // get noise at 8 lattice points
		for(int j = 0; j <= 1; j++)
			for(int i = 0; i <= 1; i++)
				d[k][j][i] = latticeNoise(ix + i, iy + j,iz + k);

	x0 = lerp(tx, d[0][0][0],d[0][0][1]);
	x1 = lerp(tx, d[0][1][0],d[0][1][1]);
	x2 = lerp(tx, d[1][0][0],d[1][0][1]);
	x3 = lerp(tx, d[1][1][0],d[1][1][1]);
	y0 = lerp(ty, x0, x1);
	y1 = lerp(ty, x2, x3);
	return lerp(tz, y0, y1);	
}

float Noise::noise(float scale, Point3& p)
{
	float d[2][2][2];
	Point3 pp;
	
	pp.x = p.x * scale + 10000; //offset avoids negative values
	pp.y = p.y * scale + 10000;
	pp.z = p.z * scale + 10000;
	long ix = (long)pp.x; long iy = (long)pp.y; long iz = (long)pp.z;
	float tx,ty,tz, x0,x1,x2,x3, y0,y1;

	tx = pp.x - ix; ty = pp.y - iy; tz = pp.z - iz; // fractional parts
	float mtx = 1.0 - tx, mty = 1.0 - ty, mtz = 1.0 - tz;

	for(int k = 0; k <= 1; k++) // get noise at 8 lattice points
		for(int j = 0; j <= 1; j++)
			for(int i = 0; i <= 1; i++)
				d[k][j][i] = latticeNoise(ix + i, iy + j,iz + k);

	x0 = lerp(tx, d[0][0][0],d[0][0][1]);
	x1 = lerp(tx, d[0][1][0],d[0][1][1]);
	x2 = lerp(tx, d[1][0][0],d[1][0][1]);
	x3 = lerp(tx, d[1][1][0],d[1][1][1]);
	y0 = lerp(ty, x0, x1);
	y1 = lerp(ty, x2, x3);
	return lerp(tz, y0, y1);
}

float Noise::turbulence(float s, Point3& p) {
	return .5*noise(s,p) + .25 * noise(2*s, p) + .0125 * noise(4*s, p);
}

float Noise::marble(float x, float y, float z) {
	return mySpline(sin(z)); 
}

float Noise::marble(float strength,Point3& p) {
	float A = 6.0;
	return mySpline(sin((2*3.1415926*p.z) + (A * turbulence(strength, p))));
}

// used for marble
float Noise::mySpline(float x) {
	if(x < -0.4) 
		return 0.15 + 2.857 * SQR(x + 0.75);
	else if(x < 0.4) 
		return 0.95 - 2.8125 * SQR(x);
	else 
		return 0.26 + 2.666 * SQR(x - 0.7);
}

// return noise value on an integer lattice
float Noise::latticeNoise(int i, int j, int k)
{ 
	return noiseTable[INDEX(i,j,k)];
}
// End Noise methods

// Scene methods
Scene::Scene():light(NULL),obj(NULL),tail(NULL) //default constructor
{
	currMtrl.setDefault();
	background.set(0,0,0.6f);
	ambient.set(0.1f,0.1f,0.1f);
	minReflectivity = 0.5;
	minTransparency = 0.5;
	maxRecursionDepth = 3;
	loud = new Noise();
}

Scene::Scene(string fname)
{
	Scene(); 
	read(fname);
}

//<<<<<<<<<<<<<<<<< nextToken>>>>>>>>>>>>>>>>>>>
string Scene :: nexttoken(void)
{
	char c;
	string token;
	int lastchar = 1;

	if (!f_in) 
	{
		return(token);
	}
	if (f_in->eof()) 
	{
		return(token);
	}
	while (f_in->get(c))
	{
		if (f_in->eof()) {
			return(token);
		}
		switch (c) {
			case '\n': nextline += 1;
			case ' ' :
			case '\t':
			case '\a':
			case '\b':
			case '\v':
			case '\f':
			case '\r': {
				if ( lastchar == 0 ) 
				{
					return(token);
				}
				break; 
			}
			case '{': {
				token = c; 
				return(token); 
				break;
			}
			case '}': {
				token = c;
				return(token);
				break; 
			}
			case '!': {
				while ( c != '\n' && f_in->get(c)) 
				{
				}
				nextline++; 
				break;
			}
			default: {
				token = token + c;
				lastchar = 0;
				if ((f_in->peek() == '{') ||
					(f_in->peek() == '}') ) 
				{
					if ( lastchar == 0 ) {
						return(token);
					} else {
						f_in->get(c);
						token = c;
						return(token);
					}
				}
				line = nextline;
			}
		}	
	}
	return(" ");
}

//<<<<<<<<<<<<<< getFloat >>>>>>>>>>>>>>>>
float Scene :: getFloat() //############ getFloat()
{
	strstream tmp;
	float number;
	string str = nexttoken();

	tmp << str;
	if(!(tmp >> number))
	{
		cerr << "Line " << line << ": error getting float" << endl;
		exit(-1);
	}
	else
	{
		char t;
		if ( (tmp >> t ) )
		{
			cerr << "Line " << line << ": bum chars in number" << endl;
			exit(-1);
		}
	}
	return number;
}

//<<<<<<<<<<<<<<<<< isidentifier >>>>>>>>>>>>>>>>
bool Scene :: isidentifier(string keyword) { //######## isidentifier
	string temp = keyword;

	if (!isalpha(temp[0])) 
		return(false);
	for (int count = 1; count < temp.length(); count++) 
	{
		if ((!isalnum(temp[count]))&& (temp[count]!='.')) 
			return(false);
	}
	return(true);
}

//<<<<<<<<<<<<<<<< cleanUp >>>>>>>>>>>>>>>>
void Scene :: cleanUp() //######### cleanUp
{ // release stuff after parsing file
	affStk.releaseAffines(); 		//delete affine stack
	def_stack->release();
	delete def_stack; // release the DefUnitStack memory
}

//<<<<<<<<<<<<<<<<< freeScene >>>>>>>>>>>>>>
void Scene :: freeScene()
{ // release the object and light lists
	GeomObj *p = obj;
	while(p)
	{
		GeomObj* q = p;
		p = p->next;
		delete q;
	}
	Light * q = light;
	while(q)
	{
		Light* r = q;
		q = q->next;
		delete r;
	}
}

void Scene::makeLightsOpenGL()
{
	/* to be implemented */
}

//<<<<<<<<<<<<<<<<< whichToken >>>>>>>>>>>>>>>
mTokenType Scene :: whichtoken(string keyword)
{
	string temp = keyword;

	if ( temp == "light" )	  		  return LIGHT;
	if ( temp == "rotate" )			  return ROTATE;
	if ( temp == "translate" ) 		  return TRANSLATE;
	if ( temp == "scale")			  return (SCALE);
	if ( temp == "push")			  return (PUSH);
	if ( temp == "pop")				  return (POP);
	if ( temp == "identityAffine")    return (IDENTITYAFFINE);
	if ( temp == "cube")			  return (CUBE);
	if ( temp == "sphere")			  return (SPHERE);
	if ( temp == "torus")			  return (TORUS);
	if ( temp == "plane")			  return (PLANE);
	if ( temp == "square")			  return (SQUARE);
	if ( temp == "cylinder")		  return (CYLINDER);
	if ( temp == "taperedCylinder")   return (TAPEREDCYLINDER);
	if ( temp == "taperedTube")		  return (TAPEREDTUBE); 
	if ( temp == "cone")			  return (CONE);
	if ( temp == "tetrahedron")       return (TETRAHEDRON);
	if ( temp == "octahedron")        return (OCTAHEDRON);
	if ( temp == "dodecahedron")      return (DODECAHEDRON);
	if ( temp == "icosahedron")       return (ICOSAHEDRON);
	if ( temp == "buckyball")         return (BUCKYBALL);
	if ( temp == "diamond")           return (DIAMOND);
	if ( temp == "wineglass")		  return (WINEGLASS);
	if ( temp == "teapot")			  return (TEAPOT);
	if ( temp == "union")             return (UNION);
	if ( temp == "intersection")      return (INTERSECTION);
	if ( temp == "difference")        return (DIFFERENCEa);
	if ( temp == "mesh")              return (MESH);
	if ( temp == "makePixmap")        return (MAKEPIXMAP);
	if ( temp == "defaultMaterials")  return (DEFAULTMATERIALS);
	if ( temp == "ambient")           return (AMBIENT);
	if ( temp == "diffuse")           return (DIFFUSE);
	if ( temp == "specular")          return (SPECULAR);
	if ( temp == "specularFraction")  return (SPECULARFRACTION);
	if ( temp == "surfaceRoughness")  return (SURFACEROUGHNESS);
	if ( temp == "emissive")          return (EMISSIVE);
	if ( temp == "specularExponent")  return (SPECULAREXPONENT);
	if ( temp == "speedOfLight")      return (SPEEDOFLIGHT);
	if ( temp == "transparency")      return (TRANSPARENCY);
	if ( temp == "reflectivity")      return (REFLECTIVITY);
	if ( temp == "parameters")        return (PARAMETERS);
	if ( temp == "texture")			  return (TEXTURE);
	if ( temp == "globalAmbient")	  return (GLOBALAMBIENT);
	if ( temp == "minReflectivity")	  return (MINREFLECTIVITY);
	if ( temp == "minTransparency")	  return (MINTRANSPARENCY);
	if ( temp == "maxRecursionDepth") return (MAXRECURSIONDEPTH);
	if ( temp == "background")        return (BACKGROUND);
	if ( temp == "backgroundImage")   return (BACKGROUNDIMAGE);
	if ( temp == "{")                 return (LFTCURLY);
	if ( temp == "}")                 return (RGHTCURLY);
	if ( temp == "def")               return (DEF);
	if ( temp == "use")               return (USE);
	if ( temp == " " )                return (T_NULL);
	if ( isidentifier(temp) )         return (IDENT);
	cout << temp << ":" << temp.length() << endl;
	return(UNKNOWN);
} // end of whichtoken

//<<<<<<<<<<<<<<<<<<<<<  drawSceneOpenGL >>>>>>>>>>>>>>>>>>>>>>>.
void Scene :: drawSceneOpenGL()
{ //draw each object on object list
			for(GeomObj* p = obj; p ; p = p->next)
				p->drawOpenGL(); //draw it
}

//<<<<<<<<<<<<<<< Scene :: read >>>>>>>>>>>>>>>>
bool Scene:: read(string fname)// return true if ok; else false
{
	file_in = new ifstream(fname.c_str());

	if(!(*file_in))
	{
		cout << "I can't find or open file: " << fname << endl;
		return false;
	}
	f_in = new strstream();
	line = nextline = 1;
	def_stack = new DefUnitStack();
	char ch;
	freeScene(); //delete any previous scene
	// initialize all for reading:
	obj = tail = NULL;
	light = NULL;
	affStk.tos = new AffineNode;
	affStk.tos->next = NULL;
	// read whole file
	while (file_in->get(ch)) 
	{
		*f_in << ch;
	}
	while(1) //read file, collecting objects, until EOF or an error
	{
		GeomObj * shp = getObject(); // get the next shape
		// no object: either error or EOF
		if(!shp) 
			break; 
		shp->next = NULL; // to be safe
		// empty list so far
		if(obj == NULL)
		{ 
			obj = tail = shp;
		} 
		else { // add new object to queue
			tail->next = shp; 
			tail = shp;
		} 
	}
	file_in->close();
	cleanUp(); // delete temp lists, etc.
	return true;
} // end of read()

//<<<<<<<<<<<<<< Scene :: getObject >>>>>>>>>>>>>>>
GeomObj* Scene :: getObject()
{ //reads tokens from stream f_in (a data member of Scene),
  // building lights, getting materials, doing transformations,
  // until it finds a new object
  // returns NULL if any error occurs, or end of file
	string s;
	string fname;
	GeomObj * newShape;
	mTokenType typ;

	while ((typ = (whichtoken( s = nexttoken() ))) != T_NULL)
	{
		if(typ == UNION || typ == INTERSECTION || typ == DIFFERENCEa)
		{
			switch(typ)
			{
				case UNION:			newShape = new UnionBool();	break;
				case INTERSECTION:	newShape = new IntersectionBool();	break;
				case DIFFERENCEa:	newShape = new DifferenceBool();break;
			} // end of little switch
			GeomObj* p = newShape;
			p = getObject(); // get left child
			if(!p) 
				return NULL; // Error! should always get an object
			((Boolean*)newShape)->left  = p; // hook it up
			p = getObject();// get right child
			if(!p) 
				return NULL;
			((Boolean*)newShape)->right = p; // hook it up
			return newShape;
		}// end of if(typ == UNION etc....
		switch(typ)
		{
			case LIGHT: {
				Point3 p;
				Color3 c;
				p.x = getFloat(); p.y = getFloat();	p.z = getFloat();
				c.red = getFloat(); c.green = getFloat();	c.blue = getFloat();
				Light *l = new Light;
				l->setPosition(p);
				l->setColor(c);
				l->next = light; //put it on the list
				light = l; 
				break;
			}
			case ROTATE: {
				float angle;
				Vector3 u;
				angle = getFloat(); u.x = getFloat();
				u.y = getFloat(); u.z = getFloat();
				affStk.rotate(angle,u);
				break;
			}
			case TRANSLATE: {
				Vector3 d;
				d.x = getFloat(); d.y = getFloat(); d.z = getFloat();
				affStk.translate(d);
				break;
			}
			case SCALE: {
				float sx, sy, sz;
				sx = getFloat(); sy = getFloat(); sz = getFloat();
				affStk.scale(sx, sy, sz);
				break;
			}
			case PUSH: 
				affStk.dup(); 
				break;
			case POP:  
				affStk.popAndDrop(); 
				break;
			case IDENTITYAFFINE: 
				affStk.setIdentity();
				break;
			case AMBIENT: {
				float dr, dg, db;
				dr = getFloat(); dg = getFloat(); db = getFloat();
				currMtrl.ambient.set(dr,dg,db); 
				break;
			}
			case DIFFUSE: {
				float dr,dg,db;
				dr = getFloat(); dg = getFloat(); db = getFloat();
				currMtrl.diffuse.set(dr,dg,db); 
				break;
			}
			case SPECULAR:{
				float dr,dg,db;
				dr = getFloat(); dg = getFloat(); db = getFloat();
				currMtrl.specular.set(dr,dg,db); 
				break;
			}
			case EMISSIVE: {
				float dr,dg,db;
				dr = getFloat(); dg = getFloat(); db = getFloat();
				currMtrl.emissive.set(dr,dg,db); 
				break;
			}
			case PARAMETERS: { // get a list of numParams parameters
				currMtrl.numParams = (int)getFloat();
				for(int i = 0; i < currMtrl.numParams; i++)
					currMtrl.params[i] = getFloat();
				break;
			}
			case SPECULARFRACTION: 
				currMtrl.specularFraction = getFloat(); 
				break;
			case SURFACEROUGHNESS: 
				currMtrl.surfaceRoughness = getFloat(); 
				break;
			case TEXTURE:  // get type, 0 for none
				currMtrl.textureType = (int) getFloat();
				break;
			case DEFAULTMATERIALS: 	
				currMtrl.setDefault();
				break;
			case SPEEDOFLIGHT: 
				currMtrl.speedOfLight = getFloat(); 
				break;
			case SPECULAREXPONENT: 
				currMtrl.specularExponent = getFloat(); 
				break;
			case TRANSPARENCY:
				currMtrl.transparency = getFloat(); 
				break;
			case REFLECTIVITY: 
				currMtrl.reflectivity = getFloat(); 
				break;
			case GLOBALAMBIENT:
				ambient.red = getFloat(); ambient.green = getFloat(); ambient.blue = getFloat(); 
				break;
			case BACKGROUND:
				background.red = getFloat();
				background.green = getFloat();
				background.blue = getFloat();
				break;
			case BACKGROUNDIMAGE:
				fname = nexttoken(); // get file name for mesh
				if(!backPixmap.readBMPFile(fname))
				{   // read BMP file into this pixmap
					cout << " \ncan't read that RGBpixmap file!\n";
					background.red = 1.0;
					background.green = 1.0;
					background.blue = 1.0;
					imageGood = 0;
					break;
				}
				imageGood = 1;
				break;
			case MINREFLECTIVITY: 
				minReflectivity = getFloat(); 
				break;
			case MINTRANSPARENCY:
				minTransparency = getFloat(); 
				break;		
			case MAXRECURSIONDEPTH: 
				maxRecursionDepth = getFloat(); 
				break;
			case MAKEPIXMAP: {	// get BMP file name for a pixmap			
				int which = (int) getFloat();// index of this pixmap in pixmap array
				if(which < 0 || which > 15)	
				{
					cout << "\nbad index of RGBpixmap!\n";
				}
				string fname = nexttoken(); // get file name for mesh
				if(!pixmap[which].readBMPFile(fname))
				{   // read BMP file into this pixmap
					cout << " \ncan't read that RGBpixmap file!\n";
					return NULL;  
				}
				break;
			}// end of case: MAKEPIXMAP
			case T_NULL: 
				break; // The null token represents end-of-file
			case DEF: {
				string name, temp, lb, rb;
				int l = line;
				string inp;
				
				name = nexttoken();
				if ( whichtoken(name) != IDENT ) {
					cout << "Error:  Identifier expected." << endl;
					return NULL;
				}
				if ( def_stack->search(name) ) {
					cout << line << ": " << name;
					cout << ": attempt to redefine. " << endl;
					return NULL;
				}
				lb = nexttoken();
				if ( whichtoken(lb) != LFTCURLY ) {
					cout << "Error: { expected." << endl;
					return NULL;
				}
				while ( whichtoken( temp = nexttoken()) != RGHTCURLY ) {
					cout << temp << endl;
					inp = inp + temp + " ";
					if (!f_in) {
						cout << "Error: end of file detected." << endl;
						return NULL;
					}
				}
				// Push the contents of the string on the stack.
				def_stack->push(name, inp);
				break;
			} // end of case: DEF
			case USE: {
				string name;
				name = nexttoken();
				if ( whichtoken(name) != IDENT ) {
					cout << line << ": " << name;
					cout << ": identifier expected.";
					return NULL;
				}
				if (! def_stack->search(name) ) {
					cout << line << ": " << name;
					cout << ": not defined.";
					return NULL;
				}
				cout << def_stack->contents(name) << endl;
				strstream *temp_fin = new strstream;
				*temp_fin << def_stack->contents(name) << " ";
				*temp_fin << f_in->rdbuf();
				delete (f_in);
				f_in = temp_fin;
				break; 
			} // end of case: USE
			default:  { // inner switch for Shapes
				switch(typ)
				{
					//float param;
					case CUBE:			
						newShape = new Cube;
						break;
					case SPHERE:		
						newShape = new Sphere;
						break;
					case TETRAHEDRON:	
						newShape = new Mesh("tetra.3vn");
						break;
					case TORUS:		
						newShape = new Torus;
						break;
					case PLANE:		
						newShape = new Plane;
						break;
					case SQUARE:		
						newShape = new Square;
						break;
					case TAPEREDCYLINDER:	
						newShape = new TaperedCylinder;
						((TaperedCylinder*)newShape)->smallRadius = getFloat(); 
						break;
					case TAPEREDTUBE:	
						newShape = new TaperedTube;
						((TaperedTube*)newShape)->smallRadius = getFloat();
						((TaperedTube*)newShape)->thickness = getFloat();
						break;
					case CONE:			
						newShape = new TaperedCylinder;
						((TaperedCylinder*)newShape)->smallRadius = 0; 
						break;
					case CYLINDER:		
						newShape = new TaperedCylinder;
						((TaperedCylinder*)newShape)->smallRadius = 1; 
						break;
					case OCTAHEDRON:		
						newShape = new Mesh("octa.3vn");
						break;
					case DODECAHEDRON:	
						newShape = new Mesh("dodeca.3vn"); 
						break;
					case ICOSAHEDRON:	
						newShape = new Mesh("icosa.3vn"); 
						break;
					case BUCKYBALL:		
						newShape = new Mesh("bucky.3vn");	
						break;
					case DIAMOND:		
						newShape = new Mesh("diamond.3vn"); 
						break;
					case WINEGLASS:		
						newShape = new Mesh("wineglass.3vn"); 
						break;
					case TEAPOT:		
						newShape = new Teapot; 
						break;
					case MESH: { // get a filename (with extension) for this mesh
						string fname = nexttoken(); // get file name for mesh
						newShape = new Mesh(fname); 
						break;
					}//	end of case: MESH
					default: {
						cerr << "Line " << nextline << ": unknown keyword " << s << endl;
						return NULL;
					}
				} // end of inner switch
				// common things to do to all Shape’s
				((Shape*)newShape)->mtrl.set(currMtrl);
				// load transform and its inverse
				((Shape*)newShape)->transf.set(*(affStk.tos->affn));
				((Shape*)newShape)->invTransf.set(*(affStk.tos->invAffn));
				return newShape;
			 }// end of default: block
	    } // end of outer switch
	} // end of while
	return NULL;
} // end of getObject

// DefUnitStack methods
DefUnitStack::DefUnitStack() 
{
	stack = NULL;
}

void DefUnitStack :: push(string n, string s) 
{
	D4S *temp_d4s = new D4S;
	temp_d4s->current = new DefUnit(n, s);
	temp_d4s->next = stack;
	stack = temp_d4s;
}

void DefUnitStack :: print() 
{
	D4S *temp = stack;
	string t;
	while (temp) {
		cout << temp->current->name << ":" ;
		cout << temp->current->stuff << endl;
		temp = temp->next;
	}
}

int DefUnitStack :: search(string s) 
{
	D4S *temp = stack;
	while (temp) {
		if ( temp->current->name == s ) {
			return(1);
		}
		temp = temp->next;
	}
	return(0);
}

string DefUnitStack :: contents(string s) 
{
	D4S *temp = stack;
	while (temp) {
		if (temp->current->name == s ) {
			return(temp->current->stuff);
		}
		temp = temp->next;
	}
	return(NULL);
}

void DefUnitStack :: release()
{
	while(stack)
	{
		D4S* tmp = stack; // grab it
		stack = stack->next; // advance p
		delete tmp->current; // release 2 strings
		delete tmp; // release node
	}
	stack = NULL;
}


// Returns the color of a checkerboard at point w
float checkerBoard(Point3 w)
{
	int jx, jy, jz;

	jx = (int) (w.x/.5);
	jy = (int) (w.y/.5);
	jz = (int) (w.z/.5);
	if ((jx + jy + jz) % 2)
		return 0.8f;
	return 0.2f;
}

//double rings(Point3 w)
float rings(Point3 w)
{
	double r = sqrt((w.x * w.x) + (w.y * w.y));
	
	r /= 0.6; 
	r += 3.5 * sin((atan2(w.y, w.x) / .05) + (1.8 * w.z));
	return r;
}

// Returns the color of a wood grain at point w
float woodGrain(Point3 w)
{
	return rings(w);
}

// This function returns the color of a point depending on the textureType of the shape
// textureType = -1 for a checkerboard, -2 for a wood, -3 for marble
float Scene::texture(Shape *obj, Point3 w)
{
	Point3 t;

	xfrmPoint(t, obj->invTransf, w); // get the point in generic coordinates

	if(obj->mtrl.textureType == -1)
		return checkerBoard(t);
	if(obj->mtrl.textureType == -2)
		return woodGrain(t);
	if(obj->mtrl.textureType == -3)
		return loud->marble(5.0, t);
	return 1.0;
}

//<<<<<<<<<<<<<<<<<<<<<<<< shade >>>>>>>>>>>>>>>>>>>>>>>>>>>
Color3 Scene :: shade(Ray& ray) // return the color of this ray
{
	Color3 color, diffColor, specColor; //for ambient and diffuse with texturing
	Intersection inter, bestInter; // construct a new intersection record
	Vector3 s, half, v, normal;

	bestInter.numHits = 0; // no hits at the start
	for(GeomObj* pOb = obj; pOb != NULL; pOb = pOb->next)  // test each object in the scene
	{
		if(!pOb->hit(ray, inter)) // have a hit at a positive hit time?
			continue; // nope: a miss
		if(bestInter.numHits == 0 || inter.hit[0].hitTime < bestInter.hit[0].hitTime)
			bestInter.set(inter);  // copy inter into best
	} // end of replacement for getFirstHit()

	if(bestInter.numHits == 0)
	{
		if((imageGood) &&
		   ((backPixmap.nRows > ray.row) && (backPixmap.nCols > ray.col)))
			return backPixmap.getPixel(ray.col, ray.row);
		return background; // no objects were hit
	}

	HitInfo h;
	h.set(bestInter.hit[0]); //copy first hit record of bestInter
	Point3 wHitPoint(ray.start.x + ray.dir.x * h.hitTime,
					 ray.start.y + ray.dir.y * h.hitTime,
					 ray.start.z + ray.dir.z * h.hitTime);
	v.set(-ray.dir.x, -ray.dir.y, -ray.dir.z);
	v.normalize();
	Shape* myObj = (Shape*)h.hitObject; //point to hit object
	color.set(myObj->mtrl.emissive); //start with emissive part;
//	color.add(myObj->mtrl.ambient); // add ambient	
	xfrmNormal(normal, myObj->invTransf, h.hitNormal);
	normal.normalize();
	for(Light *l = light; l != NULL; l = l->next)
	{
		s.setDiff(l->pos, wHitPoint);
		s.normalize();
		float mDotS = s.dot(normal);
		if(mDotS > 0.0)
		{			
			diffColor.set(mDotS * ((myObj->mtrl).diffuse.red) * ((l->color).red),
						  mDotS * ((myObj->mtrl).diffuse.green) * ((l->color).green),
						  mDotS * ((myObj->mtrl).diffuse.blue) * ((l->color).blue));		
		}			
		color.add(diffColor);
		if(myObj->mtrl.textureType < 0)
		{
			float t = texture(myObj, wHitPoint);
			if(myObj->mtrl.textureType == -2) {
				color.set(0.6f, 0.5f, 0.2f);
				color.add(.4 * t, .12 * t, .06 * t);
			} else {			
				color.red *= t;
				color.green *= t;
				color.red *= t;
			}
		} else if(myObj->mtrl.textureType > 0) {
			color.set(myObj->imageTexture(pixmap[myObj->mtrl.textureType], wHitPoint, h.surface));
		}
		half.set(s.x + v.x,
				 s.y + v.y,
				 s.z + v.z);
		half.normalize();
		float mDotH = half.dot(normal);
		if(mDotH <= 0.0)
			continue;
		float phong = pow(mDotH, (myObj->mtrl).specularExponent);
		specColor.set(phong * ((myObj->mtrl).specular.red) * ((l->color).red),
					  phong * ((myObj->mtrl).specular.green) * ((l->color).green),
					  phong * ((myObj->mtrl).specular.blue) * ((l->color).blue));
		color.add(specColor);
	}
	return color;
} // end of Scene :: shade

// Ray methods
// constructor
Ray::Ray()
{
	start.set(0,0,0); 
	dir.set(0,0,0);
	numInside = 0;
}

Ray::Ray(Point3& origin, Vector3& direction)
{ 
	start.set(origin); 
	dir.set(direction); 
	numInside = 0;
} 

void Ray::setStart(Point3& p)
{
	start.set(p);
}

void Ray::setDir(float x, float y, float z)
{
	dir.x = x; dir.y = y; dir.z = z;
}

// HitInfo methods
HitInfo::HitInfo()
{
	hitObject = NULL; 
	hitTime = -1000; 
	surface = 0; 
	isEntering = 0;
} // constructor

void HitInfo::set(HitInfo& h)
{
	hitTime = h.hitTime;
	hitObject = h.hitObject;
	surface = h.surface;
	isEntering = h.isEntering;
	hitPoint.set(h.hitPoint);
	hitNormal.set(h.hitNormal);
}

// Intersection methods
// default constructor
Intersection::Intersection()
{
	numHits = 0;
}  

void Intersection::set(Intersection& intr)
{ // copy intersection info
	numHits = intr.numHits;
	for(int i = 0; i < maxNumHits; i++)
		hit[i].set(intr.hit[i]);
}
