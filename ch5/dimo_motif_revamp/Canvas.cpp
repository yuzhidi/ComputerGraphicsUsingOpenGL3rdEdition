#include <cmath>
#include "Canvas.h"
#include "glutWin.h"


// Canvas functions
Canvas::Canvas( int width, int height, const char * windowTitle, bool bUseDoubleBuffer ) {
	
	// initialize openGL and everything
	GlutWin initw(	height, width, 20, 20,
					( bUseDoubleBuffer ? GLUT_DOUBLE: GLUT_SINGLE ) | GLUT_RGB,
					windowTitle );
	
	// make sure the frustum is set up properly
	setWindow( 0.0f, (float)width, 0.0f, (float)height );
	setViewPort( 0, width, 0, height );
	
	// set turtle position
	currPos = Point2( 0.0f, 0.0f );
	currDir = 0.0f;

	// set initial colors
	setBackgroundColor( 0.0f, 0.0f, 0.0f );
	setColor( 1.0f, 0.0f, 0.0f );
	
}

void Canvas::setWindow( float l, float r, float b, float t ) {

	window.set( l, r, b, t );

	// modify frustum
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( l, r, b, t );
}

void Canvas::setViewPort( int l, int r, int b, int t ) {

	viewPort.set( l, r, b, t );

	// modify ogl viewport
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glViewport(l, b, r, t);

}

void Canvas::clearScreen() {

	glClearColor( bgColor[0], bgColor[1], bgColor[2], 0.0f );
	glClear( GL_COLOR_BUFFER_BIT ); // <-- ADDED THIS IN IT IS NEEDED~!@#

}

void Canvas::setBackgroundColor( float r, float g, float b ) {

	bgColor[0] = r;
	bgColor[1] = g;
	bgColor[2] = b;

}

void Canvas::setColor( float r, float g, float b ) {

	color[0] = r;
	color[1] = g;
	color[2] = b;

	glColor4f( r, g, b, 1.0f );

}

void Canvas::lineTo( float x, float y ) {

	glBegin( GL_LINES );

		glVertex2f( currPos.x, currPos.y );
		glVertex2f( x, y );

	glEnd();

	moveTo( x, y );

}

void Canvas::lineTo( const Point2 & p ) {

	// simply call other variant
	lineTo( p.getX(), p.getY() );

}

void Canvas::lineRel( float dx, float dy ) {

	lineTo( currPos.x + dx, currPos.y + dy );

}

void Canvas::forward( float dist, bool visible ) {

	const double radPerDeg = 0.017453393f;

	double x = currPos.x + ( dist * cos( currDir * radPerDeg ) );
	double y = currPos.y + ( dist * sin( currDir * radPerDeg ) );

	if( visible )
		lineTo(x, y);
	else
		moveTo(x, y);
	
}


void Canvas::moveTo( float x, float y ) {
	
	currPos.set( x, y );
	
}

void Canvas::moveTo( const Point2 & p ) {
	
	currPos = p;
	
}

// relative turtle functions

void Canvas::moveRel( float dx, float dy ) {
	
	currPos.set( currPos.x + dx, currPos.y + dy );
	
}

// polar turtle functions
void Canvas::turnTo( double angle ) {
	
	currDir = angle;
	
}

void Canvas::turn( double theta ) {
	
	currDir += theta;
	
	if( currDir >=360.0f )
		currDir -= 360.0f;
	
}

void Canvas::initCT() {

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

void Canvas::scale2D( float x, float y ) {
	
	glMatrixMode( GL_MODELVIEW );
	glScalef( x, y, 1.0 );

}

void Canvas::translate2D( float x, float y ) {
	
	glMatrixMode( GL_MODELVIEW );
	glTranslatef( x, y, 0.0 );

}

void Canvas::rotate2D( float theta ) {
	
	glMatrixMode( GL_MODELVIEW );
	glRotatef( theta, 0.0, 0.0, 1.0 );

}
