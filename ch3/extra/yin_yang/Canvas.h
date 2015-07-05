#include "glutWin.h"

////////////////////////////////////////////////////////////
//                       Point2 Class                         
//                                                        
// class to represent a point in a 2D coordinate system   
////////////////////////////////////////////////////////////
class Point2 {
private:

	GLfloat x, y;

public:

	friend class Canvas; // let Canvas access x and y

	// set of constructors
	Point2()                         { x = y = 0.0f; } 
	Point2( GLfloat xi, GLfloat yi ) { x = xi; y = yi; }
	Point2( const Point2 & p )       { x = p.x; y = p.y; }
	~Point2(){}

	// accessors, mutator
	inline GLfloat getX() const { return( x ); }
	inline GLfloat getY() const { return( y ); }

	inline void set( GLfloat xi, GLfloat yi ) { x = xi; y = yi; }

	void draw() {

		glBegin( GL_POINTS );
			glVertex2f( x, y );
		glEnd();
	
	}

};

////////////////////////////////////////////////////////////
//                        Rect Class                           
//                                                        
// templated class to represent a rect in a 2D coordinate 
// system (mainly uses ints and floats as template parameter) 
////////////////////////////////////////////////////////////
template <typename T>
class Rect {
private:

	T	l, r, b, t; // left, right, bottom, top

public:

	Rect() : l(0), r(100), b(0), t(100) {}; // default constructor
	
	Rect( T left, T right, T bottom, T top ) :
	l(left), r(right), b(bottom), t(top) {}  // construction with values

	inline void set( T left, T right, T bottom, T top ) {

		l = left; r = right; b = bottom; t = top;

	}

	void draw() {

		// for simplicity's sake implicitly convert all types into GLfloat
		// (alternative: partial template specialization - not supported on some compilers)
		glBegin( GL_LINE_STRIP );

			glVertex2f( l, b ); // bottom left
			glVertex2f( l, t ); // top left
			glVertex2f( r, t ); // top right
			glVertex2f( r, b ); // bottom right
			glVertex2f( l, b ); // bottom left again

		glEnd();

	}

};

class Canvas {
private:

	Point2			currPos;	// turtle position
	double			currDir;	// turtle angle (CCW from +x)
	Rect<int>			viewPort;	// viewport dimensions
	Rect<float>		window;	// window dimensions
	float			bgColor[3];	// background color triple
	float			color[3];	// foreground color triple


public:

	Canvas( int width, int height, const char * windowTitle );
	~Canvas() {}

	// mutators
	void setWindow( float l, float r, float b, float t );
	void setViewPort( int l, int r, int b, int t );

	// accessors
	Rect<float>	getWindow()   const { return( window ); }
	Rect<int>		getViewPort() const { return( viewPort ); }

	// miscellaneous drawing
	void clearScreen();
	void setBackgroundColor( float r, float g, float b );
	void setColor( float r, float g, float b );
	
	// turtle functions
	void lineTo( float x, float y );
	void lineTo( const Point2 & p );

	void moveTo( float x, float y );
	void moveTo( const Point2 & p );

	// relative turtle functions
	void lineRel( float dx, float dy );
	void moveRel( float dx, float dy );

	// polar turtle functions
	void turnTo( double angle );
	void turn( double theta );
	void forward( float dist, bool visible );


};
