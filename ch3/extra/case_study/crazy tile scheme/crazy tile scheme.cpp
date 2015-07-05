#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <cmath>
#include <vector>

const float TILE_SIZE = 20.0f;
const float HALF_SIZE = TILE_SIZE / 2.0f;

std::vector<double>	x_list, y_list;

void setWindow(float left, float right, int bottom, int top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top); 
}

void initQuarterCircle( double radius  ) {

	double theta = 90.0f / 40.0f;
	theta /= ( 180.0f / 3.14159265f ); // deg -> rad

	for( int c = 0; c <= 40; c++ ) {
		x_list.push_back( radius * sin( c * theta ) );
		y_list.push_back( radius * cos( c * theta ) );
	}
}

void drawQuarterCircle() {

	glBegin( GL_LINE_STRIP );

	for( int c = 0; c < 40; c++ )
		glVertex2f( x_list[c%40], y_list[c%40] );

	glEnd();

}

// expects n in {-1,1}
void tile1() {

	drawQuarterCircle();

	glPushMatrix();
	glTranslatef( TILE_SIZE, TILE_SIZE, 0 );
	glScalef( -1, -1, 1 );
	drawQuarterCircle();
	glPopMatrix();


}

void tile2() {

	glPushMatrix();
	glTranslatef( 0, TILE_SIZE, 0 );
	glScalef( 1, -1, 1 );
	drawQuarterCircle();
	glPopMatrix();

	glPushMatrix();
	glTranslatef( TILE_SIZE, 0, 0 );
	glScalef( -1, 1, 1 );
	drawQuarterCircle();
	glPopMatrix();

}

void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	setWindow(0, TILE_SIZE, 0, TILE_SIZE);		// set a fixed windo
	glViewport(0, 0, 640, 480); // set the next viewport

	for(int i = 0; i < 30; i++) {			// for each column
		for(int j = 0; j < 25; j++)			// for each row
		{
			glViewport( i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE ); // set the next viewport
			if( rand() % 2 ) tile1(); else tile2();
		}
	}

	//tile1();


	glFlush();
	glutSwapBuffers();
	//glutPostRedisplay();

}


int main(int argc, char** argv)
{
	glutInit( &argc, argv ); 
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( 30 * TILE_SIZE,  25* TILE_SIZE );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "This is what the foeld around wires would look like !@# fear" );

	initQuarterCircle( HALF_SIZE );

	glutDisplayFunc( render );
	glutMainLoop();

	return( 0 );

}