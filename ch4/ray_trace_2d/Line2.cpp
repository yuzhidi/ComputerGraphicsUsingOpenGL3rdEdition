#include "Line2.h"

Line2::Line2( const Vector2 & a, const Vector2 & b, LineType t ) {

	setVectors( a, b, t );

}

Line2::Line2( float x1, float y1, float x2, float y2 ) {

	setVectors( Vector2( x1, y1 ),
				Vector2( x2, y2 ),
				LINE_SEGMENT );

}

void Line2::setVectors( const Vector2 & a, const Vector2 & b, LineType t ) {

	switch( t ) {

	case LINE_SEGMENT:

		// init segment vars
		A = a;
		B = b;
		isSeg = true;

		// init param form
		C = (a - b);
		C.normalize();
		S = a;
		// init PN form 
		P = a;
		N = C.perp();
		break;

	case LINE_PTNORM:

		// init PV vars
		P = a;
		N = b;
		N.normalize();
		// init param form
		S = a;
		C = N.perp();
		// segment form makse no sense here
		isSeg = false;
		break;

	case LINE_PARAM:

		// init param vars
		S = a;
		C = b;
		C.normalize();
		// init PV vars
		P = a;
		N = C.perp();
		// segment form makse no sense here
		isSeg = false;
		break;

	default: break;

	}
}

bool Line2::getVectors( Vector2 & a, Vector2 & b, LineType t ) const {

	switch( t ) {

	case LINE_SEGMENT:

		if( isSeg ) {

			a = A;
			b = B;

		} else {

			// user asked for segment endpoints, and this is not a seg
			return( false ); // = error

		}

		break;

	case LINE_PTNORM:

		a = P;
		b = N;
		break;

	case LINE_PARAM:

		a = S;
		b = C;
		break;

	default:

		return( false );

	}

	return( true );

}

