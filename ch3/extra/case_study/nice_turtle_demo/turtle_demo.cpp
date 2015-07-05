#include "Canvas.h"
#include <cmath>

Canvas cvs( 640, 480, "this window is brought to you by rob hall", true );
float x = 0, y = 1, z = 0;

int n = 0;


void lorentz() {

	
	float dt = 0.01f, dt2 = 0.005f;

	float d0[3], d1[3], d2[3], d3[3], xt, yt, zt;
	
	float r = 1, g = 0, b = 0;

	for( int c = 0; c < n; c++ ) {

		d0[0] = 10 * (y - x) * dt2;
		d0[1] = (-x*z + 28*x - y)*dt2;
		d0[2] = (x*y - 8*z/3)*dt2;

		xt = x + d0[0];
		yt = y + d0[1];
		zt = z + d0[2];

		d1[0] = 10 * (yt - xt) * dt2;
		d1[1] = (-xt*zt + 28*xt - yt)*dt2;
		d1[2] = (xt*yt - 8*zt/3)*dt2;

		xt = x + d1[0];
		yt = y + d1[1];
		zt = z + d1[2];

		d2[0] = 10 * (yt - xt) * dt;
		d2[1] = (-xt*zt + 28*xt - yt)*dt;
		d2[2] = (xt*yt - 8*zt/3)*dt;

		xt = x + d2[0];
		yt = y + d2[1];
		zt = z + d2[2];

		d3[0] = 10 * (yt - xt) * dt2;
		d3[1] = (-xt*zt + 28*xt - yt)*dt2;
		d3[2] = (xt*yt - 8*zt/3)*dt2;

		float y0 = y;

		x += ( d0[0] + d1[0] + d2[0] + d3[0] ) / 3.0f;
		y += ( d0[1] + d1[1] + d2[1] + d3[1] ) / 3.0f;
		z += ( d0[2] + d1[2] + d2[2] + d3[2] ) / 3.0f;

		cvs.setColor( r, g, b );
		cvs.lineTo( x, y );

		//r -= ( 1.0 / 8000.0 );
		//g += ( 1.0 / 8000.0 );
		//b = sin( (c * 3.2425f) / 8000.0f ); 

	}

}

void render() {
	
	cvs.setViewPort( 0, 640, 0, 480 );

	cvs.clearScreen();

	cvs.moveTo( 0, 0 );

	lorentz();

	glFlush();
	glutSwapBuffers();

	n++;

	glutPostRedisplay();

}

int main() {

	//cvs = new Canvas( 640, 480, "this window oewns house" );

	cvs.setBackgroundColor( 0, 0, 0 );
	cvs.setColor( 1, 0, 0 );

	cvs.setWindow(-20.0, 20.0, -20.0, 20.0);

	glutDisplayFunc( render );
	glutMainLoop();

	return( 0 );

}