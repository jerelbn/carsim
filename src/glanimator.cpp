/*
 * SimpleAnim.c
 *
 * Example program illustrating a simple use
 * of OpenGL to animate motion.  Creates rotating
 * overlapping triangles.
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 * USAGE:
 *    Press "r" key to toggle (off and on) running the animation
 *    Press "s" key to single-step animation
 *    The up arrow key and down array key control the
 *			time step used in the animation rate.  Each key
 *			press multiplies or divides the times by a factor 
 *			of sqrt(2).
 *	  Press ESCAPE to exit.
 *
 */

#include <functional>
#include <math.h>		// For math routines (such as sqrt & trig).
#include <stdio.h>
#include <stdlib.h>		// For the "exit" function
#include <GL/glut.h>	// OpenGL Graphics Utility Library
#include "glanimator.h"


namespace glanimator
{


GLanimator::GLanimator() : Xmin(0.0), Xmax(3.0), Ymin(0.0), Ymax(3.0)
{
	RunMode = 1;
	CurrentAngle = 0.0;
	AnimateStep = 1.0 * M_PI / 180.0;
}


/*
 * drawScene() handles the animation and the redrawing of the
 *		graphics window contents.
 */
void GLanimator::drawScene()
{
	// Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (RunMode==1) {
		// Calculate animation parameters
        CurrentAngle += AnimateStep;
		if ( CurrentAngle > 360.0 ) {
			CurrentAngle -= 360.0*floor(CurrentAngle/360.0);	// Don't allow overflow
		}
	}
	
	// Rotate the image
	glMatrixMode( GL_MODELVIEW );			// Current matrix affects objects positions
	glLoadIdentity();						// Initialize to the identity
	glTranslatef( 1.5, 1.5, 0.0 );					// Translate rotation center from origin
	glRotatef( CurrentAngle, 0.0, 0.0, 1.0 );		// Rotate through animation angle
	glTranslatef( -1.5, -1.5, 0.0 );				// Translate rotation center to origin

	// Draw three overlapping triangles of different colors
	glBegin( GL_TRIANGLES );
	glColor3f( 1.0, 0.0, 0.0 );
	glVertex3f( 0.3, 1.0, 0.5 );
	glVertex3f( 2.7, 0.85, 0.0 );
	glVertex3f( 2.7, 1.15, 0.0 );

	glColor3f( 0.0, 1.0, 0.0 );
	glVertex3f(2.53, 0.71, 0.5 );
	glVertex3f(1.46, 2.86, 0.0 );
	glVertex3f(1.2, 2.71, 0.0 );

	glColor3f( 0.0, 0.0, 1.0 );
	glVertex3f(1.667, 2.79, 0.5);
	glVertex3f(0.337, 0.786, 0.0);
	glVertex3f(0.597, 0.636, 0.0);
	glEnd();

	// Flush the pipeline, swap the buffers
    glFlush();
    glutSwapBuffers();

	if ( RunMode==1 ) {
		glutPostRedisplay();	// Trigger an automatic redraw for animation
	}
}


// Initialize OpenGL's rendering modes
void GLanimator::initRendering()
{
    glShadeModel(GL_FLAT);	// The default value of GL_SMOOTH is usually better
    glEnable(GL_DEPTH_TEST);	// Depth testing must be turned on
}


// Called when the window is resized
//		w, h - width and height of the window in pixels.
void GLanimator::resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	// Define the portion of the window used for OpenGL rendering.
	glViewport( 0, 0, w, h );	// View port uses whole window

	// Set up the projection view matrix: orthographic projection
	// Determine the min and max values for x and y that should appear in the window.
	// The complication is that the aspect ratio of the window may not match the
	//		aspect ratio of the scene we want to view.
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}
	
	// Now that we know the max & min values for x & y 
	//		that should be visible in the window,
	//		we set up the orthographic projection.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );
}


// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void GLanimator::myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {
	case 'r':
		RunMode = 1-RunMode;		// Toggle to opposite value
		if ( RunMode==1 ) {
			glutPostRedisplay();
		}
		break;
	case 's':
		RunMode = 1;
		drawScene();
		RunMode = 0;
		break;
	case 27:	// Escape key
		exit(1);
	}
}


// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of
//		special keys.
void GLanimator::mySpecialKeyFunc( int key, int x, int y )
{
	switch ( key ) {
	case GLUT_KEY_UP:		
		if ( AnimateStep < 360.0 * M_PI / 180.0) {			// Avoid overflow problems
			AnimateStep += 1.0 * M_PI / 180.0;		// Increase the angle increment
		}
		break;
	case GLUT_KEY_DOWN:
		if (AnimateStep > 1.0 * M_PI / 180.0) {		// Avoid underflow problems.
			AnimateStep -= 1.0 * M_PI / 180.0;	// Decrease the angle increment
		}
		break;
	}
}


} // namespace glanimator