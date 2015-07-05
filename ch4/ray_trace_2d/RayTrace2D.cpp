#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>

#include "GlutWin.h"
#include "Line2.h"
#include "Vector2.h"

using namespace std;

// globals //////////////////////////////////////////////////////////
Vector2						C, S,		// ray vars p(t) = C + tS
							EndPt;		// for displaying
bool						showNormals;// draw line norms or not
int							numPoints;	// number of points placed by user
vector< vector<Line2> >		polys;		// line lists
vector<Line2>				rays;		// list of rays we already traced
GlutWin *					win;

// loadpolys ////////////////////////////////////////////////////////
bool loadPolys( const char * fileName ) {
	
	/* Expected file format:
	 * 
	 * x0 y0
	 * x1 y1
	 * x2 y2
	 * .
	 * x3 y3
	 * etc...
	 *
	 * where . represents the end of a poly
	 *
	 * NOTE:
	 * all polys must be wound CCW
	 * this is to make normal generation less complicated
	 */

	Vector2			first, curr, last;
	bool				newPoly = true;
	vector<Line2>		v;
	Line2				currSeg;
	int					n = 0;
	char c;
	ifstream			i( fileName );

	if( ! i.is_open() ) 
		return( false );

	while( ! i.eof() ) {

		i >> c;

		if( c == '.' ) {

			// end of a polygon
			newPoly = true;

			// set seg to last point and first one
			currSeg.setVectors( curr, first, LINE_SEGMENT );
			
			v.push_back( currSeg );

			// if less than 2 points in poly then error
			if( n < 1 ) {
				cerr << "Bad Poly\n";
				i.close();
				return( false );
			}

			// put poly onto vector of polys
			polys.push_back( v );
			v.clear();

			// unset c (prevent bug out)
			c = 0;

			continue;

		} else {

			i.putback( c );

			if( newPoly ) {

				// read first point
				i >> curr;
				first = last = curr;
				n = 0;
				newPoly = false;

			} else {
	
				last = curr;
				// read next point
				i >> curr;
				currSeg.setVectors( last, curr, LINE_SEGMENT );
				n++;

				// put on polys line list
				v.push_back( currSeg );
			}
		}	
	}	

	i.close();
	return( true );
}

// LipInterval /////////////////////////////////////////////////////////
bool LipInterval( float & in, float & out, float num, float den ) {

	float hit = num / den;

	if( den < 0 ) {
		if( hit > out ) return( false ); // missed poly
		if( hit > in  ) in = hit;
	}
	else if( den > 0 ) {
		if( hit < in  ) return( false ); // missed poly
		if( hit < out ) out = hit;
	}
	//else if( num <= 0 ) return( false );

	return( true );

}

// timetohitpoly ////////////////////////////////////////////////////
bool timeToHitPoly( int nPolyInd, int & nLineInd, float & hit) {

	Vector2			t, p, n;
	float			num, den;
	float			out = 10000;
	float			in = 0.0f;
	bool			b = false;
	vector<float>	hitTimes;

	for( int c = 0; c < polys[nPolyInd].size(); c++ ) {

		polys[nPolyInd][c].getVectors( p, n, LINE_PTNORM );

		// from equation 4.59
		t = p - S;
		num = n * t;
		den = n * C;

		hitTimes.push_back( num / den );

		//cout << num << " " << den << endl;

		if( ! LipInterval( in, out, num, den ) )
			// ray misses poly entirely
			return( false );

		if( num / den < 0 ) // ray goes backwards to hit line
			continue;

		b = true;
				
	}

	if( nPolyInd == 0 ) {

		// poly 0 is "enclosing" the others
		// so ray would be exiting it
		if( out < 10000.0f ) {

			hit = out;

			for( int c = 0; c < hitTimes.size(); c++ ) {
				if( hitTimes[c] == hit ) {
					nLineInd = c;
					break;
				}
			}
		}
	
	} else {

		// other polys would be entered by the ray
		if( in > 0.0f ) {

			hit = in;

			for( int c = 0; c < hitTimes.size(); c++ ) {
				if( hitTimes[c] == hit ) {
					nLineInd = c;
					break;
				}
			}
		}
	}
	
	return( b );
}



// raytrace2d_render ///////////////////////////////////////////////
void raytrace2d_render() {

	glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glColor3f( 1.0f, 0.0f, 0.0f );

	Vector2	a, b, m, n;

	//handleKB( ' ', 0, 0 );

	for( int x = 0; x < polys.size(); x++ ) {

		glBegin( GL_LINES );

			for( int y = 0; y < polys[x].size(); y++ ) {

				polys[x][y].getVectors( a, b, LINE_SEGMENT );
				polys[x][y].getVectors( m, n, LINE_PTNORM );

				glVertex2f( a.x, a.y );
				glVertex2f( b.x, b.y );

				if( showNormals ) {
					glVertex2f( (a + (0.5f * (b - a))).x, (a + (0.5f * (b - a))).y );
					glVertex2f( ((a + (0.5f * (b - a)) + (10 * n))).x, ((a + (0.5f * (b - a)) + (10 * n))).y );
				}
			}

		glEnd();

	}

	glColor3f( 0.0f, 0.0f, 1.0f );

	glBegin( GL_LINES );

	if( numPoints == 0 ) {


			for( int c = 0; c < rays.size(); c++ ) {

					rays[c].getVectors( a, b, LINE_SEGMENT );

					glVertex2f( a.x, a.y );
					glVertex2f( b.x, b.y );
					
			}

	} else {

		glVertex2f( S.x, S.y );
		glVertex2f( EndPt.x, EndPt.y );
					
	}

	glEnd();

	glutSwapBuffers();
	glFlush();

}

// raytrace2d_keyboard //////////////////////////////////////////////
void raytrace2d_keyboard( unsigned char key, int x, int y ) {

	Vector2	a, b, norm;
	Line2		l;
	bool		h;
	float		t, f = 10000;
	int			nPolyHit, nFaceHit, n;

	if( key == ' ' && numPoints != 1 ) {

		for( int x = 0; x < polys.size(); x++ ) {

			h = timeToHitPoly(x, n, t);

			cout << x << ":" << (h ? "true" : "false") << " " << t << endl;

			if( h && t < f ) {
				f = t;
				nPolyHit = x;
				nFaceHit = n;
			}

		}

		cout << "=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-" << endl;

		// calculate current ray as a line segment
		a = S;
		b = S + (f * C);
		l = Line2( a, b, LINE_SEGMENT );
		// throw it in our vector of rays
		rays.push_back( l );

		// now figure out new direction
		S	= b;
		polys[nPolyHit][nFaceHit].getVectors( a, norm, LINE_PTNORM );

		// from equation 4.27 / 2.68
		C = C - ( ( 2.0f * ( C * norm ) ) * norm );


	}

	if( key == 'n' )
		showNormals = !showNormals;

	glutPostRedisplay();

}

// raytrace2d_mouse /////////////////////////////////////////////////
void raytrace2d_mouse( int button, int state, int x, int y ) {

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

		numPoints++;

		if( numPoints == 1 ) {
			// user is placing the starting point for the ray
			S = Vector2( x, (600 - y) );
			rays.clear();
			EndPt = Vector2( x, 600 - y );
		}

		if( numPoints == 2 ) {
			// user is placing point deining rays direction
			C = Vector2( x, (600 - y) ) - S;
			if( C == Vector2( 0, 0 ) )
				C = Vector2( 1, 0 );
			numPoints = 0;
			raytrace2d_keyboard( ' ', 0, 0 ); // cheese tactics
		}


	}

	glutPostRedisplay();

}

// raytrace2d_motion ///////////////////////////////////////////////
void raytrace2d_motion( int x, int y ) {

	if( numPoints == 1 ) {

		EndPt = Vector2( x, 600 - y );

		glutPostRedisplay();

	}

}

// main /////////////////////////////////////////////////////////////
int main( int argc, char **argv ) {

	// initialize GLUT class
	win = new GlutWin( 600, 800,
					   100, 100,
					   GLUT_DOUBLE | GLUT_RGB,
					   "2d ray tracing demo" );

	if( ! loadPolys( "polys.txt" ) )
		cerr << "unable to open file: polys.txt" << endl;

	C = Vector2( 10, 0 );
	S = Vector2( 150, 300 );

	showNormals = false;
	numPoints = 0;
						   
	glutDisplayFunc( raytrace2d_render );
	glutKeyboardFunc( raytrace2d_keyboard );
	glutMouseFunc( raytrace2d_mouse );
	glutPassiveMotionFunc( raytrace2d_motion );
						   
	// start rendering loop
	glutMainLoop();

	delete win;
	
	return( 0 );
}