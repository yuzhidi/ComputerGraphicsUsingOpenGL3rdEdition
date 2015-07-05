#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

void setWindow(float left, float right, int bottom, int top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top); 
}

void drawQuarterCircle( int cx, int cy, double radius  ) {

	double theta = 90.0f / 40.0f;
	theta /= ( 180.0f / 3.14159265f ); // deg -> rad

	glBegin( GL_LINE_STRIP );

	for( int c = 0; c <= 40; c++ )
		glVertex2i( cx + ( radius * sin( c * theta ) ), cy + ( radius * cos( c * theta ) ) );

	glEnd();

}

// assume that a tile os in the range [(0,0),(1,1)]
void tile1() {

	


}

void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	setWindow(0, 640.0, 0, 480.0);		// set a fixed windo
	glViewport(0, 0, 640, 480); // set the next viewport

	drawQuarterCircle( 0, 0, 100 );

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