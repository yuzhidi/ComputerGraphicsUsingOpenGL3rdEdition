#include "Canvas.h"
#include <cmath>

Canvas cvs( 640, 480, "asD" );

void drawHexagonPlz() {
	  
	double radius = 1.0f;
	double angleInc =  2.0f * 3.14159265 / 6.0f; // angle increment
	double angle = 0.0f;

	cvs.moveTo(radius * cos(angle), radius * sin(angle));
	for(int k = 0; k <= 6; k++, angle += angleInc)
		cvs.lineTo(radius * cos(angle), radius * sin(angle));


}

void hexSwirl() {

	float scale = 0.01f;
	float theta = -12.1f;

	for( int c = 0; c < 100; c++ ) {

		glPushMatrix();
		glRotatef( theta, 0, 0, 1 );
		glScalef( scale, scale, scale );

		drawHexagonPlz();

		glPopMatrix();

		scale += 0.05f;
		theta += 5.5f;


	}


}

void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	cvs.setWindow(-0.6, 0.6, -0.6, 0.6); // the portion of the swirl to draw
	cvs.setViewPort( 0, 640, 0, 480 );
	for(int i = 0; i < 5; i++)	     // make a pattern of 5 by 4 copies
		for(int j = 0; j < 4; j++)
		{
			int L = 80; // the amount to shift each viewport
			cvs.setViewPort(i * L, L, j * L, L); // the next viewport
			hexSwirl();
		}


	glFlush();

}

int main(int argc, char** argv)
{ 
	
	glutDisplayFunc( render );

	glutMainLoop();

	return( 0 );

}