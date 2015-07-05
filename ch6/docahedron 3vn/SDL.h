// SDL.h  - adjusted 1/30/2000 for ECE661 to admit simple raytracing
// definition of simple support classes
// students must implement the hit(), and drawOpenGL() methods of some shapes

#ifndef _SDLStuff
#define _SDLStuff
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

#include <windows.h>
#include <assert.h>
#include <math.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>

#include "RGBpixmap.h" 

#define PERM(x) index[(x) & 255]
#define INDEX(ix, iy, iz) PERM( (ix) + PERM((iy) + PERM(iz)) )
#define lerp(f, A, B) A + f * (B - A) // linearly interpolated lattice noise

class Ray; // declare these classes' existence
class Intersection;

//@@@@@@@@@@@@@@@@@@ Point3 class @@@@@@@@@@@@@@@@

class Point3{
public:
	float x,y,z;

	void set(float dx, float dy, float dz);
	void set(Point3& p);
	Point3(float xx, float yy, float zz);
	Point3();
	void build4tuple(float v[]);
};

//@@@@@@@@@@@@@@@@@@ Vector3 class @@@@@@@@@@@@@@@@
class Vector3{
public:
	float x,y,z;

	void set(float dx, float dy, float dz);
	void set(Vector3& v);
	void flip();
	void setDiff(Point3& a, Point3& b); //set to difference a - b
	void normalize(); //adjust this vector to unit length
	Vector3(float xx, float yy, float zz);
	Vector3(Vector3& v);
	Vector3();
	Vector3 cross(Vector3 b); //return this cross b
	float dot(Vector3 b); // return this dotted with b
};

//@@@@@@@@@@@@@@@@@@@@ light class @@@@@@@@@@@@@@@@@@@

class Light{ // for a linked list of light sources’ color and position
public:
	Point3 pos;
	Color3 color;
	Light* next;

	void setPosition(Point3 p);
	void setColor(Color3 c);
	Light();
};

// @@@@@@@@@@@@@@@@@@@@@ Affine4 class @@@@@@@@@@@@@@@@

class Affine4 {// manages homogeneous affine transformations
// including inverse transformations
// and a stack to put them on
// used by Scene class to read SDL files
public:
	float m[16]; // hold a 4 by 4 matrix

	Affine4();
	void setIdentityMatrix();
	void set(Affine4 a);
	void preMult(Affine4 n);
	void postMult(Affine4 n);
}; // end of Affine4 class

//@@@@@@@@@@ AffineNode class @@@@@@@@@@@
class AffineNode{
// used by Scene class to read SDL files
public:
	Affine4 * affn;
	Affine4 * invAffn;
	AffineNode * next;

	AffineNode();
	~AffineNode(); //destructor
};

//@@@@@@@@@@@@@@@@ AffineStack class @@@@@@@@@@@@
class AffineStack{
// used by Scene class to read SDL files
public:
	AffineNode * tos;

	AffineStack(); //default constructor;puts identity on top
	void dup();
	void setIdentity(); // make top item the identity matrix
	void popAndDrop();
	void releaseAffines(); // pop and drop all remaining items
	void rotate(float angle, Vector3 u);
	void scale(float sx, float sy, float sz);
	void translate(Vector3 d);
}; // end of AffineStack class

//@@@@@@@@@@@@@@@@@ Material class @@@@@@@@@@@@@@
class Material{
public:
	Color3 ambient, diffuse, specular, emissive;
	int numParams; // for textures
	float params[10]; // for textures
	int textureType;  // 0 for none, neg for solids, pos for images
	float specularExponent, reflectivity, transparency, speedOfLight;
	float specularFraction, surfaceRoughness;

	void setDefault();
	void set(Material& m);
}; // end of Material


//@@@@@@@@@@@@@@@@@@@@@ GeomObj class @@@@@@@@@@@@@@@@
class GeomObj{
public:
	//IntRect scrnExtnt;
	GeomObj * next;

	GeomObj();
	virtual bool hit(Ray &r, Intersection &inter);
	virtual void loadStuff();
	virtual void drawOpenGL();
	virtual void tellMaterialsGL();
};

//@@@@@@@@@@@@@@@@@@@ Boolean @@@@@@@@@@@@@@@@@@
class Boolean: public GeomObj{
public:
	GeomObj *left, *right;

	Boolean();
	virtual bool hit(Ray &r, Intersection &inter);
	virtual void drawOpenGL();
};

//@@@@@@@@@@@@@@@@@@@@ UnionBool @@@@@@@@@@@@@@@@
class UnionBool : public Boolean{
public:
	UnionBool(); //constructor
	bool hit(Ray &r, Intersection &inter);
};

//@@@@@@@@@@@@@@@@@@@@ IntersectionBool @@@@@@@@@@@@@@@@
class IntersectionBool : public Boolean{
public:
	IntersectionBool();
	bool hit(Ray &r, Intersection &inter);
};

//@@@@@@@@@@@@@@@@@@@@ DifferenceBool @@@@@@@@@@@@@@@@
class DifferenceBool : public Boolean{
public:
	DifferenceBool();
	bool hit(Ray &r, Intersection &inter);
};

//@@@@@@@@@@@@@@@@@ Shape @@@@@@@@@@@@@@@@@@@@@
class Shape: public GeomObj{
public:
	Material mtrl;
	Affine4 transf,invTransf;

	//virtual Color3 texture(HitInfo& h, int whichTexture);
	Shape();
	void setMaterial(Material& mt);
	void tellMaterialsGL();
	virtual void drawOpenGL();
	virtual bool hit(Ray &r, Intersection &inter);
	virtual Color3 imageTexture(RGBpixmap &p, Point3 w, int surf);
}; //end: Shape class

//@$@$@$@$@$@$@$@$@ Cube class $@$@$@$@$@$@$@$@$@$@
class Cube : public Shape{
public:
	Cube();
	void drawOpenGL();
	bool hit(Ray &r, Intersection &inter);
	Color3 imageTexture(RGBpixmap &p, Point3 w, int surf);
};

//@$@$@$@$@$@$@$@$ Sphere class @$@$@$@$@$@$@$@$@$@$@$@
class Sphere : public Shape{
public:
	void drawOpenGL();
	Sphere();
	bool hit(Ray &r, Intersection &inter);	
};

//@$@$@$@$@$@$@$@$@$@ TaperedCylinder class @$@$@$@$@$@$@$@$@$
class TaperedCylinder : public Shape{
public:
	float smallRadius;
	GLUquadricObj * pwall; // OpenGL needs pointers to quadric objects 
	GLUquadricObj * pbase; 
	GLUquadricObj * pcap; 	

	TaperedCylinder();
	void drawOpenGL();
	bool hit(Ray &r, Intersection &inter);
	Color3 imageTexture(RGBpixmap &p, Point3 w, int surf);
};

//@$@$@$@$@$@$@$@$@$@ TaperedTube class @$@$@$@$@$@$@$@$@$
class TaperedTube : public Shape{
public:
	float smallRadius,
		  thickness;
	GLUquadricObj * pwall; // OpenGL needs pointers to quadric objects 
	GLUquadricObj * pbase; 
	GLUquadricObj * pcap; 	

	TaperedTube();
	void drawOpenGL();
	bool hit(Ray &r, Intersection &inter);
};

//@$@$@$@$@$@$@$@$@$@ Square class @$@$@$@$@$@$@$@$@$
class Square : public Shape{
public:
	Square();
	void drawOpenGL();
	bool hit(Ray &r, Intersection &inter);
	Color3 imageTexture(RGBpixmap &p, Point3 w, int surf);
};

//@$@$@$@$@$@$@$@$@$@ Plane class @$@$@$@$@$@$@$@$@$
class Plane : public Shape{
public:
	Plane();
	void drawOpenGL();
	bool hit(Ray &r, Intersection &inter);
};

//################## class VertexID ################
//used to define a Mesh
class VertexID{public: int vertIndex, normIndex;};
//################# class FACE ##############
//used to define a Mesh
class Face{
public:
	int nVerts;
	VertexID * vert; // array of vertex and normal indices

	Face();
	~Face();
};

//@$@$@$@$@$@$@$@$@$@ Mesh class @$@$@$@$@$@$@$@$@$
class Mesh : public Shape{
private:
	int numVerts, numNorms, numFaces;
	Point3 *pt; // array of points
	Vector3 *norm; // array of normals
	Face *face; // array of faces
	int lastVertUsed;
	int lastNormUsed;
	int lastFaceUsed;

public:
	string meshFileName; // holds file name for this Mesh

	void readMesh(string fname);
	void writeMesh(char* fname);
	void printMesh();
	void drawMesh();
	void drawEdges();
	void freeMesh();
	int isEmpty();
	void makeEmpty();
	Mesh();
	virtual void drawOpenGL();
	Mesh(string fname);
	Vector3 newell4(int indx[]);
	bool hit(Ray &r, Intersection &inter);
}; // end of Mesh class

//@$@$@$@$@$@$@$@$@$@ Torus class @$@$@$@$@$@$@$@$@$
class Torus : public Shape{
public:
	void drawOpenGL();
	bool hit(Ray &r, Intersection &inter);

};

//@$@$@$@$@$@$@$@$@$@ Teapot class @$@$@$@$@$@$@$@$@$
class Teapot : public Shape{
public:
	void drawOpenGL();
	bool hit(Ray &r, Intersection &inter);
};

//@@@@@@@@@@@@@@@@@ DefUnit & DefUnitStack classes @@@@@@@@@@@@@@
//used in Scene to read SDL files
class DefUnit {
	// developed by Steve Morin
public:
	string name, stuff;

	DefUnit(string n, string s);
};

class DefUnitStack {
public:
	DefUnitStack();
	void push(string n, string s);
	void print();
	int search(string s);
	string contents(string s);
	void release();

private:
	struct D4S {
		DefUnit *current;
		struct D4S *next;
	} d4s;
	D4S *stack;
}; // end of DefUnitStack class

//+++++++++++++ TokenType +++++++++++++
enum mTokenType {IDENT, LIGHT, ROTATE, TRANSLATE,
		SCALE, PUSH, POP, IDENTITYAFFINE,
		GLOBALAMBIENT, BACKGROUND, BACKGROUNDIMAGE, MINREFLECTIVITY,
		MINTRANSPARENCY, MAXRECURSIONDEPTH, CUBE, SPHERE, TORUS, PLANE,
		SQUARE, CYLINDER, CONE, TAPEREDCYLINDER,TETRAHEDRON, OCTAHEDRON,
		DODECAHEDRON, ICOSAHEDRON,BUCKYBALL, TEAPOT, WINEGLASS,
		DIAMOND,UNION,INTERSECTION, DIFFERENCEa, MAKEPIXMAP,
		MESH, DEFAULTMATERIALS, AMBIENT, DIFFUSE,SPECULAR,
		SPECULARFRACTION, SURFACEROUGHNESS,EMISSIVE, SPECULAREXPONENT,
		SPEEDOFLIGHT, TRANSPARENCY,REFLECTIVITY, PARAMETERS, TEXTURE,
		TAPEREDTUBE, LFTCURLY, RGHTCURLY, DEF, USE, T_NULL, F_EOF, UNKNOWN };

//@@@@@@@@@@@@@ Noise class @@@@@@@@@@@@@@@@@@@@
class Noise{
public:
	Noise();	
	float noise(float x, float y, float z);
	float noise(float scale, Point3& p);
	float turbulence(float s, Point3& p);
	float marble(float x, float y, float z);
	float marble(float strength,Point3& p);

private:
	float* noiseTable; // array of noise values
	unsigned char * index; // pseudo random indices
	float mySpline(float x); // used for marble
	float latticeNoise(int i, int j, int k);
};

//@@@@@@@@@@@@@  Scene class @@@@@@@@@@@@@@@@@@@@
class Scene{
public:
	Light *light;  // attach linked list of lights here
	GeomObj * obj;  // attach the object list here
	Color3 background, ambient;
	int maxRecursionDepth;
	int imageGood;
	//must #include RGBpixmap.h to have following texture fields
	RGBpixmap pixmap[16]; //list of attached pixmaps
	RGBpixmap backPixmap; // Background picture
	float minReflectivity, minTransparency;
	Noise *loud;
	//bool isInShadow(Ray& f); // for ray tracing: implementation left to the reader

	Scene();
	Scene(string fname);
	void freeScene();
	void makeLightsOpenGL();
	void drawSceneOpenGL();
	bool read(string fname);
	GeomObj* getObject();
	float texture(Shape *obj, Point3 w);	
	Color3 shade(Ray& ray);

private:
	// private stuff used only for reading a scene
	int line;
	int nextline;
	ifstream  *file_in;
	strstream *f_in;
	strstream temp_fin;
	DefUnitStack *def_stack;
	GeomObj * tail; // tail of object list
	AffineStack affStk; // affine stack
	Material currMtrl;
	string nexttoken(void);
	float getFloat();
	bool isidentifier(string keyword);
	void cleanUp();
	mTokenType whichtoken(string keyword);
};

class Intersection;

#define MaxNumInside 10

//@#@#@#@#@#@#@#@# Ray class @#@#@#@#@#@#@#@#@#
class Ray{
public:
	Point3 start;
	Vector3 dir;
	int recurseLevel;
	int row, col; // for screen extents
	int numInside; // number of objects on list
	GeomObj* inside[MaxNumInside]; // array of object pointers

	Ray();
	Ray(Point3 origin); //constructor: set start point of ray
	Ray(Point3& origin, Vector3& direction);
	void setStart(Point3& p);
	void setDir(float x, float y, float z);
	void setRayDirection(Light *L); //for shadow feelers
	void setRayDirection(Vector3& dir); //for reflected and transmitted rays
	int isInShadow();
	void getHitParticulars(Intersection& inter,Point3& hitPoint,Vector3& normal);
	void makeGenericRay(GeomObj* p, Ray& gr);
};

//@#@#@#@#@#@#@#@#  HitInfo class @#@#@#@#@#@#@#@#
class HitInfo { // data for each hit with a surface
public:
	double hitTime;    		// the hit time
	GeomObj* hitObject;  	// the object hit
	int surface;			    // which surface is hit?
	int isEntering;			 // is ray entering the object?
	Point3 hitPoint;		   // hit point
	Vector3 hitNormal;		// normal at hit point

	HitInfo();
	void set(HitInfo& h);
};

#define maxNumHits 8

//@#@#@#@#@#@#@#@# Intersection class @#@#@#@#@#@#@#@#
class Intersection{ // hold hit list
public:
	int numHits;									// the number of hits
	HitInfo hit[maxNumHits];			// list of hits;

	Intersection();  // default constructor
	void set(Intersection& intr);
};

void xfrmPoint(Point3& res, Affine4& aff,float x, float y, float z);
Point3 rayPos(Point3& s, Vector3& v, float t);
Point3 rayPos(Ray&r, float t);
void xfrmPoint(Point3& res, Affine4& aff,Point3& pt);
void xfrmVect(Vector3& res, Affine4& aff,Vector3& v);
void xfrmRay(Ray& g, Affine4& aff, Ray& r);
void xfrmNormal(Vector3& res, Affine4& aff,Vector3& v);

// end of SDL.h

#endif