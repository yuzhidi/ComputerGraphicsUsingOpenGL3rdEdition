/*	mywiewport.cpp:

	Implements the MyViewport class.
	
	Written 10/23/99 by Andrew Hannon */

#include <gl/glut.h>
#include "myviewport.h"
#include "sdl.h"

MyViewport::MyViewport(GLdouble sWidth, GLdouble sHeight)
{
	viewWidth = screenWidth = sWidth;
	viewHeight = screenHeight = sHeight;
}

/* This function adjusts the viewport based on the aspect ratio input. 
   It uses screenHeight and screenWidth as the width and height */
void MyViewport::adjustViewport(GLdouble aspRatio)
{
	GLdouble screenAspRat = screenWidth / screenHeight;

	if(aspRatio > screenAspRat)
	{ 
		viewWidth = screenWidth;
		viewHeight = screenWidth / aspRatio;		
	} 
	else if(aspRatio < screenAspRat)
	{
		viewWidth = screenHeight * aspRatio;
		viewHeight = screenHeight;					
	}
	else if(aspRatio == screenAspRat)
	{
		viewWidth = screenWidth;
		viewHeight = screenHeight;							
	}	
	setViewport(0.0, viewWidth, 0.0, viewHeight);		
}

void MyViewport::setScreen(GLdouble sWidth, GLdouble sHeight)
{
	screenWidth = sWidth;
	screenHeight = sHeight;
	adjustViewport(screenWidth / screenHeight);
}

// This function sets the current viewport (it just encapsulates glViewport)
void MyViewport::setViewport(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
	glViewport(left, bottom, right - left, top - bottom);
}

GLdouble MyViewport::getWidth()
{
	return viewWidth;
}

GLdouble MyViewport::getHeight()
{
	return viewHeight;
}