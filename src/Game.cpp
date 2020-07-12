#include "game.h"

// Private functions

void Game::initGLFW()
{
    // INIT GLFW
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "ERROR:GAME.CPP::GLFW_INIT_FAILED" << std::endl;
        glfwTerminate();
        exit(1);
    }
}

void Game::initWindow(
    const char *title,
    bool resizable)
{
    // CREATE WINDOW

    // do not allow to use fixed function pipeline
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // SET OPENGL VERSION MAJOR.MINOR
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);

    // SET WHETHER OPENGL IS RESIZABLE OR NOT
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    // macOS
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // creates a window and context, but does not change the current context
    // to set the context as current, do it separately
    GLFWwindow *window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

    if (window == nullptr)
    {
        glfwTerminate();
        std::cout << "ERROR:GAME.CPP::GLFW_WINDOW_COULD_NOT_BE_CREATED" << std::endl;
        exit(1);
    }

    // FOR RESIZABLE WINDOWS
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    // FOR NON RESIZABLE WINDOWS

    // enabling this to get initial window size, before we hit the while loop.
    glfwGetFramebufferSize(window, &this->framebufferWidth, &this->framebufferHeight);
    // glViewport(0, 0, framebufferWidth, framebufferHeight);

    glfwMakeContextCurrent(window); // IMPORTANT FOR GLEW!!

    // PRINT OUT OPENGL VERSION IN USE
    std::cout << glGetString(GL_VERSION) << std::endl;

    this->window = window;
}

void Game::initGLEW()
{
    // INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR:GAME.CPP::GLEW_INIT_FAILED" << std::endl;
        glfwTerminate();
        exit(0);
    }
}

void Game::initOpenGLOptions()
{
    // enable the z depth
    glEnable(GL_DEPTH_TEST);

    // remove the non rendered face
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // remove the back face
    glFrontFace(GL_CCW); // the front face is counter-clockwise

    // enable color blending
    glEnable(GL_BLEND);
    // (As/kA,As/kA,As/kA,As/kA), (1,1,1,1)−(As/kA,As/kA,As/kA,As/kA)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // use GL_LINE when you just wanna draw the outlines
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // callback that changes the render mode when you press tab
    glfwSetKeyCallback(window, Game::changeRenderMode);
}

void Game::initMatrices()
{
    // not initializing as identity will cause a black screen.
    this->ViewMatrix = glm::mat4(1.f);
    this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

    // not initializing as identity will cause a black screen.
    this->ProjectionMatrix = glm::mat4(1.f);
    this->ProjectionMatrix = glm::perspective(
        glm::radians(this->fov),
        static_cast<float>(this->framebufferWidth) / static_cast<float>(this->framebufferHeight),
        this->nearPlane,
        this->farPlane);
}
// Constructors / Destructors
Game::Game(const char *title,
           const int height,
           const int width,
           int GL_VERSION_MAJOR,
           int GL_VERSION_MINOR,
           bool resizable)
    : WINDOW_HEIGHT(height), WINDOW_WIDTH(width), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR)
{
    // init variables
    this->window = nullptr;
    this->framebufferHeight = WINDOW_HEIGHT;
    this->framebufferWidth = WINDOW_WIDTH;

    this->worldUp = glm::vec3(0.f, 1.f, 0.f);     // straight up in y axis
    this->camFront = glm::vec3(0.f, 0.f, -1.f);   // straight ahead
    this->camPosition = glm::vec3(0.f, 0.f, 1.f); // set to origin as default

    this->fov = 90.f;       // field of view (degrees, to be converted later)
    this->nearPlane = 0.1f; // clip stuff slightly behind the camera, so we don't see it disappear
    this->farPlane = 1000.f;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
    this->initMatrices();
}

Game::~Game()

{
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

// Accessors

int Game::getWindowShouldClose()
{
    return glfwWindowShouldClose(this->window);
}

// Modifiers

void Game::setWindowShouldClose()
{
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//  Functions

void Game::update()
{
    glfwPollEvents();
    // Update Inputs
}

void Game::render()
{

    // DRAW CANVAS

    // CLEAR
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // select program

    // move, rotate and scale

    // update framebuffersize and projection matrix

    // activate texture

    // Bind vertex array object (data for trinangle) and DRAW

    // ENDDRAW

    // FORCE EXECUTION OF GL COMMANDS IN FINITE TIME
    glFlush();

    // reset all bindings to free up space
    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
}

// Static Functions
void Game::framebuffer_resize_callback(GLFWwindow *window, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
}

void Game::changeRenderMode(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        GLint polygonMode;
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

        switch (polygonMode)
        {
        case GL_LINE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case GL_FILL:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        default:
            break;
        }
    }
}
