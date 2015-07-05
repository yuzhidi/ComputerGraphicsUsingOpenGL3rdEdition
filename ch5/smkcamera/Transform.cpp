#include <cmath>

#include "GlutWin.h"
#include "Vector3.h"
#include "Camera.h"

// types //////////////////////////////////////////////////
struct TransformInfo {

	// holds transformation info
	Vector3	vTranslate,
			vRotate,
			vScale;

	// also some RGB color info
	Vector3	vColor;

};

// globals ////////////////////////////////////////////////////
int					n;			// index to "selected" object in scene
TransformInfo		pTransInfo[9];// array to hold info for each object
GLUquadricObj	*	qobj;			// pointer to GLU structure/handle
GlutWin			*	win;
Camera			*	camera;

double t = 0;

// functions //////////////////////////////////////////////////
void glRotatefv( Vector3 r ) {

	glRotatef( r[0], 1, 0, 0 );
	glRotatef( r[1], 0, 1, 0 );
	glRotatef( r[2], 0, 0, 1 );

}

void glTranslatefv( Vector3 t ) {

	glTranslatef( t[0], t[1], t[2] );

}

void glScalefv( Vector3 s ) {

	glScalef( s[0], s[1], s[2] );

}

void glColorfv( Vector3 c ) {

	// RGB style
	glColor3f( c[0], c[1], c[2] );

}

void init() {

	// first entry governs whole scene
	pTransInfo[0].vColor		= Vector3( 1.0f, 0.0f, 0.0f ); // red
	pTransInfo[0].vRotate		= Vector3( 0.0f, 0.0f, 0.0f );	// no rotation
	pTransInfo[0].vScale		= Vector3( 1.0f, 1.0f, 1.0f ); // no scale
	pTransInfo[0].vTranslate	= Vector3( 0.5f, 0.5f, 0.5f ); // center of scene

	// sphere
	pTransInfo[1].vColor		= Vector3( 0.0f, 0.0f, 0.0f ); // black
	pTransInfo[1].vRotate		= Vector3( 0.0f, 0.0f, 0.0f );	// no rotation
	pTransInfo[1].vScale		= Vector3( 1.0f, 1.0f, 1.0f ); // no scale
	pTransInfo[1].vTranslate	= Vector3( 0.5f, 0.5f, -0.5f );// sphere at (1,1,0)

	// cone
	pTransInfo[2].vColor		= Vector3( 0.0f, 0.0f, 0.0f ); // black
	pTransInfo[2].vRotate		= Vector3( 0.0f, 0.0f, 0.0f );	// no rotation
	pTransInfo[2].vScale		= Vector3( 1.0f, 1.0f, 1.0f ); // no scale
	pTransInfo[2].vTranslate	= Vector3( 0.5f, -0.5f, 0.5f );//  cone at (1,0,1)

	// teapot
	pTransInfo[3].vColor		= Vector3( 0.0f, 0.0f, 0.0f ); // black
	pTransInfo[3].vRotate		= Vector3( 0.0f, 0.0f, 0.0f );	// no rotation
	pTransInfo[3].vScale		= Vector3( 1.0f, 1.0f, 1.0f ); // no scale
	pTransInfo[3].vTranslate	= Vector3( 0.5f, 0.5f, 0.5f ); // teapot at (1,1,1)

	// torus
	pTransInfo[4].vColor		= Vector3( 0.0f, 0.0f, 0.0f ); // black
	pTransInfo[4].vRotate		= Vector3( 90.0f, 0.0f, 0.0f );// rotate about x
	pTransInfo[4].vScale		= Vector3( 1.0f, 1.0f, 1.0f ); // no scale
	pTransInfo[4].vTranslate	= Vector3( -0.5f, 0.5f, -0.5f );// torus at (0,1,0)

	// dodecahedron
	pTransInfo[5].vColor		= Vector3( 0.0f, 0.0f, 0.0f ); // black
	pTransInfo[5].vRotate		= Vector3( 0.0f, 0.0f, 0.0f );	// no rotation
	pTransInfo[5].vScale		= Vector3( 0.15f, 0.15f, 0.15f );// make small
	pTransInfo[5].vTranslate	= Vector3( 0.5f, -0.5f, -0.5f );// dodecahedron at (1,0,0)

	// small cube
	pTransInfo[6].vColor		= Vector3( 0.0f, 0.0f, 0.0f ); // black
	pTransInfo[6].vRotate		= Vector3( 0.0f, 0.0f, 0.0f );	// no rotation
	pTransInfo[6].vScale		= Vector3( 1.0f, 1.0f, 1.0f ); // no scale
	pTransInfo[6].vTranslate	= Vector3( -0.5f, 0.5f, 0.5f );// small cube at (0,1,1)

	// cylinder
	pTransInfo[7].vColor		= Vector3( 0.0f, 0.0f, 0.0f ); // black
	pTransInfo[7].vRotate		= Vector3( 0.0f, 0.0f, 0.0f );	// no rotation
	pTransInfo[7].vScale		= Vector3( 1.0f, 1.0f, 1.0f ); // no scale
	pTransInfo[7].vTranslate	= Vector3( -0.5f, -0.5f, 0.5f );// cylinder at (0,0,1)

	// initialize quadric handle
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj,GLU_LINE);

	n = 0;

	// clear rendering surface
	glClearColor(1.0f, 1.0f, 1.0f,0.0f);  // background is white
	glViewport(0, 0, 640, 480);

	// set the view 
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	//glOrtho(-2.0*64/48.0, 2.0*64/48.0, -2.0, 2.0, 0.001, 10);
	gluPerspective( 75, 4.0/3.0, 0.01, 10.0 );

	camera = new Camera( 2, 2, 2, 0, 0, 0, 0, 1, 0 );

}

void doTransform( TransformInfo & t ) {

	glTranslatefv( t.vTranslate );
	glRotatefv( t.vRotate );
	glScalefv( t.vScale );

	// finally set color
	glColorfv( t.vColor );

}

void transform_render() {

	glClear(GL_COLOR_BUFFER_BIT); // clear the screen
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	camera->Update();

	glPushMatrix();

		//transform entire scene
		doTransform( pTransInfo[0] );

		glutWireCube(1.0);

		glPushMatrix();	
			doTransform( pTransInfo[1] );
			glutWireSphere(0.25, 10, 8);
		glPopMatrix();
		
		glPushMatrix();	
			doTransform( pTransInfo[2] );
			glutWireCone(0.2, 0.5, 10, 8);
		glPopMatrix();
		
		glPushMatrix();
			doTransform( pTransInfo[3] );
			glutWireTeapot(0.2);
		glPopMatrix();

		glPushMatrix();
			doTransform( pTransInfo[4] );
			glutWireTorus(0.1, 0.3, 10,10);
		glPopMatrix();

		glPushMatrix();
			doTransform( pTransInfo[5] );
			glutWireDodecahedron();
		glPopMatrix();

		glPushMatrix();
			doTransform( pTransInfo[6] );
			glutWireCube(0.25);
		glPopMatrix();

		glPushMatrix();
			doTransform( pTransInfo[7] );
			gluCylinder(qobj, 0.2, 0.2, 0.4, 8,8);
		glPopMatrix();

	glPopMatrix();

	// refresh image
	glutSwapBuffers();

	glutPostRedisplay();
	
}

void transform_keyboard( unsigned char key, int x, int y ) {

	if( key == 'q' )
		exit( 0 );

	// select object
	if(key == 'a') {
		// reset color to black
		pTransInfo[n].vColor = Vector3( 0.0f, 0.0f, 0.0f );
		n++;	// increment
		n%=8; // wrap around
		// set color to red
		pTransInfo[n].vColor = Vector3( 1.0f, 0.0f, 0.0f );
	}

	// scale object
	if(key == '-') {
		pTransInfo[n].vScale[0] -= 0.1f;
		pTransInfo[n].vScale[1] -= 0.1f;
		pTransInfo[n].vScale[2] -= 0.1f;
	}

	if(key == '+') {
		pTransInfo[n].vScale[0] += 0.1f;
		pTransInfo[n].vScale[1] += 0.1f;
		pTransInfo[n].vScale[2] += 0.1f;
	}

	// translate object
	// along x axis
	if(key == 'w')
		pTransInfo[n].vTranslate[0] += 0.1f;
	if(key == 's')
		pTransInfo[n].vTranslate[0] -= 0.1f;

	// along y axis
	if(key == 'e')
		pTransInfo[n].vTranslate[1] += 0.1f;
	if(key == 'd')
		pTransInfo[n].vTranslate[1] -= 0.1f;

	// along z axis
	if(key == 'r')
		pTransInfo[n].vTranslate[2] += 0.1f;
	if(key == 'f')
		pTransInfo[n].vTranslate[2] -= 0.1f;


	// rotate object
	// about x axis
	if(key == 't')
		pTransInfo[n].vRotate[0] += 1.0f;
	if(key == 'g')
		pTransInfo[n].vRotate[0] -= 1.0f;

	// about y axis
	if(key == 'y')
		pTransInfo[n].vRotate[1] += 1.0f;
	if(key == 'h')
		pTransInfo[n].vRotate[1] -= 1.0f;

	// about z axis
	if(key == 'u')
		pTransInfo[n].vRotate[2] += 1.0f;
	if(key == 'j')
		pTransInfo[n].vRotate[2] -= 1.0f;


	switch( key ) {

	case 'z':
		camera->Move( 0.5f );
		break;
	case 'x':
		camera->Move( -0.5f );
		break;
	case 'c':
		camera->Strafe( -0.1f );
		break;
	case 'v':
		camera->Strafe( 0.1f );
	default:
		break;

	}
	
	glutPostRedisplay();

}

int main( int argc, char **argv ) {

	// initialize GLUT class
	win = new GlutWin( 600, 800,
						100, 100,
						GLUT_DOUBLE | GLUT_RGB,
						"3D Transformations Demo" );

	init();
						   
	glutDisplayFunc( transform_render );
	glutKeyboardFunc( transform_keyboard );
						   
	// start rendering loop
	glutMainLoop();
	delete win;
	delete camera;
	
	return( 0 );
}
