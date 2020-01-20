/*
 * SimpleAnim.h
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
 */
#pragma once


namespace glanimator
{


class GLanimator
{
public:

    GLanimator();
    GLanimator(const std::string& filename);

    void initRendering();
    void resizeWindow(int w, int h);
    void drawScene(double& t, const double& dt, const double& px, const double& py, const double& psi, const double& theta);
    void myKeyboardFunc(unsigned char key, double& t, const double& dt, const double& px, const double& py, const double& psi, const double& theta);
    void mySpecialKeyFunc(int key, double& force, double& torque, const double& max_force, const double& max_torque);

private:

    int run_mode; // Toggle between step by step drawing or real time

	// These variables set the dimensions of the rectanglar region we wish to view.
	const double x_min = -100.0;
    const double x_max =  100.0;
	const double y_min = -100.0;
    const double y_max =  100.0;

    // Car dimensions
    double car_length = 4.0;
    const double car_width = 0.5*car_length;
    const double tire_length = 0.3*car_length;
    const double tire_width = 0.4*tire_length;

};


} // namespace glanimator