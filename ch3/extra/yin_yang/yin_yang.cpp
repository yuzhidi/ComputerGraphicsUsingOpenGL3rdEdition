#include "Canvas.h"
#include <cmath>

Canvas cvs( 640, 480, "this window oewns house" );

void drawArc( Point2 center, float radius, float startAngle, float sweep )
{ 	  
	// startAngle and sweep are in degrees 
	const int n = 30; // number of intermediate segments in arc 
	double angle = startAngle * 3.14159265 / 180.0f; // initial angle in radians
	double angleInc = sweep * 3.14159265 / (180.0f * n); // angle increment
	float cx = center.getX(), cy = center.getY();
	cvs.moveTo(cx + radius * cos(angle), cy + radius * sin(angle));
	for(int k = 0; k <= n; k++, angle += angleInc)
		cvs.lineTo(cx + radius * cos(angle), cy + radius * sin(angle));
}



void render() {
	
	cvs.setWindow(-10.0, 10.0, -10.0, 10.0);
	cvs.setViewPort( 0, 640, 0, 480 );


	cvs.clearScreen();

	glBegin(GL_LINE_STRIP);{

	
glVertex2i(20,10);
glVertex2i(50,10);
glVertex2i(20,80);
glVertex2i(50,80);
glEnd();
glFlush();

	}
/*	float radius = 6.0f;
	Point2 origin = Point2( 0.0f, 0.0f );
	Point2 pt1 = Point2( (radius / 2.0f) * cos( 3.141593 / 4.0f ), 
					     (radius / 2.0f) * sin( 3.141593 / 4.0f ));
	Point2 pt2 = Point2( -(radius / 2.0f) * cos( 3.141593 / 4.0f ), 
					     -(radius / 2.0f) * sin( 3.141593 / 4.0f ));

	// outer circle
	drawArc( origin, radius, 0.0f, 360.0f );
	
	// two inner circles
	drawArc( pt1, 0.5f, 0.0f, 360.0f );
	drawArc( pt2, 0.5f, 0.0f, 360.0f );

	// S thing
	drawArc( pt1, (radius / 2.0f), 45, 180 );
	drawArc( pt2, (radius / 2.0f), 225, 180 );
*/
	

}

int main() {

	//cvs = new Canvas( 640, 480, "this window oewns house" );

	cvs.setBackgroundColor( 0, 0, 0 );
	cvs.setColor( 1, 0, 0 );

	glutDisplayFunc( render );
	glutMainLoop();

	return( 0 );

}