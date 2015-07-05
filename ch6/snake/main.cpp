#include <cmath>
#include <iostream.h>

#include "GlutWin.h"
#include "Camera.h"
#include "Vector3.h"

GlutWin		*win;
Camera		*camera;

// mouse cursor position
float		mouse_x,
			mouse_y;

bool		mouse_updated = false;

// describe the section of the curve, and how often to sample
float		t_0 = 0.0;
float		t_end = 0.1;
float		t_incr = 0.5;

// describes shape of polygon extruded along curve
// and which is a common n-gon
int			n_gon = 4;

Vector3 C( float t ) {

	return( Vector3( 3.0 * sin(t), 3.0 * cos(t), 1.0 * t ) );

}

void create_frenet_frame( double t, float mat[] ) {

	Vector3 N, B, T, C2, C1, C0;
	double epsilon = 0.001;

	// approximate deriv and 2nd deriv turned out
	// to be too unstable, therefore we provide 
	// these functions explicitly, derivs done by hand
	C0 = Vector3( 2*sin(t), 2*cos(t), t * 0.5);
	C1 = Vector3( 2*cos(t), -2*sin(t), 0.5 );
	C2 = Vector3( -2*sin(t), -2*cos(t), 0 );

	// calculate vectors that define the frenet frame
	// at this point on the curve
	T = C1;
	T.normalize();

	B = C2;
	B.normalize();

	N = B.cross( T );

	// create affine transform matrix
	// assume mat has 16 elems
	mat[ 0] = N.x; mat[ 4] = B.x; mat[ 8] = T.x; mat[12] = C0.x;
	mat[ 1] = N.y; mat[ 5] = B.y; mat[ 9] = T.y; mat[13] = C0.y;
	mat[ 2] = N.z; mat[ 6] = B.z; mat[10] = T.z; mat[14] = C0.z;
	mat[ 3] = 0.0; mat[ 7] = 0.0; mat[11] = 0.0; mat[15] = 1.00;


}

bool initdemo() {

	// initialize GLUT class
	win = new GlutWin( 600, 800,
						100, 100,
						GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH,
						"Extrusion Demo" );


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
	float light_pos[] = { 0.0, 0.0, 0.0, 1.0 };

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

Vector3 matrixmult( float mat[], Vector3 v ) {

	return( Vector3( mat[ 0] * v.x + mat[ 4] * v.y + mat[ 8] * v.z + mat[12],
					 mat[ 1] * v.x + mat[ 5] * v.y + mat[ 9] * v.z + mat[13],
					 mat[ 2] * v.x + mat[ 6] * v.y + mat[10] * v.z + mat[14] ) );

}

void render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

	float mat1[16], mat2[16];

	glPointSize( 3.0 );

	for( float t = t_0; t <= t_end; t += t_incr ) {

		// calculate frenet frames for this waist and the next one
		create_frenet_frame( t, mat1 );
		create_frenet_frame( t+t_incr,mat2 );

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();

		// calc vertices for this "waist"
		Vector3 waist1[25], waist2[25], norm[25];

		// create polygon
		double anginc = ( 2.0 * 3.14159 / double( n_gon ) );
		double ang = 0.0;

		for( int k = 0; k < n_gon; k++ ) {
		
			Vector3 pos = Vector3( 0.4 * cos( ang ), 0.4 * sin( ang ), 0.0 );

			ang += anginc;

			waist1[k] = matrixmult( mat1, pos );
			waist2[k] = matrixmult( mat2, pos );

		}

		// calc normal vectors per surface
		// based on equation 6.1
		for( int j = 0; j < n_gon; j++ ) {

			Vector3 t1 = ( waist1[j] - waist2[j] );
			Vector3 t2 = ( waist1[j] + waist2[j] ); 

			norm[j].x = t1.y * t2.z;
			norm[j].y = t1.z * t2.x;
			norm[j].z = t1.x * t2.y;

			t1 = ( waist2[j] - waist2[(j+1)%n_gon] );
			t2 = ( waist2[j] + waist2[(j+1)%n_gon] ); 

			norm[j].x += t1.y * t2.z;
			norm[j].y += t1.z * t2.x;
			norm[j].z += t1.x * t2.y;

			t1 = ( waist2[(j+1)%n_gon] - waist1[(j+1)%n_gon] );
			t2 = ( waist2[(j+1)%n_gon] + waist1[(j+1)%n_gon] ); 

			norm[j].x += t1.y * t2.z;
			norm[j].y += t1.z * t2.x;
			norm[j].z += t1.x * t2.y;

			norm[j].normalize();
		}

		// render face
		glBegin( GL_QUADS );

		for( int a = 0; a < n_gon; a++ ) {

			glNormal3f( norm[a].x, norm[a].y, norm[a].z );

			glVertex3f( waist1[a].x, waist1[a].y, waist1[a].z );
			glVertex3f( waist2[a].x, waist2[a].y, waist2[a].z );

			glVertex3f( waist2[(a+1)%n_gon].x, waist2[(a+1)%n_gon].y, waist2[(a+1)%n_gon].z );
			glVertex3f( waist1[(a+1)%n_gon].x, waist1[(a+1)%n_gon].y, waist1[(a+1)%n_gon].z );

		}

		glEnd();

		glPopMatrix();

	}


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
	case '[':	if( t_incr >= 0.01 ) t_incr /= 2.0; break;
	case ']':	if( t_incr < 0.5 )	 t_incr *= 2.0; break;
	case '+':	t_end  += 1.0; break;
	case '-':	t_end  -= 1.0; break;
	case '/':	if( n_gon < 20) n_gon++; break;
	case '*':	if( n_gon > 3 ) n_gon--; break;
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











	