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

    void drawScene();
    void initRendering();
    void resizeWindow(int w, int h);
    void myKeyboardFunc(unsigned char key, int x, int y);
    void mySpecialKeyFunc(int key, int x, int y);

private:

    int RunMode;		// Used as a boolean (1 or 0) for "on" and "off"

	// The next global variable controls the animation's state and speed.
	float CurrentAngle;			// Angle in degrees
	float AnimateStep;			// Rotation step per update

	// These variables set the dimensions of the rectanglar region we wish to view.
	const double Xmin, Xmax;
	const double Ymin, Ymax;

};


} // namespace glanimator