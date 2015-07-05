#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <fstream>
#include <vector>

struct dino_vert {

	GLint x, y;

};

struct dino_poly {

	int						num_verts;
	std::vector<dino_vert>	verts;

};

std::vector<dino_poly>		dinoPolys;
dino_vert					center = {100, 100};
int							dir = 1;
int dino_length;

bool loadPolyLineFile( const char * fileName ) {

	std::ifstream inStream;
	inStream.open( fileName );	// open the file
	if( inStream.fail() )
		return( false );

	GLint		numpolys, numLines, x ,y, min, max;
	dino_vert	curr_vert;
	dino_poly	curr_poly;

	inStream >> numpolys;		           // read the number of polylines
	for(int j = 0; j < numpolys; j++)  // read each polyline
	{

		curr_poly.verts.clear();

		inStream >> numLines;

		for (int i = 0; i < numLines; i++)
		{
			inStream >> curr_vert.x >> curr_vert.y;        // read the next x, y pair

			if( i == 0 && j == 0 ) {

				min = curr_vert.x;
				max = curr_vert.x;

			}

			if( curr_vert.x > max )
				max = curr_vert.x;

			if( curr_vert.x < min )
				min = curr_vert.x;

			curr_poly.verts.push_back( curr_vert );
		}
		
		curr_poly.num_verts = curr_poly.verts.size();
		dinoPolys.push_back( curr_poly );

	}

	dino_length = max - min;
	dino_length *= 0.66f;

	inStream.close();

	return( true );
}

void renderPolyLine() {

	glPushMatrix();

	glRotatef( ( dir ? 0 : 1 ) * 180, 0, -1, 0 );
	glTranslatef( -( dino_length / 2 ), 0 , 0 );
	
	for( std::vector<dino_poly>::iterator i = dinoPolys.begin();
	i != dinoPolys.end();
	++i ) {
		
		glBegin( GL_LINE_STRIP );
		
		for( std::vector<dino_vert>::iterator j = (*i).verts.begin();
		j != (*i).verts.end();
		++j ) {
			
			glVertex2i( (*j).x, (*j).y );
			
			
		}
		
		
		glEnd();
	}
	
	
	glPopMatrix();
}

//--------------- setWindow ---------------------
void setWindow(float left, float right, int bottom, int top)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(left, right, bottom, top); 
}
//---------------- setViewport ------------------
void setViewport(int left, int bottom, int width, int height)
{
glViewport(left, bottom, width, height);
}


void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	setWindow(0, 640.0, 0, 480.0);		// set a fixed windo
	glViewport(0, 0, 640, 480); // set the next viewport

	glTranslatef( center.x, center.y, 0 );
	glScalef( 0.1, 0.1, 0.1 );
	renderPolyLine();		// draw it again

	glFlush();
	glutSwapBuffers();

	glutPostRedisplay();

}

void handleKB( unsigned char key, int x, int y ) {

	switch( key ) {

	case 'q':
		exit( 0 );

	case 'w':
		center.y += 5;
		break;
	case 's':
		center.y -= 5;
		break;
	case 'a':
		center.x -= 5;
		dir = 1;
		break;
	case 'd':
		center.x += 5;
		dir = 0;
		break;
	default:
		break;

	}

}

int main(int argc, char** argv)
{
	glutInit( &argc, argv ); 
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( 640,480 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "Pseudo-elite Dino Anim case Study o_O" );

	loadPolyLineFile( "dino.dat" );
	
	glutDisplayFunc( render );
	glutKeyboardFunc( handleKB );
	glutMainLoop();

	return( 0 );

}