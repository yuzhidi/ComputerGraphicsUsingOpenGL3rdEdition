/*	myviewport.h:

	This is the header for the viewport class. The viewport class basically
	sets up the viewport with the proper aspect ratio. 

	Written 10/23/99 by Andrew Hannon */

class MyViewport
{
	public:
		MyViewport(GLdouble sWidth, GLdouble sHeight);	// Constructor
		
		/*	This fucntion adjusts the viewport given an aspect ratio
			and uses the screen height and width */
		void adjustViewport(GLdouble aspRatio);
		void setScreen(GLdouble sWidth, GLdouble sHeight);
		GLdouble getWidth();
		GLdouble getHeight();

	private:
		GLdouble screenWidth, screenHeight,
				 viewWidth, viewHeight;		

		// This function takes the data and actually changes the viewport
		void setViewport(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
};