#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>
#include <windows.h>

using namespace std;

struct GLintPoint {
			GLint x, y;
};

void drawDot(GLint x, GLint y)

{

	glBegin (GL_POINTS);
	glVertex2i(x,y);
	glEnd();

}

int	screenWidth = 640, screenHeight = 480;

void myDisplay() {

	glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glColor3f( 1.0f, 1.0f, 1.0f );
}
/*
	if( selected ) {

		glBegin( GL_QUADS );

		glVertex2i( corner[0].x, corner[0].y );
		glVertex2i( corner[0].x, corner[1].y );
		glVertex2i( corner[1].x, corner[1].y );
		glVertex2i( corner[1].x, corner[0].y );

		glEnd();
 
	}

	glutSwapBuffers();
*/	


void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	  GLint x = mouseX;
		GLint y = screenHeight - mouseY; // flip the y value as always
	  switch(theKey)
  {
		  case ‘p’:
		  	drawDot(x, y);	// draw a dot at the mouse position
		  	break;
	  	case GLUT_KEY_LEFT: List[++last].x = x; // add a point
                        List[  last].y = y;
			  break;
    case ‘E’:
			  exit(-1);		//terminate the program		
	  	default: 
			break;		      // do nothing
	}
}



int main( int argc, char ** argv ) {

	glutInit( &argc, argv );

	// initialize window
	glutInitWindowSize( screenWidth, screenHeight );
	glutInitWindowPosition( 0, 0 );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );

	// create window
	glutCreateWindow( "Rubber Rect Demo" );

	// set the projection matrix
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	gluOrtho2D( 0, screenWidth, 0, screenHeight );

	glMatrixMode( GL_MODELVIEW );
	// clear rendering surface
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // background is black
	glViewport(0, 0, screenWidth, screenHeight);

	glutMouseFunc( myMouse );
	glutDisplayFunc( myDisplay );
//	glutPassiveMotionFunc( myPassiveMotion );
	glutMainLoop();

	return( 0 );
}