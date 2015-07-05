#ifndef _Vector2_H
#define _Vector2_H

#include <cmath>
#include <iostream>

using namespace std;

class Vector2 {
public:

	// vars
	float x, y;

	// constructors
	Vector2() {}

	Vector2( float x1, float y1 ) : x(x1), y(y1) {}

	// vector ops
	void normalize() {

		float temp = 1 / length();

		x *= temp;
		y *= temp;

	}

	inline double length() const {

		return( sqrt((x * x) + (y * y)) );

	}

	Vector2 perp() const {

		return( Vector2( y, -x ) );

	}

	//  operators
	Vector2 operator + ( const Vector2 & rhs ) const {

		return( Vector2(x + rhs.x, y + rhs.y) );

	}

	Vector2 operator - ( const Vector2 & rhs ) const {

		return( Vector2( x - rhs.x, y - rhs.y ) );

	}

	Vector2 operator / ( float k ) const {

		return( Vector2(x / k, y / k) );

	}

	float operator * ( const Vector2 & rhs ) const {

		// dot product
		return( (x * rhs.x) + (y * rhs.y) );

	}

	Vector2 operator * ( const float & rhs ) const {

		// scale by scalar
		return( Vector2( x * rhs, y * rhs ) );

	}

	bool operator == ( const Vector2 & rhs ) {

		return( rhs.x == x && rhs.y == y );

	}

	friend Vector2 operator * ( const float & lhs, const Vector2 & rhs ) {

		// scale by scalar
		return( Vector2( rhs.x * lhs, rhs.y * lhs ) );

	}

	float & operator [] ( int n ) {

		// access like an array
		switch( n ) {

		case 0	: return( x );
		case 1	: return( y );
		default	: /* bug out */;

		}
	}

	friend istream & operator >> ( istream & i, Vector2 & v ) {
		
		i >> v.x >> v.y;
		return( i );

	}

	friend ostream & operator << ( ostream & o, Vector2 & v ) {

		o << v.x << " " << v.y;
		return( o );

	}

}; // end class

#endif // _Vector2_H