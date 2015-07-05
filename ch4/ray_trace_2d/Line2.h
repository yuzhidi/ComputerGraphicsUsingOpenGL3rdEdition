#ifndef _LINE2_H_
#define _LINE2_H_

#include "Vector2.h"

#include <iostream>
using namespace std;

enum LineType {

	LINE_SEGMENT, 
	LINE_PTNORM,
	LINE_PARAM

};

class Line2 {

public:

	Line2( const Vector2 & a, const Vector2 & b, LineType t );	// totally utility
	Line2( float x1, float y1, float x2, float y2 );				// nasty line seg
	Line2(){};

	bool getVectors( Vector2 & a, Vector2 & b, LineType t ) const;
	void setVectors( const Vector2 & a, const Vector2 & b, LineType t );

	friend ostream & operator << ( ostream & o, Line2 & l ) {

		o << l.A << " " << l.B;
		return( o );

	}


protected:

	Vector2	C, S,	// param form p(t) = s + tc 
			A, B, // line seg form a -> b
			P, N; // point normal form

	bool	isSeg;	// is a segment or a line?



};


#endif