#include <cmath>
#include <vector>
#include <iostream.h>
#include <fstream>

#include "GlutWin.h"
#include "Camera.h"
#include "Vector3.h"

using namespace std;

GlutWin		*win;
Camera		*camera;

// mouse cursor position
float		mouse_x,
			mouse_y;

bool		mouse_updated = false;

int num_steps_u = 30;
int num_steps_v = 15;



Vector3 P0( float u ) {

	// return a point on a helix, unit radius

	return( Vector3( cos(u), sin(u), u ) );


}

Vector3 P1( float u ) {

	// return a point on a helix
	// this helix spirals around the previous one

	return( Vector3( -cos(u), -sin(u), u ) );

}

bool initdemo() {

	// initialize GLUT class
	win = new GlutWin( 600, 800,
						100, 100,
						GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH,
						"Ruled Surface Demo" );


	camera = new Camera();
	camera->set( 5, 5, 5, 0, 5, 5, 0, 1, 0 );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	// enable depth testing
	glEnable( GL_DEPTH_TEST );

	// set up lighting in ogl
	glEnable( GL_LIGHTING );

	float light_amb[] = { 0.5, 0.5, 0.5, 1.0 };
	float light_dif[] = { 0.25, 0.25, 0.25, 1.0 };
	float light_pos[] = { 10.0, 0.0, 0.0, 1.0 };

	float material_amb[] = { 1.0, 1.0, 1.0, 1.0 };
	float material_dif[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, material_amb );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif );

	glLightfv( GL_LIGHT0, GL_AMBIENT, light_amb );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_dif );

	glLightfv( GL_LIGHT0, GL_POSITION, light_pos );

	glEnable( GL_LIGHT0 );

	return( true );

}

void render() {

	const float PI = 3.14159265359;

	float step_u = (2*PI)/(float)num_steps_u;
	float step_v = (2*PI)/(float)num_steps_v;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	
	glBegin( GL_QUADS );
	
	for( float u = 0; u < 7.0; u += 0.1 ) {

		Vector3 p[4];

		p[0] = P0(u);
		p[1] = P1(u);
		p[2] = P1(u+0.1);
		p[3] = P0(u+0.1);

		// calc normal
		Vector3 t = p[1] - p[0];
		Vector3 b = p[2] - p[1];

		Vector3 n = t.cross( b );
		n.normalize();

		glNormal3f( n.x, n.y, n.z );

		for( int c = 0; c < 4; c++ )
			glVertex3f( p[c].x, p[c].y, p[c].z );

	}

	glEnd();


	// refresh image
	glutSwapBuffers();
	glutPostRedisplay();


}


void kb_input( unsigned char key, int x, int y ) {


	switch( key ) {

	case 'a':	camera->slide( -1.0, 0.0, 0.0 ); break;
	case 'd':	camera->slide( 1.0, 0.0, 0.0 ); break;
	case 's':	camera->slide( 0.0, 0.0, 1.0 ); break;
	case 'w':	camera->slide( 0.0, 0.0, -1.0 ); break;
	case 'q':	exit( 0 );
	default:	break;

	}

	glutPostRedisplay();
}

void mouse_motion( int x, int y ) {

	float dx, dy;

	if( ! mouse_updated) {

		mouse_x = x;
		mouse_y = y;

		mouse_updated = true;

	} else {

		// calc delta
		dx = x - mouse_x;
		dy = y - mouse_y;

		mouse_x = x;
		mouse_y = y;

		// determine rotation amount

		camera->pitch( dy / 5.0 );
		camera->yaw( dx );


	}

}

int main() {

	// init
	if( initdemo() ) {

		glutDisplayFunc( render );
		glutKeyboardFunc( kb_input );
		glutPassiveMotionFunc( mouse_motion );

		glutMainLoop();

		delete win;
		delete camera;

	}

}











	