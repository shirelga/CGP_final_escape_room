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

#include "Mesh.h"
#include "Model.h"
#include "ShadingTech.h"
#include "NullTech.h"
#include "ShadowVolumeTech.h"
#include "Texture.h"

#include "Camera.h"
#include "ShaderIO.h"


#define RIGHT true
#define LEFT false
#define NUM_OF_LIGHTS 1
#define WALLS_COLOR glm::vec4(1.f, 1.f, 1.f, 1.f);
#define FLOOR_COLOR glm::vec4(0.5f, 1.f, 0.7f, 1.f);
#define LIGHT_COLOR1 glm::vec3(1.f, 1.f, 1.f);
#define LIGHT_COLOR2 glm::vec3(1.f, 1.f, 0.875f);
#define LIGHT_POS1 glm::vec4(0.f, 19.8f, 10.f, 1.f);
#define LIGHT_POS2 glm::vec4(0.f, 0.f, -5.f, 1.f);
#define TEX_PATH "../textures/"
#define WIDTH glutGet(GLUT_SCREEN_WIDTH)
#define HIEGHT glutGet(GLUT_SCREEN_HEIGHT)
#define NUM_OF_FURNITURES 6

enum TECH{
    NULL_TECH, SHADOW_TECH, SHADING_TECH
};

std::vector<Mesh *> furnitures;
Model surface, cieling, wall_front, wall_back, wall_left, wall_right;
ShadingTech gShadingTech;
NullTechnique gNullTech;
ShadowVolumeTechnique gShadowVolumeTech;
glm::vec3 gLights_color[2];
glm::vec4 gLights_pos[2];
static float fps = 0, currentTime, deltaTime, prevTime = 0;


void RenderSceneIntoDepth();
void RenderShadowVolIntoStencil();
void RenderShadowedScene();
void RenderAmbientLight();
void init_room();
void init_furniture();
void draw_room(TECH t);
void draw_furniture(TECH t);

void init( void )
{
    Camera::getInstance().set_screen_width(WIDTH);
    Camera::getInstance().set_screen_hieght(HIEGHT);

    glm::vec3 eye;

    gShadingTech.init();
    gLights_color[0] = LIGHT_COLOR1
    gLights_color[1] = LIGHT_COLOR2
    gLights_pos[0] = LIGHT_POS1
    gLights_pos[1] = LIGHT_POS2

    eye = Camera::getInstance().getPos();
    gShadingTech.use();
    gShadingTech.set_light(gLights_pos, NUM_OF_LIGHTS, gLights_color);
    gShadingTech.set_eye(eye);

    gNullTech.Init();
    gShadowVolumeTech.init();

    init_furniture();

    init_room();
}


void display( void )
{
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    RenderSceneIntoDepth();

    glEnable(GL_STENCIL_TEST);

    RenderShadowVolIntoStencil();

    RenderShadowedScene();

    glDisable(GL_STENCIL_TEST);

    RenderAmbientLight();

    glutSwapBuffers();
}


void keyPressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            Camera::getInstance().destroy();
            for(Mesh * f : furnitures)
            {
                delete f;
            }
            exit(0);
        default:
            Camera::getInstance().keyPressed(key, x, y);
            break;
    }
    glutPostRedisplay();
}


void update()
{
//    static int currentTime, deltaTime, prevTime = 0;
//    static int startTime, elapsedTime = 0;
    fps++;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentTime - prevTime;
    if(deltaTime > 1000)
    {
//        std::cout<<"fps: "<<fps/(deltaTime / 1000.f)<<std::endl;

        prevTime = currentTime;

        fps = 0;

    }

    glutPostRedisplay();
}

void RenderSceneIntoDepth()
{
    glDrawBuffer(GL_NONE);

    gNullTech.use();

    draw_furniture(NULL_TECH);

    draw_room(NULL_TECH);
}

void RenderShadowVolIntoStencil()
{
    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_CLAMP);
    glDisable(GL_CULL_FACE);

    // We need the stencil test to be enabled but we want it
    // to succeed always. Only the depth test matters.
    glStencilFunc(GL_ALWAYS, 0, 0xff);


    gShadowVolumeTech.use();

    gShadowVolumeTech.SetLightPos(glm::vec3(gLights_pos[0]));

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    draw_furniture(SHADOW_TECH);

    // Restore local stuff
    glDisable(GL_DEPTH_CLAMP);
    glEnable(GL_CULL_FACE);
}

void RenderShadowedScene()
{
    glDrawBuffer(GL_BACK);

    // Draw only if the corresponding stencil value is zero
    glStencilFunc(GL_EQUAL, 0x0, 0xFF);

    // prevent update to the stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    gShadingTech.use();
    gShadingTech.set_ambient(0.1f);
    gShadingTech.set_diffuse(0.4f);

    draw_furniture(SHADING_TECH);
    draw_room(SHADING_TECH);
}


void RenderAmbientLight()
{

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    gShadingTech.use();
    gShadingTech.set_ambient(0.1f);
    gShadingTech.set_diffuse(0.4f);

    draw_furniture(SHADING_TECH);

    draw_room(SHADING_TECH);

    glDisable(GL_BLEND);
}

void init_furniture()
{
    std::string tmp[6] = {TEX_PATH "wall1.jpg", TEX_PATH "wall2.jpg",TEX_PATH "wall3.jpg",
                          TEX_PATH "wall1.jpg", TEX_PATH "wall2.jpg",TEX_PATH "wall3.jpg"};
    glm::vec3 positions[] = {
                            glm::vec3(0.f, 5.f, -20.f),
                            glm::vec3(-(4 * 19.8 - 3.f), 8.f, 0.f),
                            glm::vec3((4 * 19.8 - 3.f), 8.f, 0.f),
                            glm::vec3(0.f, 5.f, 20.f),
                            glm::vec3(0.f, 8.f, (4 * 19.8 - 3.f)),
                            glm::vec3(0.f, 8.f, -(4 * 19.8 - 3.f))
                            };

    glm::vec3 scales[] = {
                        glm::vec3(5.f),
                        glm::vec3(3.f, 8.f, 40.f),
                        glm::vec3(3.f, 8.f, 40.f),
                        glm::vec3(5.f),
                        glm::vec3(40.f, 8.f, 3.f),
                        glm::vec3(40.f, 8.f, 3.f)
                        };
    for(int i = 0; i < NUM_OF_FURNITURES; i++)
    {
        Mesh * f;
        try {
           f = new Mesh();
        } catch (std::bad_alloc &e)
        {
            std::cerr<<e.what()<<std::endl;
            exit(1);
        }
        f->init("../mesh/cube.off", tmp, 6,
                positions[i], true);
        f->scale(scales[i]);
        furnitures.push_back(f);
    }
}

void draw_furniture(TECH t)
{
    glm::mat4 w, v, p;
    glm::vec4 met_color;
    v = Camera::getInstance().getView();
    p = Camera::getInstance().getPerspective();
    for(auto f : furnitures)
    {
        w = f->get_model();
        switch (t) {
            case NULL_TECH:
                if(glIsEnabled(GL_CULL_FACE))
                {
                    glDisable(GL_CULL_FACE);
                }
                gNullTech.SetWVP(p * v * w);
                f->draw();
                if(!glIsEnabled(GL_CULL_FACE)) {
                    glEnable(GL_CULL_FACE);
                }
                break;
            case SHADOW_TECH:
                gShadowVolumeTech.SetWVP(p * v * w);
                f->draw();
                break;
            case SHADING_TECH:
                met_color = glm::vec4(1.f, 0.f, 1.f, 1.f);
                gShadingTech.set_wvp(w, v, p);
                gShadingTech.set_material(met_color);
                gShadingTech.set_texture_type(true);
                f->draw();
                gShadingTech.set_texture_type(false);
                break;
        }
    }
}


void init_room()
{
    surface.init(TEX_PATH "floor.jpg");
    surface.translte(glm::vec3(0.f, 0, 0.f));
    surface.scale(glm::vec3(4.f, 1.f, 4.f));

    cieling.init("");
    cieling.translte(glm::vec3(0.f, 2 * 19.8f, 0.f));
    cieling.scale(glm::vec3(4.f, 1.f, 4.f));

    wall_left.init(TEX_PATH "wall1.jpg");
    wall_left.rotate(90.f, glm::vec3(0,0,1));
    wall_left.translte(glm::vec3(4 * 19.8f, 19.8f, 0.f));
    wall_left.scale(glm::vec3(1.f, 1.f, 4.f));

    wall_right.init(TEX_PATH "wall1.jpg");
    wall_right.rotate(90.f, glm::vec3(0,0,1));
    wall_right.translte(glm::vec3(4 * -19.8f, 19.8f, 0.f));
    wall_right.scale(glm::vec3(1.f, 1.f, 4.f));

    wall_back.init(TEX_PATH "wall2.jpg");
    wall_back.rotate(90.f, glm::vec3(1, 0, 0));
    wall_back.translte(glm::vec3(0.f, 19.8f, 4 * -19.8));
    wall_back.scale(glm::vec3(4.f, 1.f, 1.f));


    wall_front.init(TEX_PATH "wall1.jpg");
    wall_front.rotate(90.f, glm::vec3(1, 0, 0));
    wall_front.translte(glm::vec3(0.f, 19.8f, 4 * 19.8));
    wall_front.scale(glm::vec3(4.f,1.f, 1.f));
}

void draw_room(TECH t)
{
    glm::mat4 w, v, p;
    glm::vec4 met_color;

    v = Camera::getInstance().getView();
    p = Camera::getInstance().getPerspective();
    glDisable(GL_CULL_FACE);
    switch (t) {
        case NULL_TECH:
            glDisable(GL_CULL_FACE);
            w = surface.get_model();
            gNullTech.SetWVP(p * v * w);
            surface.draw();

            w = cieling.get_model();
            gNullTech.SetWVP(p * v * w);
            cieling.draw();

            w = wall_left.get_model();
            gNullTech.SetWVP(p * v * w);
            wall_left.draw();

            w = wall_right.get_model();
            gNullTech.SetWVP(p * v * w);
            wall_right.draw();

            w = wall_back.get_model();
            gNullTech.SetWVP(p * v * w);
            wall_back.draw();

            w = wall_front.get_model();
            gNullTech.SetWVP(p * v * w);
            wall_front.draw();
            break;
        case SHADING_TECH:
            glDisable(GL_CULL_FACE);
            gShadingTech.set_texture_type(false);

            met_color = WALLS_COLOR;
            gShadingTech.set_material(met_color);
            w = cieling.get_model();
            gShadingTech.set_wvp(w, v, p);
            cieling.draw();

            w = wall_right.get_model();
            gShadingTech.set_wvp(w, v, p);
            wall_right.draw();

            w = wall_front.get_model();
            gShadingTech.set_wvp(w, v, p);
            wall_front.draw();

            w = wall_left.get_model();
            gShadingTech.set_wvp(w, v, p);
            wall_left.draw();

            w = wall_back.get_model();
            gShadingTech.set_wvp(w, v, p);
            wall_back.draw();

            met_color = FLOOR_COLOR;
            gShadingTech.set_material(met_color);
            w = surface.get_model();
            gShadingTech.set_wvp(w, v, p);
            surface.draw();
            break;
    }
    glEnable(GL_CULL_FACE);
}

int main( int argc, char **argv )
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH |GLUT_STENCIL | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(WIDTH, HIEGHT);
    glutCreateWindow("ESCAPE IT!");

    glewExperimental = GL_TRUE;
    glewInit();
#ifdef __APPLE__
    GLint sync = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc(keyPressed);
    glutIdleFunc( update );

    glutMainLoop();

    return 0;
}