#include <cmath>
#include <iostream>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Mesh.h"
#include "Model.h"
#include "Camera.h"
#include "ShaderIO.h"


#define RIGHT true
#define LEFT false
#define UP true
#define DOWN false
#define LIGHT_MOVE_CONST 0.08f
#define KEY_FORWARD       ('w') // Key used to toggle wireframe rendering   //
#define KEY_BACKWORD       ('s') // Key used to toggle wireframe rendering   //
#define KEY_LEFT      ('a') // Key used to toggle wireframe rendering   //
#define KEY_RIGHT       ('d') // Key used to toggle wireframe rendering   //



Mesh simple;
Model m;
static float currentTime, deltaTime, prevTime = 0;


void keyOperations();

void init( void )
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    programManager::sharedInstance().createProgram("simple", "../shaders/phong.vert", "../shaders/phong.frag");
    programManager::sharedInstance().createProgram("sill", "../shaders/silhouette.vs.glsl", "../shaders/silhouette.gs.glsl", "../shaders/silhouette.fs.glsl");
//    programManager::sharedInstance().createProgram("sill", "../shaders/silhouette.vs.glsl", "../shaders/silhouette.fs.glsl");
    simple.init("../mesh/cube.off", glm::vec3(0.f));
    m.init();
//    simple.init("../mesh/cube.off", prog);
}


void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
    GLuint prog = programManager::sharedInstance().programWithID("simple");
    glUseProgram(prog);
    GLint wUniform = glGetUniformLocation(prog, "gView");
    glm::mat4 v = Camera::getInstance().getView();
    glUniformMatrix4fv(wUniform, 1, GL_FALSE, glm::value_ptr(v));
    GLint vUniform = glGetUniformLocation(prog, "gModel");
    glm::mat4 w = simple.get_model();
    glUniformMatrix4fv(vUniform, 1, GL_FALSE, glm::value_ptr(w));
    GLint pUniform = glGetUniformLocation(prog, "gProjection");
    glm::mat4 p = Camera::getInstance().getPerspective();
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, glm::value_ptr(p));
    GLint viewUniform = glGetUniformLocation(prog, "gEyePosition");
    glUniform3fv(viewUniform, 1, glm::value_ptr(Camera::getInstance().getPos()));
    GLint lightUniform = glGetUniformLocation(prog, "gLightPosition");

    glm::vec4 lights[2] = {glm::vec4(0.f, -10.f, 0.0f, 0.f), glm::vec4(0.f, -6.f, 0.f, 0.f)};

    glUniform4fv(lightUniform, 2, glm::value_ptr(lights[0]));
    GLint metUniform = glGetUniformLocation(prog, "gMaterialColor");
    glUniform4fv(metUniform, 1, glm::value_ptr(glm::vec4(1.f, 0.f, 1.f, 1.f)));
    simple.set_adjacency(false);
//    simple.draw(prog);
    glUseProgram(0);

    prog = programManager::sharedInstance().programWithID("sill");
    glUseProgram(prog);
    wUniform = glGetUniformLocation(prog, "wvp");
    glm::mat4 wvp = p * v * w;
    glUniformMatrix4fv(wUniform, 1, GL_FALSE, glm::value_ptr(wvp));
    viewUniform = glGetUniformLocation(prog, "world");
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(w));
    lightUniform = glGetUniformLocation(prog, "gLightPos");
    glUniform3fv(lightUniform, 1, glm::value_ptr(glm::vec3(0.f, 10.f, 0.f)));
    glLineWidth(10.0f);
    simple.set_adjacency(true);
    simple.draw(prog);
    glUseProgram(0);

//    m.draw(p * v * glm::mat4(1.f));
    glutSwapBuffers();
}

void keyOperations()
{

}

void specialkey(int key, int x, int y )
{
    switch (key)
    {
        case 'Q':
        case 'q':
        case 27:
            Camera::getInstance().destroy();
            exit(0);
        default:
            std::cerr<< static_cast<char>(key)<<" not initialize key"<<std::endl;
            break;
    }
    glutPostRedisplay();
}

/********************************************************************
 * Function  :   motion
 * Arguments :   x   : x valustd::cout<<"We are currently at ("<<x<<","<<y<<")"<<std::endl;e of the current mouse location
 *               y   : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse dragging events.
 *
 \******************************************************************/
void motion(int x, int y)
{
//	std::cout<<"x pos: "<<x<<" "<<"y pos: "<<y<<std::endl;

//	float dt = glutGet(GLUT_ELAPSED_TIME);
//    if (x!=lastPos.x || y!=lastPos.y) {
//        int speed = 25;
//        //calculate the difference vector.
//        difference= glm::vec3(1.0f*(x-lastPos.x)/glutGet(GLUT_INIT_WINDOW_WIDTH) * speed
//                ,-1.0f*(y-lastPos.y)/glutGet(GLUT_INIT_WINDOW_HEIGHT) * speed
//                ,0.0f);
//
//        //obtain the transformation matrix
//        spotLightModel = glm::translate(spotLightModel,difference);
//        //pre multiply to perform the translation and update the model matrix.
//        //update the x/y mouse positions.
//        lastPos.x = x;
//        lastPos.y = y;
//    }

//    if(x < 0)
//    {
//        glutWarpPointer(0, y);
//    }
//    else if(x > 512)
//    {
//        glutWarpPointer(512, y);
//    }
//    else if(y < 0)
//    {
//        glutWarpPointer(x, 0);
//    }
//    else if(y > 512)
//    {
//        glutWarpPointer(x, 512);
//    }
//    glutPostRedisplay();
//	lightPos1[0] = ((x - lightPos1[0])/ 512.f);
//	lightPos1[1] = ((y - lightPos1[1]) / 512.f);

}

void update()
{
//    static int currentTime, deltaTime, prevTime = 0;
    static int startTime, elapsedTime = 0;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    keyOperations();

    // translate & rotateY around x/y axis
    static float angle = 0.0f;
    angle += 0.1f;
    glm::mat4 xTranslate, xRotation;

    // 1st geometry translate & rotate in x & rotateY in y
    xTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1, 0));
    xRotation = glm::rotate(xTranslate, angle, glm::vec3(1, 0, 0));

    glutPostRedisplay();
}

int main( int argc, char **argv )
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize( 512, 512 );
    glutCreateWindow("ESCAPE IT!");

    glewExperimental = GL_TRUE;
    glewInit();
#ifdef __APPLE__
    GLint sync = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif

    init();

    glutDisplayFunc( display );
//    glutKeyboardFunc(keyPressed);
//    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc( specialkey );
//    glutPassiveMotionFunc(motion);
    glutIdleFunc( update );

    glutMainLoop();

    return 0;
}