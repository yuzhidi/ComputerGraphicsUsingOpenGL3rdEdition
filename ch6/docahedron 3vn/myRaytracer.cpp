/* Andrew Hannon   
   ECE 661
   Project 1 */

#include "SDL.h"
#include "Camera.h"
#include "myviewport.h"
#include <math.h>
#include <io.h> // needed to display current directory - DOS

//############################ GLOBALS ########################
Camera cam; // construct it
Scene scn;  // construct it
MyViewport *myView;
int screenWidth = 640, screenHeight = 480;
int pointSet = 0;
int doRayTrace = 0, doSpecRayTrace;
IntRect specialRayCoord;
float lightStrength = 1.0;
float amount = 1.0;
int blockSize = 8;
string fname;

//<<<<<<<<<<<<<<<<<<<<<< getFileName >>>>>>>>>>>>>>>>.
void getFileName(string& fname)
{ // display list of all .dat files in current directory - needs #include <io.h>
	struct _finddata_t dat_file;
	long hFile;
	if( (hFile = _findfirst("*.dat", &dat_file)) == -1L)
		 cout << " No *.dat files in current directory!\n";
	else
	{
			cout << "Available *.dat files\n";
			cout << "FILE NAME\t\tSIZE\n";
			cout << dat_file.name << "\t\t"<< dat_file.size << endl;
			while( _findnext( hFile, &dat_file) == 0)
					cout << dat_file.name << "\t\t" << dat_file.size << endl;
	}
	_findclose(hFile);
	cout << "type file name: (omit the '.dat') \n";
	cin >> fname; // user types his/her choice
	fname += ".dat"; //append suffix
}

//<<<<<<<<<<<<<<<<<<<<<<<< myKeyboard >>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(unsigned char key, int x, int y)
{
	// let user type 2-char string to specify a command
	static int numGot = 0; // # of char's so far typed
	static string s = "";
	
	if(key == 27) exit(-1);
	if(key == '.'|| key == ' '){numGot = 0; return;} // reset the 2-char string
	if(numGot % 2 == 0)	{s = ""; numGot = 0;}
	s += key; numGot++;
	if(numGot == 1) return; // still need another char for command
	//-------------------------------------------
	//now have two letter command; process it
	if(s == "LS"){/* increase light strength */}
	else if(s == "ls"){/* decrease light strength */}
	else if(s == "qu") exit(-1);
	else if(s == "AM") amount *= 2;
	else if(s == "am") amount /= 2;
	else if(s == "sf") cam.slide(0,0, -amount);  // slide camera forward
	else if(s == "sb") cam.slide(0,0,amount);  //slide camera back
	else if(s == "sl") cam.slide(-amount,0,0);  // slide camera to left
	else if(s == "sr") cam.slide( amount, 0,0);  //slide camera right
	else if(s == "su") cam.slide(0, amount,0);  //slide camera up
	else if(s == "sd") cam.slide(0,-amount,0);  //slide camera down
	else if(s == "pu") cam.pitch(-10 * amount);
	else if(s == "pd") cam.pitch( 10 * amount);
	else if(s == "rl") cam.roll( 10 * amount);  //roll CCW
	else if(s == "rr") cam.roll(-10 * amount);  //roll CW
	else if(s == "yl") cam.yaw( -10 * amount);
	else if(s == "yr") cam.yaw(  10 * amount);
	else if(s == "c1") cam.set(20.0,15.0,20.0,0,0,0,0,1,0);
  	else if(s == "c2") cam.set(20,20,0,0,0,0,0,1,0);
	else if(s == "CV"){/* open camera viewangle */}
	else if(s == "cv"){/* close camera viewangle */}
	else if(s == "r1"){ blockSize = 1; doRayTrace = 1;}
	else if(s == "r4"){ blockSize = 4; doRayTrace = 1;}
	else if(s == "r2"){ blockSize = 2; doRayTrace = 1;}
	else if(s == "r8"){ blockSize = 8; doRayTrace = 1;}
	else if(s == "rs") {
		getFileName(fname);
		scn.read(fname);
	}
	else cout << "\nnot a valid command\n";
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
	y = screenHeight - y;

	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) 
			{
				if(pointSet)
				{
					specialRayCoord.set(specialRayCoord.left, specialRayCoord.top, x, y);
					specialRayCoord.fix();
					doSpecRayTrace = 1;
					pointSet = 0;
					glutPostRedisplay();
				} else {
					specialRayCoord.set(x, y, 0, 0);
					pointSet = 1;
				}
			}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<< myReshape >>>>>>>>>>>>>>>>>>>
void myReshape(int width, int height)
{ // adjust the camera aspect ratio to match that of the viewport
	screenHeight = height;
	screenWidth = width;
	myView->setScreen((GLdouble) width, (GLdouble) height);	
	cam.setAspect(((GLdouble) width) / ((GLdouble) height));
	cam.setDisplay(height, width);
	glutPostRedisplay();
}

//<<<<<<<<<<<<<<<<<<<<<<< display >>>>>>>>>>>>>>>>>>>>>>>>>>
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);	
	cam.drawOpenGL(scn);
	if (doSpecRayTrace)
	{
		cam.raytrace(scn, specialRayCoord);
		doSpecRayTrace = 0;		
	} else if(doRayTrace)
	{
		cam.raytrace(scn, blockSize);
		doRayTrace = 0;
	}
} // end of display

//<<<<<<<<<<<<<<<<<<<<<<<<<<< myinit >>>>>>>>>>>>>>>>>>>>>>>>>>>.
void myInit(void)
{
	string fname;

	glColor3f(0.0f,0.0f,0.0f);
	glShadeModel(GL_SMOOTH); // or could be GL_FLAT
	glEnable(GL_NORMALIZE);
	myView = new MyViewport(screenWidth, screenHeight);
	cam.set(20.0f, 15.0f, 20.0f, // initial eye
		    0.0f ,0.0f, 0.0f, // lookAt
			0,1,0); // and up
	cam.setShape(60.0f, screenWidth/(float)screenHeight,
		         1.0, 10000.0); // match it to viewport
    cam.setDisplay(screenHeight, screenWidth); //sets nRows, nCols in camera
	getFileName(fname);
	scn.read(fname);
} // end of myInit


//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Realistic Raytracer");
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	myInit();
	glutMainLoop();
}