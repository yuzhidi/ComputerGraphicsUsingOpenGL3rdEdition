#include "Canvas.h"

Canvas cvs( 640, 480, "this window owns house" );

void arrow(float f, float h, float t, float w)
{ // assumes global Canvas object: cvs
	cvs.lineRel(-w - t / 2, -f); 		// down the left side  
	cvs.lineRel(w, 0);  
	cvs.lineRel(0, -h); 	
	cvs.lineRel(t, 0); 			// across 
	cvs.lineRel(0, h);  			// back up 
	cvs.lineRel(w, 0); 
	cvs.lineRel(-w - t / 2, f); 	
}


void display() {
	
	cvs.setWindow(-10.0, 10.0, -10.0, 10.0);
	cvs.setViewPort( 0, 640, 0, 480 );

	cvs.clearScreen();

	cvs.moveTo(0, 0 );  // draw a line

	arrow( 3, 1, 4, 1 );

	glFlush();

}

int main() {


	cvs.setBackgroundColor( 0, 0, 0 );
	cvs.setColor( 1, 0, 0 );

	glutDisplayFunc( display );
	glutMainLoop();

	return( 0 );

}