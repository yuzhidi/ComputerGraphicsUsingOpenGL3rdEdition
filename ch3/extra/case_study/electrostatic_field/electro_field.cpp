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


void drawCircle( int cx, int cy, double radius ) {

	double theta = 360.0f / 50.0f; // approxiamte circle as 50-gon
	theta /= ( 180.0f / 3.14159265f ); // deg -> rad

	glBegin( GL_LINE_LOOP );

	for( int c = 0; c < 50; c++ )
		glVertex2i( cx + ( radius * sin( c * theta ) ), cy + ( radius * cos( c * theta ) ) );

	glEnd();

}

void draw2pointers() {

	double a = 20.0f;
	double m = 1.0f;

	for( int c = 0; c < 4; c++, m += 1.0f ) {

		drawCircle( 0, a * sqrt(m*m - 1), a * m );
		drawCircle( 0, a * -sqrt( m*m - 1), a * m );

	}

}

void drawSurrounders() {

	double a = 20.0f;
	double n = 1.0f;

	for( int c = 0; c < 4; c++, n += 1.0f ) {

		drawCircle( a * n, 0, a * sqrt( n*n - 1 ) );
		drawCircle( -a * n, 0, a * sqrt( n*n - 1 ) );

	}

}

void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	setWindow(-320, 320.0, -240, 240.0);		// set a fixed windo
	glViewport(0, 0, 640, 480); // set the next viewport

	draw2pointers();
	drawSurrounders();

	glFlush();

}



int main(int argc, char** argv)
{
	glutInit( &argc, argv ); 
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize( 640,480 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "This is what the foeld around wires would look like !@# fear" );

	glutDisplayFunc( render );
	glutMainLoop();

	return( 0 );

}