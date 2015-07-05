#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <windows.h>

#include "GlutWin.h"
#include "Vector2.h"

using namespace std;

// globals //////////////////////////////////////////////////////////
const double	vector_scale = 10.0;
const double	rads_per_deg = 0.0174532925;
const double	pixels_per_meter = 10;

// coefficients of friction
const double	mu_kinetic	= 0.25;
const double	mu_static	= 0.53;

const double	g			= 9.81; // acceleration due to gravity (m/s^2)

double			theta		= 0;	// current incline of plane (degrees)
double			mass		= 1;	// mass of block (kilograms)

double			box_pos		= 0;	// keep track of position in one dim (along axis of move)
double			box_vel		= 0;	// velocity along axis of motion

bool			friction_overcome = false; // whether to use static or kinetic coef
bool			simulation_over	  = false;

Vector2			force_grav;
Vector2			force_drag;
Vector2			force_norm;

LARGE_INTEGER	clock_freq;
LARGE_INTEGER	last_time;

// draw_string //////////////////////////////////////////////////////
void draw_string( int x, int y, const char * buffer ) {
	// function to draw a string in glut

	glRasterPos2i( x, y );

	// i think its safe to assume a null terminator appears somewhere
	while( *buffer )
		glutBitmapCharacter( (void *)GLUT_BITMAP_TIMES_ROMAN_24, *(buffer++) );


}

// init_simulation //////////////////////////////////////////////////
void init_simulation() {

	theta		= 0;
	mass		= 1;
	box_pos		= 0;
	box_vel		= 0;
	simulation_over = false;
	friction_overcome = false;

}

// fricion_render ///////////////////////////////////////////////////
void friction_render() {

	glClearColor( 0.0, 0.0, 0.0, 0.0 ); // clear to black
	glClear( GL_COLOR_BUFFER_BIT );
	glColor3f( 0.5, 0.5, 0.5 );

	// the c way is somehow easier than the c++ way
	char buffer[80];

	// write theta = val near the angle as it appears on the screen
	sprintf( buffer, "Theta = %.2f deg", theta );
	draw_string( 600, 75, buffer );

	// calc gravity
	force_grav.x = mass * g * sin( theta * rads_per_deg );
	force_grav.y = -mass * g * cos( theta * rads_per_deg );

	// calc normal force
	force_norm.x = 0; // see diagram for reasoning :p
	force_norm.y = -force_grav.y;

	force_drag.x = -force_norm.length() * ( friction_overcome ? mu_kinetic : mu_static );
	force_drag.y = 0;

	sprintf( buffer, "Fg = %.3f N", force_grav.length() );
	draw_string( 0, 540, buffer );
	sprintf( buffer, "Fn = %.3f N", force_norm.length() );
	draw_string( 0, 580, buffer );
	sprintf( buffer, "Fd = %.3f N", force_drag.length() );
	draw_string( 0, 560, buffer );

	LARGE_INTEGER i;

	QueryPerformanceCounter( &i );

	double elapsed_time = double( i.QuadPart - last_time.QuadPart ) / clock_freq.QuadPart;

	last_time = i;

	if( ! simulation_over ) {

		// figure out if it moves
		if( ! friction_overcome ) {

			draw_string( 0, 520, "static friction not overcome" );

			if( force_drag.length() < fabs( force_grav.x ) ) {
				// overcome static friction
				friction_overcome = true;
				// box will start to animate next frame
			}

			sprintf( buffer, "acceleration = 0.000 m/s/s" );
			draw_string( 0, 500, buffer );

		} else {
			// box in motion
			// F = ma -> a = F/m
			double acc = ( force_grav.x - force_drag.length() ) / mass;
			box_vel += ( acc * elapsed_time );

			draw_string( 0, 520, "static friction has been overcome" );

			sprintf( buffer, "acceleration = %.3f m/s/s", acc );
			draw_string( 0, 500, buffer );

			cout << box_pos << endl;

		}

		sprintf( buffer, "velocity = %.3f m/s", box_vel );
		draw_string( 0, 480, buffer );

		box_pos += ( box_vel * pixels_per_meter ) * elapsed_time;			

	} else {

		draw_string( 0, 520, "Simulation ended - space to reset" );

	}

	// see if it ended yet
	if( box_pos >= 600 ) {

		simulation_over = true;

	}


	// draw inclined plane, origin at (700,100)
	double dw = 600;

	// draw "ground"
	glBegin( GL_LINES );

		glVertex2i( 700, 100 );
		glVertex2i( 100, 100 );

	glEnd();

	// this is why hierarchial transformations are my friend
	glPushMatrix();
	
		// go to origin (place where plane meets ground)
		glTranslatef( 700, 100, 0 );
		glRotatef( -theta, 0, 0, 1);

		// draw plane
		glBegin( GL_LINES );

			glVertex2i( 0, 0 );
			glVertex2i( -dw, 0 );

		glEnd();

		// go to raised end of plane, and up (for box)
		glTranslatef( box_pos-dw, 50, 0 );

		glColor3f( 1.0, 0.0, 0.0 );
		// draw mass (in this case a box)
		glBegin( GL_LINE_LOOP );

			glVertex2i( -50, 50 );
			glVertex2i( 50, 50 );
			glVertex2i( 50, -50 );
			glVertex2i( -50, -50 );

		glEnd();

		// draw the vectors in a different color
		glPushMatrix();

			glScalef( vector_scale, vector_scale, 0 );

			glBegin( GL_LINES );

				glColor3f( 0.0, 1.0, 0.0 ); // grav vec in green
				glVertex2f( 0, 0 );
				glVertex2f( force_grav.x, force_grav.y );

				glColor3f( 0.0, 0.0, 1.0 ); // norm vec in blue
				glVertex2f( 0, 0 );
				glVertex2f( force_norm.x, force_norm.y );

				glColor3f( 1.0, 0.0, 1.0 ); // drag vec in purp
				glVertex2f( 0, 0 );
				glVertex2f( force_drag.x, force_drag.y );

			glEnd();

		glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();

}


// friction_keyboard ////////////////////////////////////////////////
void friction_keyboard( unsigned char key, int x, int y ) {

	switch( key ) {

	case 'q':
		exit( 0 ); // exit app

	case '+': // numpad plus
		//if( theta < 40 )
			theta += 1;
		break;

	case '-': // numpad minus
		//if( theta > 1 )
			theta -= 1;
		break;

	case ' ':
		if( simulation_over )
			init_simulation();

	default:
		break;
	}

	glutPostRedisplay();

}

// main /////////////////////////////////////////////////////////////
int main( int argc, char **argv ) {

	// get data for our high res timer
	if( QueryPerformanceFrequency( &clock_freq ) ) {

		QueryPerformanceCounter( &last_time );

		// initialize GLUT class
		GlutWin * win = new GlutWin( 600, 800,
									 0, 0,
									 GLUT_DOUBLE | GLUT_RGB,
									 "friction demo" );

							   
		glutDisplayFunc( friction_render );
		glutKeyboardFunc( friction_keyboard );
							   
		// start rendering loop
		glutMainLoop();

		delete win;

	} else {
	
		cout << "Unable to start high res timer";

	}
	
	return( 0 );
}