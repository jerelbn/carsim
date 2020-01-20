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
#include "common_cpp/transform.h"


namespace glanimator
{

GLanimator::GLanimator()
{
	run_mode = 1;
	t0 = std::chrono::high_resolution_clock::now();
}


GLanimator::GLanimator(const std::string& filename)
{
	run_mode = 1;
	t0 = std::chrono::high_resolution_clock::now();
	common::get_yaml_node("length", filename, car_length);
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
void GLanimator::drawScene(double& t, const double& dt, const double& px, const double& py, const double& psi, const double& theta)
{
	// This controls propagation so that that we can step through the simulation if desired
    if (run_mode == 1) t += dt;

	// Draw at real time
	// WARNING: This breaks as soon as the simulation runs slower than real time!
	double t_actual = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t0).count();
	if (t >= t_actual*1e-6)
	{
		t_draw_prev = t;

		// Clear the rendering window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Rotate the image
		glMatrixMode(GL_MODELVIEW);			// Current matrix affects objects positions
		glLoadIdentity();						// Initialize to the identity
		// glTranslatef( 1.5, 1.5, 0.0 );					// Translate rotation center from origin
		// glRotatef( 0.0, 0.0, 0.0, 1.0 );		// Rotate through animation angle
		// glTranslatef( -1.5, -1.5, 0.0 );				// Translate rotation center to origin

		// Define body and tire vertices in GL frame (x-forward, y-left) at identity
		static const Eigen::Vector3f body_fl_sq( 0.6*car_length,  0.5*car_width, 0.0); // vertex body front-left square
		static const Eigen::Vector3f body_fl_tr( 1.0*car_length,  0.3*car_width, 0.0); // vertex body front-left trapezoid
		static const Eigen::Vector3f body_fr_tr( 1.0*car_length, -0.3*car_width, 0.0); // vertex body front-right trapezoid
		static const Eigen::Vector3f body_fr_sq( 0.6*car_length, -0.5*car_width, 0.0); // vertex body front-right square
		static const Eigen::Vector3f body_rr_sq( 0.2*car_length, -0.5*car_width, 0.0); // vertex body rear-right square
		static const Eigen::Vector3f body_rr_tr( 0.0*car_length, -0.4*car_width, 0.0); // vertex body rear-right trapezoid
		static const Eigen::Vector3f body_rl_tr( 0.0*car_length,  0.4*car_width, 0.0); // vertex body rear-left trapezoid
		static const Eigen::Vector3f body_rl_sq( 0.2*car_length,  0.5*car_width, 0.0); // vertex body rear-left square
		static const Eigen::Vector3f tire_fl( 0.5*tire_length,  0.5*tire_width, 0.0); // vertex tire front-left
		static const Eigen::Vector3f tire_fr( 0.5*tire_length, -0.5*tire_width, 0.0); // vertex tire front-right
		static const Eigen::Vector3f tire_rr(-0.5*tire_length, -0.5*tire_width, 0.0); // vertex tire rear-right
		static const Eigen::Vector3f tire_rl(-0.5*tire_length,  0.5*tire_width, 0.0); // vertex tire rear-left
		static const Eigen::Vector3f tire_center_fl( 1.0*car_length,  0.5*car_width, 0.0); // tire center front-left
		static const Eigen::Vector3f tire_center_fr( 1.0*car_length, -0.5*car_width, 0.0); // tire center front-right
		static const Eigen::Vector3f tire_center_rr( 0.0*car_length, -0.5*car_width, 0.0); // tire center rear-right
		static const Eigen::Vector3f tire_center_rl( 0.0*car_length,  0.5*car_width, 0.0); // tire center rear-left

		// Actively tranform all vertices to align with simulation
		common::Transformf x_GL_to_b(Eigen::Vector3f(px, py, 0), common::Quaternionf(0, 0, common::wrapAngle(-psi+M_PI/2,M_PI)));
		common::Quaternionf q_theta(0, 0, theta);

		Eigen::Vector3f body_fl_sq_b = x_GL_to_b.inv().transform(body_fl_sq);
		Eigen::Vector3f body_fl_tr_b = x_GL_to_b.inv().transform(body_fl_tr);
		Eigen::Vector3f body_fr_tr_b = x_GL_to_b.inv().transform(body_fr_tr);
		Eigen::Vector3f body_fr_sq_b = x_GL_to_b.inv().transform(body_fr_sq);
		Eigen::Vector3f body_rr_sq_b = x_GL_to_b.inv().transform(body_rr_sq);
		Eigen::Vector3f body_rr_tr_b = x_GL_to_b.inv().transform(body_rr_tr);
		Eigen::Vector3f body_rl_tr_b = x_GL_to_b.inv().transform(body_rl_tr);
		Eigen::Vector3f body_rl_sq_b = x_GL_to_b.inv().transform(body_rl_sq);
		Eigen::Vector3f tire_fl_fl_b = x_GL_to_b.inv().transform(tire_center_fl+q_theta.rot(tire_fl));
		Eigen::Vector3f tire_fl_fr_b = x_GL_to_b.inv().transform(tire_center_fl+q_theta.rot(tire_fr));
		Eigen::Vector3f tire_fl_rr_b = x_GL_to_b.inv().transform(tire_center_fl+q_theta.rot(tire_rr));
		Eigen::Vector3f tire_fl_rl_b = x_GL_to_b.inv().transform(tire_center_fl+q_theta.rot(tire_rl));
		Eigen::Vector3f tire_fr_fl_b = x_GL_to_b.inv().transform(tire_center_fr+q_theta.rot(tire_fl));
		Eigen::Vector3f tire_fr_fr_b = x_GL_to_b.inv().transform(tire_center_fr+q_theta.rot(tire_fr));
		Eigen::Vector3f tire_fr_rr_b = x_GL_to_b.inv().transform(tire_center_fr+q_theta.rot(tire_rr));
		Eigen::Vector3f tire_fr_rl_b = x_GL_to_b.inv().transform(tire_center_fr+q_theta.rot(tire_rl));
		Eigen::Vector3f tire_rr_fl_b = x_GL_to_b.inv().transform(tire_center_rr+tire_fl);
		Eigen::Vector3f tire_rr_fr_b = x_GL_to_b.inv().transform(tire_center_rr+tire_fr);
		Eigen::Vector3f tire_rr_rr_b = x_GL_to_b.inv().transform(tire_center_rr+tire_rr);
		Eigen::Vector3f tire_rr_rl_b = x_GL_to_b.inv().transform(tire_center_rr+tire_rl);
		Eigen::Vector3f tire_rl_fl_b = x_GL_to_b.inv().transform(tire_center_rl+tire_fl);
		Eigen::Vector3f tire_rl_fr_b = x_GL_to_b.inv().transform(tire_center_rl+tire_fr);
		Eigen::Vector3f tire_rl_rr_b = x_GL_to_b.inv().transform(tire_center_rl+tire_rr);
		Eigen::Vector3f tire_rl_rl_b = x_GL_to_b.inv().transform(tire_center_rl+tire_rl);

		// Draw the car - square middle with trapezoid front/rear sections
		glBegin(GL_POLYGON);
		glColor3f(1.0, 0.6, 0.2); // orange
		glVertex3f(body_fl_sq_b(0), body_fl_sq_b(1), body_fl_sq_b(2)); // vertex front-left square
		glVertex3f(body_fl_tr_b(0), body_fl_tr_b(1), body_fl_tr_b(2)); // vertex front-left trapezoid
		glVertex3f(body_fr_tr_b(0), body_fr_tr_b(1), body_fr_tr_b(2)); // vertex front-right trapezoid
		glVertex3f(body_fr_sq_b(0), body_fr_sq_b(1), body_fr_sq_b(2)); // vertex front-right square
		glVertex3f(body_rr_sq_b(0), body_rr_sq_b(1), body_rr_sq_b(2)); // vertex rear-right square
		glVertex3f(body_rr_tr_b(0), body_rr_tr_b(1), body_rr_tr_b(2)); // vertex rear-right trapezoid
		glVertex3f(body_rl_tr_b(0), body_rl_tr_b(1), body_rl_tr_b(2)); // vertex rear-left trapezoid
		glVertex3f(body_rl_sq_b(0), body_rl_sq_b(1), body_rl_sq_b(2)); // vertex rear-left square
		glEnd();

		// Draw the tires
		glBegin(GL_POLYGON); // front-left
		glColor3f(0.6, 0.6, 0.6);
		glVertex3f(tire_fl_fl_b(0), tire_fl_fl_b(1), tire_fl_fl_b(2)); // vertex front-left
		glVertex3f(tire_fl_fr_b(0), tire_fl_fr_b(1), tire_fl_fr_b(2)); // vertex front-right
		glVertex3f(tire_fl_rr_b(0), tire_fl_rr_b(1), tire_fl_rr_b(2)); // vertex rear-right
		glVertex3f(tire_fl_rl_b(0), tire_fl_rl_b(1), tire_fl_rl_b(2)); // vertex rear-left
		glEnd();
		glBegin(GL_POLYGON); // front-right
		glColor3f(0.6, 0.6, 0.6);
		glVertex3f(tire_fr_fl_b(0), tire_fr_fl_b(1), tire_fr_fl_b(2)); // vertex front-left
		glVertex3f(tire_fr_fr_b(0), tire_fr_fr_b(1), tire_fr_fr_b(2)); // vertex front-right
		glVertex3f(tire_fr_rr_b(0), tire_fr_rr_b(1), tire_fr_rr_b(2)); // vertex rear-right
		glVertex3f(tire_fr_rl_b(0), tire_fr_rl_b(1), tire_fr_rl_b(2)); // vertex rear-left
		glEnd();
		glBegin(GL_POLYGON); // rear-right
		glColor3f(0.6, 0.6, 0.6);
		glVertex3f(tire_rr_fl_b(0), tire_rr_fl_b(1), tire_rr_fl_b(2)); // vertex front-left
		glVertex3f(tire_rr_fr_b(0), tire_rr_fr_b(1), tire_rr_fr_b(2)); // vertex front-right
		glVertex3f(tire_rr_rr_b(0), tire_rr_rr_b(1), tire_rr_rr_b(2)); // vertex rear-right
		glVertex3f(tire_rr_rl_b(0), tire_rr_rl_b(1), tire_rr_rl_b(2)); // vertex rear-left
		glEnd();
		glBegin(GL_POLYGON); // rear-left
		glColor3f(0.6, 0.6, 0.6);
		glVertex3f(tire_rl_fl_b(0), tire_rl_fl_b(1), tire_rl_fl_b(2)); // vertex front-left
		glVertex3f(tire_rl_fr_b(0), tire_rl_fr_b(1), tire_rl_fr_b(2)); // vertex front-right
		glVertex3f(tire_rl_rr_b(0), tire_rl_rr_b(1), tire_rl_rr_b(2)); // vertex rear-right
		glVertex3f(tire_rl_rl_b(0), tire_rl_rl_b(1), tire_rl_rl_b(2)); // vertex rear-left
		glEnd();

		// Flush the pipeline, swap the buffers
		glFlush();
		glutSwapBuffers();
	}

	if (run_mode == 1)
	{
		glutPostRedisplay();	// Trigger an automatic redraw for animation
	}
}


// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void GLanimator::myKeyboardFunc(unsigned char key, double& t, const double& dt, const double& px, const double& py, const double& psi, const double& theta)
{
	switch (key)
	{
	case 'r':
		t0 = std::chrono::high_resolution_clock::now() - std::chrono::microseconds(unsigned(t*1e6));
		run_mode = 1 - run_mode; // Toggle to opposite value
		if (run_mode == 1)
		{
			glutPostRedisplay();
		}
		break;
	case 's':
		t0 = std::chrono::high_resolution_clock::now() - std::chrono::microseconds(unsigned(t*1e6));
		run_mode = 1;
		drawScene(t, dt, px, py, psi, theta);
		run_mode = 0;
		break;
	case 27:	// Escape key
		exit(1);
	}
}


// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of`
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