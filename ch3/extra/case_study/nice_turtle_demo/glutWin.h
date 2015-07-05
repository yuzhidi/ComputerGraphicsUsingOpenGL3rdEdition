#ifndef _GLUTWIN_H_
#define _GLUTWIN_H_

#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>

////////////////////////////////////////////////////////////
//                      glutWin                          //
//                                                        //
// our class for general purpose GLUT initialization      //
////////////////////////////////////////////////////////////
class glutWin {

public:

	// constructor
	glutWin( int nWindowHeight, int nWindowWidth,
			  int nWindowPosX, int nWindowPosY,
			  unsigned int unDisplayMode,
			  const char * szWindowTitle );

	~glutWin() {};


private:
	
	const char *	m_szWindowTitle;
	int				m_nWindowHeight, m_nWindowWidth;
	int				m_nWindowPosX, m_nwindowPosY;
	int				m_nWindowID;
	unsigned int	m_unDisplayMode;
	bool			m_bFullScreen;

};

#endif // _GLUTWIN_H_
