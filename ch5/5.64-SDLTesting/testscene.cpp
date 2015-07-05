#include "sdl_arch.h"
#include "camera.h"

#include <conio.h>

Scene scn;		// the scene object
Camera camera;	// the camera object

float screenWidth = 640, screenHeight = 480;

float R = screenWidth / screenHeight;  // Global Aspect Ratio

Mesh * myMesh = new Mesh();

//*****************************************************************************
void myKeyboard( unsigned char key, int x, int y )
{
	switch( key )
	{
		// controls for the camera
		case 'F':	
			camera.slide( 0, 0, .25 );
			break;
		case 'f':
			camera.slide( 0, 0, -0.25 );
			break;
		case 'W':
		case 'w':
			camera.slide( 0, .25, 0 );
			break;
		case 'Z':
			camera.slide( 0, .25, 0 );
			break;
		case 'z':
			camera.slide( 0, -.25, 0 );
			break;
		case 'S':
			camera.slide( -.30, 0, 0);
			break;
		case 's':
			camera.slide( .30, 0, 0 );
			break;
		case 'A': 
		case 'a':
			camera.slide( -.30, 0, 0 );
			break;
		case 'P':
		//case 80:
			camera.pitch( -1 );
			break;
		case 'p':
			camera.pitch( 1 );
			break;
		case 'R':
			camera.roll( -1 );
			break;
		case 'r':
			camera.roll( 1 );
			break;
		case 'Y':
			camera.yaw( -1 );
			break;
		case 'y':
			camera.yaw( 1 );
			break;
		case '1':
			camera.set( 0, 0, 40, 0, 0, 0, 0, 1, 0 );
			break;
		case '2':
			camera.set( -40, 0, 0, 0, 0, 0, 0, 1, 0 );
			break;
		case '3':
			camera.set( -70, 0, 0, 0, 0, 0, 0, 1, 0 );
			break;
		case '4':
			camera.set( 25, -50, -25, 25, 0, -25, 0, 1, 0 );
			break;
		default:
			;

	}

	glutPostRedisplay();  // draw it again

}

//*****************************************************************************
void myDisplay()
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//	myMesh->drawMesh();
//	myMesh->drawOpenGL();
	
	scn.drawSceneOpenGL();

	static float r = 1.0;
	static float g = 0.0;
	static float b = 0.0;
	if( b > 1 )
		b = b -1;

	// call to makeSmoothTube
	//glColor3f(0.0f,1.0f,0.0f);
	glPushMatrix();
	glRotated( 90.0, -1.0, 0.0, 0.0 );
	glTranslatef( 26.0, 27.0, -2.0 );
	glScalef( 0.75, 0.75, 0.75 );

	// NOTE this call seems to cause problems in the release version
	// on certain machines.  If this is the case then the line can
	// be commented out or it can be run in debug mode which for some
	// reason alleviates the problem on all machines tested.
	myMesh->makeSmoothTube();

	myMesh->mtrl.ambient.set( 1-b*b, b, 1.0-b );
	b = b+0.025; 
	
	//myMesh->drawMesh();
	myMesh->drawOpenGL();

	glPopMatrix();

	// begin code for reading in arch data for building extruded arch
	// the data for the arch is read in from the file hlArch.dat
	// the file contains a number of points, and then a
	// set of x y points which will be extruded by the makeExtrudedQuadStrip
	// call

	// This call makes the arch, it makes the arch
	// using the makeExtrudedQuadStrip method with the 
	// point array the size array and the upVector I created above.

	//myMesh->makeExtrudedQuadStrip( ptArr, sizeArr, upVector );

	glFlush();
	glutSwapBuffers();	// added 8/29

}
	
//*****************************************************************************
void setViewport( int left, int right, int bottom, int top )
{
	glViewport( left, bottom, right - left, top - bottom );
}

//*****************************************************************************
void myReshape( GLsizei W, GLsizei H )
{
	if( R > W/H )
		setViewport( 0, W, 0, W/R );
	else
		setViewport( 0, H * R, 0, H );
}


//*****************************************************************************
void main( int argc, char** argv )
{	
	
	// begin code for reading in arch data for building extruded arch
	// the data for the arch is read in from the file hlArch.dat
	// the file contains a number of points, and then a
	// set of x y points which will be extruded by the makeExtrudedQuadStrip
	// call

	/*
	char * filename = "hlArch.dat";
	fstream inStream;

	inStream.open( filename ); //open needs a c-like string
	if(inStream.fail() || inStream.eof()) 
	{
		cout << "can't open file or eof: " << filename << endl; 
		return;
	}
	
	int numPts;				// the number of points at the top of the file
	inStream >> numPts;		// read it in

	Point2 * ptArr = new Point2[numPts];
	int sizeArr = numPts;
	Vector3 upVector = Vector3( 0, 0, 3 );

	//assert(pt != NULL);
	for(int i = 0; i < numPts; i++) 	// read in the vertices
		inStream >> ptArr[i].x >> ptArr[i].y ;

	// end code for building extruded arch

	// moved this up to myDisplay
	// This call makes the arch, it makes the arch
	// using the makeExtrudedQuadStrip method with the 
	// point array the size array and the upVector I created above.

	//myMesh->makeExtrudedQuadStrip( ptArr, sizeArr, upVector );
	*/
	

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );

	glutInitWindowSize( 640, 480 );

	glutInitWindowPosition( 100, 150 );
	glutCreateWindow( "castle" );

	glutKeyboardFunc( myKeyboard );
	glutDisplayFunc( myDisplay );
	glutReshapeFunc( myReshape );

	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_NORMALIZE );

	glViewport( 0, 0, 640, 480 );

	//camera.set( 0, 6, -15, 0, 0, 0, 0, 1, 0 );				// added for camera use
	camera.set( 0, 0, 40, 0, 0, 0, 0, 1, 0 );
	camera.setShape( 35.0f, 64.0f/48.0f, 0.5f, 140.0f );		// added for camera use
						
	// read in the stacked_cubes data file
	
	//scn.read("textcube.dat");
	
	//scn.read("stacked_cubes2.dat");
	//scn.read( "stacked_cubes.dat" ); //doesn't work 

	// read in the myScene1 data file
	scn.read( "HLSCENE1.DAT" );

	glEnable( GL_LIGHTING );

	scn.makeLightsOpenGL();

	double winHt = 1.0;

	myDisplay();

	glutMainLoop();
}





