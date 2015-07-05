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
	GLint numpolys, numLines, x ,y;
	inStream >> numpolys;		           // read the number of polylines
	for(int j = 0; j < numpolys; j++)  // read each polyline
	{
		inStream >> numLines;
		glBegin(GL_LINE_STRIP);	     // draw the next polyline
		for (int i = 0; i < numLines; i++)
		{
			inStream >> x >> y;        // read the next x, y pair
			glVertex2i(x, y);
		}
		glEnd();
	}
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
	
	//setWindow(0, 640.0, 0, 480.0);		// set a fixed window

	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
		{
			if((i + j) % 2 == 0)            	  // if (i + j) is even
				setWindow(0.0, 640.0, 0.0, 480.0); // right side up window
			else
				setWindow(0.0, 640.0, 480.0, 0.0); // upside down window
			glViewport(i * 64, j * 44, 64, 44);   // set the next viewport
			drawPolyLineFile("dino.dat");		  // draw it again setWindow .  
   }


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