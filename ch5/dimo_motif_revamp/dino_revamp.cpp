#include "Canvas.h"
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;

struct point {
	int x,y;
};

typedef vector<point>		polygon_t;
typedef vector<polygon_t>	polyline_t;

Canvas cvs( 640, 480, "Revamped Dinosaur Demo", true );
polyline_t dino_data;
bool rotated_mode = true;

void loadPolyLineFile() {

	ifstream inStream;
	inStream.open( "dino.dat" );	// open the file

	if(inStream.fail())
		return;

	GLint numpolys, numLines;
	point p;
	polygon_t curr_poly;

	inStream >> numpolys;		           // read the number of polylines
	for(int j = 0; j < numpolys; j++)  // read each polyline
	{
		inStream >> numLines;
		curr_poly.clear();
		for (int i = 0; i < numLines; i++)
		{
			inStream >> p.x >> p.y;        // read the next x, y pair
			curr_poly.push_back( p );
		}
		dino_data.push_back( curr_poly );
	}
	glFlush();
	inStream.close();
}

void drawDino() {

	for( int i = 0; i < dino_data.size(); i++ ) {
		// for each polygon
		glBegin( GL_LINE_STRIP );
		for( int j = 0; j < dino_data[i].size(); j++ )
			glVertex2i( dino_data[i][j].x, dino_data[i][j].y );
		glEnd();
	}

}

void render() {
	
	cvs.setViewPort( 0, 640, 0, 480 );

	cvs.clearScreen();

	cvs.translate2D( 320, 240 );
	cvs.scale2D( 0.1, 0.1 );
	
	if( rotated_mode ) {
		for( float theta = 0; theta < 360; theta += 30 ) {
			glPushMatrix();
				cvs.rotate2D( theta );
				cvs.translate2D( 0.0, 800.0 );	
				drawDino();
			glPopMatrix();
		}
	} else {
		for( float theta = 0; theta < 360; theta += 30 ) {
			glPushMatrix();	
				cvs.rotate2D( -theta );
				cvs.translate2D( 0.0, 800.0 );	
				cvs.rotate2D( theta );
				drawDino();
			glPopMatrix();
		}
	}
	glFlush();
	glutSwapBuffers();

}

void KB( unsigned char key, int x, int y ) {

	if( key == 'q' )
		exit(0);
	if( key == ' ' ){
		rotated_mode = !rotated_mode;
		glutPostRedisplay();
	}


}

int main() {

	loadPolyLineFile();

	cvs.setBackgroundColor( 0, 0, 0 );
	cvs.setColor( 1, 0, 0 );

	cvs.setWindow(0.0, 640.0, 0.0, 480.0);

	glutDisplayFunc( render );
	glutKeyboardFunc( KB );
	glutMainLoop();

	return( 0 );

}