#include <GL/glut.h>
#include "glanimator.h"

// OpenGL really likes global variables
glanimator::GLanimator* glanimatorPtr;
void initRendering();
void resizeWindow(int w, int h);
void drawScene();
void myKeyboardFunc(unsigned char key, int x, int y);
void mySpecialKeyFunc(int key, int x, int y);



int main(int argc, char** argv)
{
    glanimator::GLanimator glanimator;
    glanimatorPtr = &glanimator;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowPosition(10, 60);
    glutInitWindowSize(360, 360);
    glutCreateWindow("Car Simulator");
    initRendering();
	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpecialKeyFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(drawScene);
	glutMainLoop();

    return 0;
}



// Implementations of OpenGL functions
void initRendering()
{
    glanimatorPtr->initRendering();
}

void resizeWindow(int w, int h)
{
    glanimatorPtr->resizeWindow(w, h);
}

void drawScene()
{
    glanimatorPtr->drawScene();
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
    glanimatorPtr->myKeyboardFunc(key, x, y);
}

void mySpecialKeyFunc(int key, int x, int y)
{
    glanimatorPtr->mySpecialKeyFunc(key, x, y);
}
