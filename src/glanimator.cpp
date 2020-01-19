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

GLanimator::GLanimator()
{
	run_mode = 1;
}


GLanimator::GLanimator(const double& _x_min, const double& _x_max,
					   const double& _y_min, const double& _y_max)
					   : x_min(_x_min), x_max(_x_max), y_min(_y_min), y_max(_y_max)
{
	run_mode = 1;
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
	double windowx_min, windowx_max, windowy_min, windowy_max;

	// Define the portion of the window used for OpenGL rendering.
	glViewport(0, 0, w, h);	// View port uses whole window

	// Set up the projection view matrix: orthographic projection
	// Determine the min and max values for x and y that should appear in the window.
	// The complication is that the aspect ratio of the window may not match the
	//		aspect ratio of the scene we want to view.
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	if ( (x_max-x_min)/w < (y_max-y_min)/h )
	{
		scale = ((y_max-y_min)/h)/((x_max-x_min)/w);
		center = (x_max+x_min)/2;
		windowx_min = center - (center-x_min)*scale;
		windowx_max = center + (x_max-center)*scale;
		windowy_min = y_min;
		windowy_max = y_max;
	} 
	else
	{
		scale = ((x_max-x_min)/w)/((y_max-y_min)/h);
		center = (y_max+y_min)/2;
		windowy_min = center - (center-y_min)*scale;
		windowy_max = center + (y_max-center)*scale;
		windowx_min = x_min;
		windowx_max = x_max;
	}
	
	// Now that we know the max & min values for x & y 
	//		that should be visible in the window,
	//		we set up the orthographic projection.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( windowx_min, windowx_max, windowy_min, windowy_max, -1, 1 );
}


/*
 * drawScene() handles the animation and the redrawing of the
 *		graphics window contents.
 */
void GLanimator::drawScene(double& t, const double& dt, const double& px, const double& py, const double& psi)
{
	// Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (run_mode == 1)
	{
		t += dt;
	}
	
	// Rotate the image
	glMatrixMode(GL_MODELVIEW);			// Current matrix affects objects positions
	glLoadIdentity();						// Initialize to the identity
	glTranslatef( 1.5, 1.5, 0.0 );					// Translate rotation center from origin
	glRotatef( 0.0, 0.0, 0.0, 1.0 );		// Rotate through animation angle
	glTranslatef( -1.5, -1.5, 0.0 );				// Translate rotation center to origin

	// Draw three overlapping triangles of different colors
	glBegin( GL_TRIANGLES );
	glColor3f( 1.0, 0.0, 0.0 );
	glVertex3f( 0.3, 1.0, 0.5 );
	glVertex3f( 2.7, 0.85, 0.0 );
	glVertex3f( 2.7, 1.15, 0.0 );
	glEnd();

	// Flush the pipeline, swap the buffers
    glFlush();
    glutSwapBuffers();

	if (run_mode == 1)
	{
		glutPostRedisplay();	// Trigger an automatic redraw for animation
	}
}


// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void GLanimator::myKeyboardFunc(unsigned char key, double& t, const double& dt, const double& px, const double& py, const double& psi)
{
	switch (key)
	{
	case 'r':
		run_mode = 1 - run_mode; // Toggle to opposite value
		if (run_mode == 1)
		{
			glutPostRedisplay();
		}
		break;
	case 's':
		run_mode = 1;
		drawScene(t, dt, px, py, psi);
		run_mode = 0;
		break;
	case 27:	// Escape key
		exit(1);
	}
}


// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of
//		special keys.
void GLanimator::mySpecialKeyFunc(int key, double& force, double& torque, const double& max_force, const double& max_torque)
{
	switch (key)
	{
	case GLUT_KEY_UP:		
		// Apply forward force
		force = max_force;
		break;
	case GLUT_KEY_DOWN:
		// Apply reverse force
		force = -max_force;
		break;
	case GLUT_KEY_RIGHT:
		// Apply right steering torque
		torque = max_torque;
		break;
	case GLUT_KEY_LEFT:
		// Apply left steering torque
		torque = -max_torque;
		break;
	}
}


} // namespace glanimator