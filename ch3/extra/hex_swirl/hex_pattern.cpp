#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include <iostream>

struct GLintPoint {

	GLint x, y;

};

//--------------- setWindow ---------------------
void setWindow(float left, float right, int bottom, int top)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(left, right, bottom, top); 
}
//---------------- setViewport ------------------
void setViewport(int left, int bottom, int width, int height)
{
glViewport(left, bottom, width, height);
}

void hexSwirl() {

	const double radPerDeg = 0.017453393f;

	double x, y, cx, cy;
	double length = 10.0f;
	double scale = 0.001f;
	double theta = 0.0f;

	for( int i = 0; i < 50; i++ ) {

		x = y = 0.0f;

		x -= 11.18034f * sin( radPerDeg * 30.0f );
		y -= 11.18034f * cos( radPerDeg * 30.0f );

		glPushMatrix();

		//glTranslatef( 300.0f, 200.0f, 0.0f );
		glScalef( scale, scale, scale );
		glRotatef( theta, 0, 0, 1 );

		theta += 1.5f;
		scale += 0.007f;

		glBegin( GL_LINE_LOOP );

		for( int c = 0; c < 6; c++ ) {

				glVertex2f( x, y );

				x += ( length * cos( radPerDeg * ( c * 60.0f ) ) );
				y += ( length * sin( radPerDeg * ( c * 60.0f ) ) );

		}

		glEnd();

		glPopMatrix();

	}

}


void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	setWindow(-0.6, 0.6, -0.6, 0.6); // the portion of the swirl to draw	
	for(int i = 0; i < 5; i++)	     // make a pattern of 5 by 4 copies
		for(int j = 0; j < 4; j++)
		{
			int L = 80; // the amount to shift each viewport
			setViewport(i * L, j * L, L, L); // the next viewport
			hexSwirl();
		}


	glFlush();

}

void mouseHandler( int button, int state, int x, int y ) {

	static GLintPoint corners[2];
	static int	nCorners = 0;

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

		std::cout << "x" << nCorners << ": " << x << " y" << nCorners << ": " << y << "\n";

		corners[nCorners].x = x;
		corners[nCorners++].y = 480 - y;

		if( nCorners == 2 ) {

			nCorners = 0;
			setWindow  ( min( corners[0].x, corners[1].x ),
						 max( corners[0].x, corners[1].x ),
						 min( corners[0].y, corners[1].y ),
						 max( corners[0].y, corners[1].y ) );

			glutPostRedisplay();

		}
	}
}

void KBHandler( unsigned char key, int x, int y ) {


	if( key == 'r' ) {

		// reset
		setWindow(0, 640.0, 0, 480.0);		// set a fixed window
		glutPostRedisplay();

	}


}

int main(int argc, char** argv)
{
	glutInit( &argc, argv ); 
	
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize( 640,480 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "Poly Line Demo o_O" );
	
	glutDisplayFunc( render );
	glutMouseFunc( mouseHandler );
	glutKeyboardFunc( KBHandler );
	
	setViewport( 0, 0, 640, 480 );
	setWindow(0, 640.0, 0, 480.0);		// set a fixed window

	glutMainLoop();

	return( 0 );

}