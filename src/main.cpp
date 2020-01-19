#include <random>
#include <GL/glut.h>
#include "common_cpp/common.h"
#include "glanimator.h"
#include "bicycle.h"

// OpenGL really likes global variables and functions
glanimator::GLanimator* glanimatorPtr;
bicycle::Bicycle* bicyclePtr;
void resizeWindow(int w, int h);
void updateSimulationAndDrawScene();
void myKeyboardFunc(unsigned char key, int x, int y);
void mySpecialKeyFunc(int key, int x, int y);

double t = 0;
double dt = 0;


/*========================== MAIN ==========================*/

int main(int argc, char** argv)
{
    // Initialize time info. and random seed
    int seed;
    common::get_yaml_node("seed", "../param/simulator.yaml", seed);
    common::get_yaml_node("dt", "../param/simulator.yaml", dt);
    if (seed < 0) seed = time(0);
    std::default_random_engine rng(seed);
    std::srand(seed);

    // Create vehicles, controllers, estimators, sensor packages
    bicycle::Bicycle bicycle("../param/bicycle.yaml");

    // Create animator class and give references to pointers for 
    // use in OpenGL's global functions
    glanimator::GLanimator glanimator(-10, 10, -10, 10);
    glanimatorPtr = &glanimator;
    bicyclePtr = &bicycle;

    // OpenGL processes
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowPosition(10, 60);
    glutInitWindowSize(360, 360);
    glutCreateWindow("Car Simulator");
    glanimatorPtr->initRendering();
	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpecialKeyFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(updateSimulationAndDrawScene);
	glutMainLoop();

    return 0;
}



/*========================= OPENGL =========================*/

void resizeWindow(int w, int h)
{
    glanimatorPtr->resizeWindow(w, h);
}

void updateSimulationAndDrawScene()
{
    bicyclePtr->propagate(t);
    glanimatorPtr->drawScene(t, dt, bicyclePtr->y(), bicyclePtr->x(), bicyclePtr->psi());
    std::cout << "north = " << bicyclePtr->x() << ", east = " << bicyclePtr->y() << ", steer angle = " << bicyclePtr->theta() << std::endl;

    // Ensure force/torque is only applied when key is pressed
    bicyclePtr->force() = 0;
    bicyclePtr->torque() = 0;
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
    glanimatorPtr->myKeyboardFunc(key, t, dt, bicyclePtr->y(), bicyclePtr->x(), bicyclePtr->psi());
}

void mySpecialKeyFunc(int key, int x, int y)
{
    glanimatorPtr->mySpecialKeyFunc(key, bicyclePtr->force(), bicyclePtr->torque(), bicyclePtr->max_force(), bicyclePtr->max_torque());
}
