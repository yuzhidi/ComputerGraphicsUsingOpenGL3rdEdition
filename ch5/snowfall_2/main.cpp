#include "GlutWin.h"
#include "Camera.h"
#include "Vector3.h"
#include "RGBpixmap.h"

#define GL_GENERATE_MIPMAP 0x8191

// particle structure ////////////////////////////////
struct Particle_t {

	Vector3		pos,
				vel,
				accel,
				color;

	// everything else is implicit in this cheesy system

};

const int PARTICLE_CAP = 10000;


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

RGBpixmap p;

unsigned int	sky_id,
				ground_id,
				flake_id;



float randomInRange( float min, float max ) {

	return( (float)rand() / (float)RAND_MAX * (max - min) + min );

}

void spawnParticle( int index ) {
	
	// spawns a particle somewhere in the plane y = 10.0, 
	// and in the bounds 50<z<50 and 50<x<50

	// create particle
	particles[ index ].pos	= Vector3( randomInRange( -10, 10 ), 10, randomInRange( -10, 10 ) );
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

void pass_pixmap_to_gl( RGBpixmap & p, unsigned int * id, bool alpha = false ) {

	int num_channels = alpha ? 4 : 3;
	int size = num_channels * p.nCols * p.nRows;

	unsigned char * buffer = new unsigned char[size];

	for( int x = 0; x < p.nCols; x++ ) {

		for( int y = 0; y < p.nRows; y++ ) {

			mRGB pix = p.getPixel(x, y);

			buffer[ (x + ( y * p.nCols ) ) * num_channels + 0 ] = pix.r;
			buffer[ (x + ( y * p.nCols ) ) * num_channels + 1 ] = pix.g;
			buffer[ (x + ( y * p.nCols ) ) * num_channels + 2 ] = pix.b;

			if( alpha )
				buffer[ (x + ( y * p.nCols ) ) * num_channels + 3 ] = (pix.r + pix.b + pix.g) == 0 ? 0 : 255;


		}

	}

	glGenTextures( 1, id );

	glBindTexture( GL_TEXTURE_2D, *id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glTexImage2D( GL_TEXTURE_2D, 0,
				  alpha ? GL_RGBA : GL_RGB, 
				  p.nRows, p.nRows, 0,
				  alpha ? GL_RGBA : GL_RGB, 
				  GL_UNSIGNED_BYTE, buffer );

	delete [] buffer;



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

	p.readBMPFile( "grass.bmp" );
	pass_pixmap_to_gl( p, &ground_id );
	p.freeIt();

	p.readBMPFile( "sky.bmp" );
	pass_pixmap_to_gl( p, &sky_id );
	p.freeIt();

	p.readBMPFile( "snowflake.bmp" );
	pass_pixmap_to_gl( p, &flake_id, true );
	p.freeIt();



	return( true );

}

void render() {

	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen
	//glMatrixMode( GL_MODELVIEW );
	//glLoadIdentity();

	//camera->setModelViewMatrix();
	update();

	// get data for billboards
	float mat[16];

	glGetFloatv( GL_MODELVIEW_MATRIX, mat );

	Vector3 right	= Vector3( mat[0], mat[4], mat[8] );
	Vector3 up		= Vector3( mat[1], mat[5], mat[9] );

	glPointSize( 3.0 );

	glEnable( GL_TEXTURE_2D );

	// render floor and ceiling

	glBindTexture( GL_TEXTURE_2D, ground_id );

	glBegin( GL_QUADS );

		glTexCoord2f( 10.0, 0.0 ); glVertex3f( -50, 0, -50 );
		glTexCoord2f( 10.0, 10.0 ); glVertex3f( 50, 0, -50 );
		glTexCoord2f( 0.0, 10.0 ); glVertex3f( 50, 0, 50 );
		glTexCoord2f( 0.0, 0.0 ); glVertex3f( -50, 0, 50 );

	glEnd();

	glBindTexture( GL_TEXTURE_2D, sky_id );

	glBegin( GL_QUADS );

		glTexCoord2f( 1.0, 0.0 ); glVertex3f( -50, 10, -50 );
		glTexCoord2f( 1.0, 1.0 ); glVertex3f( 50, 10, -50 );
		glTexCoord2f( 0.0, 1.0 ); glVertex3f( 50, 10, 50 );
		glTexCoord2f( 0.0, 0.0 ); glVertex3f( -50, 10, 50 );

	glEnd();

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	glBindTexture( GL_TEXTURE_2D, flake_id );

	glBegin( GL_QUADS );

	for( int c = 0; c < PARTICLE_CAP; c++ ) {

		Vector3 pos0	= particles[c].pos + ( up - right ) * 0.1;
		Vector3 pos1	= particles[c].pos + ( right + up ) * -0.1;
		Vector3 pos2	= particles[c].pos + ( right - up ) * 0.1;
		Vector3 pos3	= particles[c].pos + ( right + up ) * 0.1;

		glTexCoord2f( 0.0, 0.0 ); glVertex3f( pos0.x, pos0.y, pos0.z );
		glTexCoord2f( 1.0, 0.0 ); glVertex3f( pos1.x, pos1.y, pos1.z );
		glTexCoord2f( 1.0, 1.0 ); glVertex3f( pos2.x, pos2.y, pos2.z );
		glTexCoord2f( 0.0, 1.0 ); glVertex3f( pos3.x, pos3.y, pos3.z );		

	}

	glEnd();

	glDisable( GL_BLEND );


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

	return( 0 );
}











	