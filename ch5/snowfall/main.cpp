#include <iostream.h>

#include "GlutWin.h"
#include "Camera.h"
#include "Vector3.h"

// particle structure ////////////////////////////////
struct Particle_t {

	Vector3		pos,
				vel,
				accel,
				color;

	// everything else is implicit in this cheesy system

};

const int PARTICLE_CAP = 10000;

// TODO
// MAKE INDEPENDENT OF FRAMERATE
// owned i still have time to play a game of solitaire

// global array of particles
Particle_t		particles[ PARTICLE_CAP ]; // decent particle cap i think
// num,ber of particles alive
int				num_live = 0;

GlutWin			*	win;
Camera			*	camera;

// mouse state info
float mouse_x = 0, 
	  mouse_y = 0;

bool mouse_updated = false;

// timer info
LARGE_INTEGER	clock_freq,
				clock_time;	

float randomInRange( float min, float max ) {

	return( (float)rand() / (float)RAND_MAX * (max - min) + min );

}

void spawnParticle( int index ) {
	
	// spawns a particle somewhere in the plane y = 10.0, 
	// and in the bounds 50<z<50 and 50<x<50

	// create particle
	particles[ index ].pos	= Vector3( randomInRange( -50, 50 ), 10, randomInRange( -50, 50 ) );
	particles[ index ].vel	= Vector3( 0, 0, 0 ); // no V_init
	particles[ index ].accel= Vector3( randomInRange( -0.8, 0.8 ), randomInRange( -2.0, -0.1 ), randomInRange( -0.8, 0.8 ) );
	particles[ index ].color= Vector3( 1.0, 1.0, 1.0 );

}

void initParticles() {

	// spawn all
	for( int c = 0; c < PARTICLE_CAP; c++ )
		spawnParticle( c );

}

void update() {

	LARGE_INTEGER time;

	// find elapsed time
	// because you got to love frame rate independant motion
	QueryPerformanceCounter( &time );

	float elapsed_time = float( time.QuadPart - clock_time.QuadPart ) / clock_freq.QuadPart;

	clock_time = time;

	for( int c = 0; c < PARTICLE_CAP; c++ ) {

		// really cheesy artists impression of physics  
		particles[c].pos   += particles[c].vel * elapsed_time;
		particles[c].vel   += particles[c].accel * elapsed_time;
		particles[c].accel	= Vector3( randomInRange( -0.8, 0.8 ), particles[c].accel.y, randomInRange( -0.8, 0.8 ) );

		if( particles[c].pos.y <= 0.0 )
			// particle died so start a new one
			spawnParticle( c );

	}

}

bool initdemo() {

	// initialize GLUT class
	win = new GlutWin( 600, 800,
						100, 100,
						GLUT_DOUBLE | GLUT_RGB,
						"3D Transformations Demo" );

	// get data for our high res timer
	if( QueryPerformanceFrequency( &clock_freq ) ) {

		QueryPerformanceCounter( &clock_time );

	} else {

		cout << "Unable to initialize high res timer\n";
		return( false );
	}

	// set the view 
	//glMatrixMode( GL_PROJECTION ); 
	//glLoadIdentity();
	//gluPerspective( 75, 4.0/3.0, 0.01, 100.0 );

	camera = new Camera();
	camera->set( 5, 5, 5, 0, 5, 5, 0, 1, 0 );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	return( true );

}

void render() {

	glClear(GL_COLOR_BUFFER_BIT); // clear the screen
	//glMatrixMode( GL_MODELVIEW );
	//glLoadIdentity();

	//camera->setModelViewMatrix();
	update();

	glPointSize( 3.0 );

	// render floor and ceiling
	glBegin( GL_QUADS );

	glColor3f( 0.0, 1.0, 0.0 );

	glVertex3f( -50, 0, -50 );
	glVertex3f( 50, 0, -50 );
	glVertex3f( 50, 0, 50 );
	glVertex3f( -50, 0, 50 );

	glColor3f( 0.5, 0.5, 0.5 );

	glVertex3f( -50, 10, -50 );
	glVertex3f( 50, 10, -50 );
	glVertex3f( 50, 10, 50 );
	glVertex3f( -50, 10, 50 );

	glEnd();

	glColor3f( 1.0, 1.0, 1.0 );

	glBegin( GL_POINTS );

	for( int c = 0; c < PARTICLE_CAP; c++ ) {

		glVertex3f( particles[c].pos.x, 
					particles[c].pos.y,
					particles[c].pos.z );

	}

	glEnd();


	// refresh image
	glutSwapBuffers();
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

void kb_input( unsigned char key, int x, int y ) {


	switch( key ) {

	case 'a':	camera->slide( -1.0, 0.0, 0.0 ); break;
	case 'd':	camera->slide( 1.0, 0.0, 0.0 ); break;
	case 's':	camera->slide( 0.0, 0.0, 1.0 ); break;
	case 'w':	camera->slide( 0.0, 0.0, -1.0 ); break;
	case 'q':	exit( 0 );
	default:	break;

	}
}

int main() {

	// init
	if( initdemo() ) {

		glutDisplayFunc( render );
		glutPassiveMotionFunc( mouse_motion );
		glutKeyboardFunc( kb_input );

		glutMainLoop();

		delete win;
		delete camera;

	}
}











	