#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <fstream>

void drawPolyLineFile(const char * fileName) {

	std::ifstream inStream;
	inStream.open(fileName);	// open the file
	if(inStream.fail())
		return;
	//glClear(GL_COLOR_BUFFER_BIT);      // clear the screen 
	GLint numpolys, numLines;
	GLfloat x ,y;
	inStream >> numpolys;		           // read the number of polylines
	//for(int j = 0; j < numpolys; j++)  // read each polyline
	//{
		//inStream >> numLines;
		glBegin(GL_LINE_STRIP);	     // draw the next polyline
		for (int i = 0; i < numpolys; i++)
		{
			inStream >> x >> y;        // read the next x, y pair
			glVertex2f(x, y);
		}
		glEnd();
	//}
	glFlush();
	inStream.close();
}

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


void render() {

	glClear(GL_COLOR_BUFFER_BIT);

	
	setWindow(0, 640.0, 0, 480.0);		// set a fixed window
	setViewport( 0, 0, 640, 480);		// set a fixed window


	glScalef( 20, 20, 20 );
	glTranslatef( 10, 4, 0 );
	glRotatef( 90, 0, 0, 1 );


	drawPolyLineFile( "MAHAL.DAT" );

	glScalef( 1, -1, 1 );

	drawPolyLineFile( "MAHAL.DAT" );

	glFlush();

}

int main(int argc, char** argv)
{
	glutInit( &argc, argv ); 
	
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize( 640,480 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "Poly Line Demo o_O" );
	
	glutDisplayFunc( render );;                   
	glutMainLoop();

	return( 0 );

}