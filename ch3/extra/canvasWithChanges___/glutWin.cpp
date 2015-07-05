#include "glutWin.h"

glutWin::glutWin( int nWindowHeight, int nWindowWidth,
					int nWindowPosX, int nWindowPosY,
					unsigned int unDisplayMode,
					const char * szWindowTitle ) {
	
	// initialize members
	m_szWindowTitle = szWindowTitle;
	m_nWindowHeight	= nWindowHeight;
	m_nWindowWidth	= nWindowWidth;
	m_nWindowPosX	= nWindowPosX;
	m_nwindowPosY	= nWindowPosY;
	m_unDisplayMode = unDisplayMode;
	m_bFullScreen	= false;

	// make some dummy command line for glut
	char	cmd_line[8];
	char *	argv[1];
	argv[0] = cmd_line;
	int		argc = 1;

	// initialize glut
	glutInit( &argc, argv );

	// initialize window
	glutInitWindowSize( nWindowWidth, nWindowHeight );
	glutInitWindowPosition( nWindowPosX, nWindowPosY );
	glutInitDisplayMode( unDisplayMode );

	// create window
	m_nWindowID	= glutCreateWindow( szWindowTitle );

	// set the view frustum
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	gluOrtho2D( 0, nWindowWidth, 0, nWindowHeight );

	glMatrixMode( GL_MODELVIEW );
	// clear rendering surface
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // background is black
	glViewport(0, 0, nWindowWidth, nWindowHeight);

}


