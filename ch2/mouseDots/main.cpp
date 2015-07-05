#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>

int x;
int y;
int screenheight = 600;
int screenwidth = 800;

void myMouse(int button, int state, int x, int y) {

	if(state == GLUT_DOWN) 
{
	if(button == GLUT_LEFT_BUTTON) 
	{
		//glClearColor(0.3f, 1.0f, 0.7f, 0.0f); //teal
		//glClear(GL_COLOR_BUFFER_BIT);
		glPointSize(5.0);
		glBegin(GL_POINTS);{
			//glVertex2i(screenwidth - x, y);
			//glVertex2i(x, y);
			glVertex2i(x, screenheight - y);
			}
		glEnd();
		glFlush();
	}
else if (button == GLUT_RIGHT_BUTTON) 
		{
			glClearColor(1.0f, 0.0f, 0.0f, 0.0f); // Red
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
		}
}

}


void myDisplay()
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f); //green
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}


int main( int argc, char ** argv ) {

	glutInit( &argc, argv );
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( 800, 600 );
	
	//glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );

	// create window
	glutCreateWindow( "Draw Dots" );

	// set the view frustum
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	gluOrtho2D( 0, 800, 0, 600 );

	//glMatrixMode( GL_MODELVIEW );
	// clear rendering surface
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // background is black
	glViewport(0, 0, 800, 600);

	glutMouseFunc( myMouse );
	glutDisplayFunc( myDisplay );
	glutMainLoop();

	return( 0 );
}