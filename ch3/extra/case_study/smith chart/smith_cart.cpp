#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>

//--------------- setWindow ---------------------
void setWindow(float left, float right, int bottom, int top)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(left, right, bottom, top); 
}


void drawCircle( double cx, double cy, double radius ) {

	double theta = 360.0f / 50.0f; // approxiamte circle as 50-gon
	theta /= ( 180.0f / 3.14159265f ); // deg -> rad

	glBegin( GL_LINE_LOOP );

	for( int c = 0; c < 50; c++ )
		glVertex2f( cx + ( radius * sin( c * theta ) ), cy + ( radius * cos( c * theta ) ) );

	glEnd();

}

// chillest solution EVER
void drawSolidCircle( double cx, double cy, double radius ) {

	double theta = 360.0f / 50.0f; // approxiamte circle as 50-gon
	theta /= ( 180.0f / 3.14159265f ); // deg -> rad

	glBegin( GL_TRIANGLE_FAN );

	glVertex2f( cx, cy );

	for( int c = 0; c <= 50; c++ )
		glVertex2f( cx + ( radius * sin( c * theta ) ), cy + ( radius * cos( c * theta ) ) );

	glEnd();

}

void drawFamily_a() {

	double m = 0.0f;

	for( int c = 0; c <= 14; c++, m += 3.14159265f / 14.0f ) {

		drawCircle(  1 - m, 0, m );

	}

}

void drawFamily_b() {

	double n = 0.0f;

	for( int c = 0; c <= 14; c++, n += 3.14159265f / 14.0f ) {

		drawCircle( 1, n, n );
		drawCircle( 1,-n, n );

	}

}

void render() {

	// enable stencil testing
	glEnable(GL_STENCIL_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	setWindow(-320, 320.0, -240, 240.0);		// set a fixed windo
	glViewport(0, 0, 640, 480); // set the next viewport

	glScalef( 50, 50, 50 );

	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 1);

	drawSolidCircle( 1.0f - 3.14159265f, 0, 3.15f );

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	drawFamily_a();
	drawFamily_b();

	glDisable(GL_STENCIL_TEST);

	glFlush();

}



int main(int argc, char** argv)
{
	glutInit( &argc, argv ); 
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB | GLUT_STENCIL );
	glutInitWindowSize( 640,480 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "This is what the foeld around wires would look like !@# fear" );

	glutDisplayFunc( render );
	glutMainLoop();

	return( 0 );

}