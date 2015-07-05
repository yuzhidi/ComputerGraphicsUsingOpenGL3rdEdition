#include "Canvas.h"
#include <cmath>

Canvas cvs( 640, 480, "this window oewns house", true );


void drawHexagonPlz() {
	  
	double radius = 1.0f;
	double angleInc =  2.0f * 3.14159265 / 6.0f; // angle increment
	double angle = 0.0f;

	cvs.moveTo(radius * cos(angle), radius * sin(angle));
	for(int k = 0; k <= 6; k++, angle += angleInc)
		cvs.lineTo(radius * cos(angle), radius * sin(angle));


}

void hexSwirl() {

	float scale = 0.1f;
	float theta = 0.1f;

	for( int c = 0; c < 100; c++ ) {

		glPushMatrix();
		glRotatef( theta, 0, 0, 1 );
		glScalef( scale, scale, scale );

		drawHexagonPlz();

		glPopMatrix();

		scale += 0.1f;
		theta += 0.5f;


	}


}

void render() {
	
	cvs.setViewPort( 0, 640, 0, 480 );

	cvs.clearScreen();

	hexSwirl();

	glFlush();
	glutSwapBuffers();

}

void KB( unsigned char key, int x, int y ) {

	static float window_scale = 1.0f;
	
	if( key == 'q' && window_scale >= 0.1f ) {

		window_scale -= 0.1f;


	}

	if( key == 'a' ) {

		window_scale += 0.1f;

	}

	float s = 10.0f * window_scale;

	cvs.setWindow( -s, s, -s, s );
	glutPostRedisplay();


}

int main() {

	//cvs = new Canvas( 640, 480, "this window oewns house" );

	cvs.setBackgroundColor( 0, 0, 0 );
	cvs.setColor( 1, 0, 0 );

	cvs.setWindow(-10.0, 10.0, -10.0, 10.0);

	glutDisplayFunc( render );
	glutKeyboardFunc( KB );
	glutMainLoop();

	return( 0 );

}