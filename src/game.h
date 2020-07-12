#pragma once
#include "include_libraries.h"

class Game
{

private:
    // variables

    // window stuff
    GLFWwindow *window;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    int framebufferWidth;
    int framebufferHeight;

    // opengl context
    const int GL_VERSION_MAJOR;
    const int GL_VERSION_MINOR;

    // matrices
    glm::mat4 ViewMatrix;
    glm::vec3 camPosition;
    glm::vec3 worldUp;
    glm::vec3 camFront;

    glm::mat4 ProjectionMatrix;
    float fov;
    float nearPlane;
    float farPlane;

    // private functions
    void initGLFW();
    void initWindow(const char *title, bool resizable);
    void initGLEW(); // AFTER CONTEXT CREATION !!
    void initOpenGLOptions();
    void initMatrices();
    // Static Variables
public:
    // Constructors
    Game(const char *TITLE,
         const int WINDOW_WIDTH,
         const int WINDOW_HEIGHT,
         const int GL_VERSION_MAJOR,
         const int GL_VERSION_MINOR,
         bool resizable);
    virtual ~Game();

    // Accessors

    int getWindowShouldClose();

    // Modifiers

    void setWindowShouldClose();

    // Functions

    void update();
    void render();

    // Static Functions
    static void framebuffer_resize_callback(GLFWwindow *window, int fbW, int fbH);
    static void changeRenderMode(GLFWwindow *window, int key, int scancode, int action, int mods);
};