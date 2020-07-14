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
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK); // remove the back face
    glFrontFace(GL_CCW); // the front face is counter-clockwise

    // enable color blending
    glEnable(GL_BLEND);
    // (As/kA,As/kA,As/kA,As/kA), (1,1,1,1)−(As/kA,As/kA,As/kA,As/kA)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // use GL_LINE when you just wanna draw the outlines
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // callback that changes the render mode when you press tab
    glfwSetKeyCallback(window, Game::changeRenderMode);

    // input

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

void Game::initShaders()
{
    this->shaders.push_back(
        new Shader((char *)"res/shaders/vertex_core.glsl",
                   (char *)"res/shaders/fragment_core.glsl",
                   this->GL_VERSION_MAJOR,
                   this->GL_VERSION_MINOR));
}

void Game::initTextures()
{
    // texture 0
    this->textures.push_back(new Texture("images/db_trans.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("images/db_trans_specular.png", GL_TEXTURE_2D));

    // texture 1
    this->textures.push_back(new Texture("images/texture.jpg", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("images/texture_specular.jpg", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
    glm::vec3 ambientColor(0.4f);
    glm::vec3 diffuseColor(1.f);
    glm::vec3 specularColor(1.f);

    this->materials.push_back(
        new Material(ambientColor,
                     diffuseColor,
                     specularColor,
                     0,
                     1));
}

void Game::initModels()

{

    std::vector<Mesh *> meshes;

    Quad tempQuad = Quad();
    meshes.push_back(new Mesh(&tempQuad, glm::vec3(1.f, 0.f, 0.f),
                              glm::vec3(0.f),
                              glm::vec3(0.f),
                              glm::vec3(1.f)));
    Pyramid tempPym = Pyramid();
    meshes.push_back(new Mesh(&tempPym, glm::vec3(0.f, 0.f, 0.f),
                              glm::vec3(0.f),
                              glm::vec3(0.f),
                              glm::vec3(1.f)));

    this->models.push_back(new Model(
        glm::vec3(0.f),
        this->materials[MAT_1],
        this->textures[TEX_CONTAINER],
        this->textures[TEX_CONTAINER_SPECULAR],
        meshes));
    this->models.push_back(new Model(
        glm::vec3(0.f, 1.f, 1.f),
        this->materials[MAT_1],
        this->textures[TEX_PUSHEEN],
        this->textures[TEX_PUSHEEN_SPECULAR],
        meshes));
    this->models.push_back(new Model(
        glm::vec3(2.f, 0.f, 2.f),
        this->materials[MAT_1],
        this->textures[TEX_CONTAINER],
        this->textures[TEX_CONTAINER_SPECULAR],
        meshes));

    for (auto *&i : meshes)
    {
        delete i;
    }
}

void Game::initLights()
{
    this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::initUniforms()
{
    this->shaders[SHADER_CORE_PROGRAM]->use();
    // pass in model matrix to vertex shader
    this->shaders[SHADER_CORE_PROGRAM]->setMat4v(this->ViewMatrix, "ViewMatrix", GL_FALSE);
    this->shaders[SHADER_CORE_PROGRAM]->setMat4v(this->ProjectionMatrix, "ProjectionMatrix", GL_FALSE);

    // // pass in the lighting data
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
    this->shaders[SHADER_CORE_PROGRAM]->unuse();
}

void Game::updateUniforms()
{
    this->shaders[SHADER_CORE_PROGRAM]->use();

    // send uniforms (variables from cpu to gpu)
    this->shaders[SHADER_CORE_PROGRAM]->set1i(0, "texture0");
    this->shaders[SHADER_CORE_PROGRAM]->set1i(1, "texture1");

    // in case of resize ( to prevent stretching )
    glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

    // update the cam uniform
    this->ViewMatrix = this->camera.getViewMatrix();
    this->shaders[SHADER_CORE_PROGRAM]->setMat4v(this->ViewMatrix, "ViewMatrix", GL_FALSE);
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

    this->ProjectionMatrix = glm::perspective(
        glm::radians(this->fov),
        static_cast<float>(this->framebufferWidth) / static_cast<float>(this->framebufferHeight),
        this->nearPlane,
        this->farPlane);

    this->shaders[SHADER_CORE_PROGRAM]->setMat4v(this->ProjectionMatrix, "ProjectionMatrix", GL_FALSE);
    this->shaders[SHADER_CORE_PROGRAM]->unuse();
}
// Constructors / Destructors
Game::Game(const char *title,
           const int width,
           const int height,
           int GL_VERSION_MAJOR,
           int GL_VERSION_MINOR,
           bool resizable)
    : WINDOW_HEIGHT(height),
      WINDOW_WIDTH(width),
      GL_VERSION_MAJOR(GL_VERSION_MAJOR),
      GL_VERSION_MINOR(GL_VERSION_MINOR),
      camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
    // init variables
    this->window = nullptr;
    this->framebufferHeight = WINDOW_HEIGHT;
    this->framebufferWidth = WINDOW_WIDTH;

    this->worldUp = glm::vec3(0.f, 1.f, 0.f);     // straight up in y axis
    this->camFront = glm::vec3(0.f, 0.f, -1.f);   // straight ahead
    this->camPosition = glm::vec3(0.f, 0.f, 1.f); // set to origin as default

    this->fov = 60.f;       // field of view (degrees, to be converted later)
    this->nearPlane = 0.1f; // clip stuff slightly behind the camera, so we don't see it disappear
    this->farPlane = 1000.f;

    this->dt = 0.0f;
    this->curTime = 0.f;
    this->lastTime = 0.f;

    this->lastMouseX = 0.0;
    this->lastMouseY = 0.0;
    this->mouseX = 0.0;
    this->mouseY = 0.0;
    this->mouseOffsetX = 0.0;
    this->mouseOffsetY = 0.0;

    // Important for initialization, to avoid zero division errors.
    this->firstMouse = true;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
    this->initMatrices();
    this->initShaders();
    this->initTextures();
    this->initMaterials();
    this->initModels();
    this->initLights();
    this->initUniforms();
}

Game::~Game()

{
    glfwDestroyWindow(this->window);
    glfwTerminate();

    for (size_t i = 0; i < this->shaders.size(); i++)
        delete this->shaders[i];

    for (size_t i = 0; i < this->textures.size(); i++)
        delete this->textures[i];

    for (size_t i = 0; i < this->materials.size(); i++)
        delete this->materials[i];
    for (auto *&i : this->models)
        delete i;
    for (size_t i = 0; i < this->lights.size(); i++)
        delete this->lights[i];
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
    this->updateDeltaTime();

    this->updateKeyboardInput();
    this->updateMouseInput();

    this->camera.updateInput(this->dt, -1, this->mouseOffsetX, this->mouseOffsetY);

    this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
    this->models[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
    this->models[2]->rotate(glm::vec3(0.f, 1.f, 0.f));

    std::cout << "FPS : " << 1 / this->dt << "\n";
    // Update Inputs
}

void Game::render()
{

    // DRAW CANVAS

    // CLEAR
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // update uniforms

    this->updateUniforms();
    // this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
    // // select program

    // // activate texture
    // this->textures[TEX_CONTAINER]->bind(0);
    // this->textures[TEX_CONTAINER_SPECULAR]->bind(1);

    // this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);

    // Render models
    for (auto &i : models)
    {
        i->render(this->shaders[SHADER_CORE_PROGRAM]);
    }

    // ENDDRAW
    glfwSwapBuffers(window);

    // FORCE EXECUTION OF GL COMMANDS IN FINITE TIME
    glFlush();

    // reset all bindings to free up space (redundant)
    glBindVertexArray(0);
    this->shaders[SHADER_CORE_PROGRAM]->unuse();
    this->textures[TEX_PUSHEEN]->unbind();
    this->textures[TEX_CONTAINER]->unbind();
}

void Game::updateKeyboardInput()
{
    if (glfwGetKey(this->window, GLFW_KEY_HOME) == GLFW_PRESS)
    {
        this->camera.move(this->dt, FORWARD);
    }

    if (glfwGetKey(this->window, GLFW_KEY_END) == GLFW_PRESS)
    {
        this->camera.move(this->dt, BACKWARD);
    }
    if (glfwGetKey(this->window, GLFW_KEY_DELETE) == GLFW_PRESS)
    {
        this->camera.move(this->dt, LEFT);
    }
    if (glfwGetKey(this->window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        this->camera.move(this->dt, RIGHT);
    }
    if (glfwGetKey(this->window, GLFW_KEY_INSERT) == GLFW_PRESS)
    {
        this->camera.move(this->dt, FORWARD);
    }
    if (glfwGetKey(this->window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        this->camera.move(this->dt, FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(0);
    }
}

void Game::updateMouseInput()
{
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

    if (this->firstMouse)
    {
        this->lastMouseX = this->mouseX;
        this->lastMouseY = this->mouseY;
        this->firstMouse = false;
    }

    // Calculate offset since last frame
    this->mouseOffsetX = this->mouseX - this->lastMouseX;
    this->mouseOffsetY = this->lastMouseY - this->mouseY;

    // Set last X and Y
    this->lastMouseX = this->mouseX;
    this->lastMouseY = this->mouseY;
}

void Game::updateDeltaTime()
{
    this->curTime = static_cast<float>(glfwGetTime());
    this->dt = this->curTime - this->lastTime;
    this->lastTime = this->curTime;

    // To get the time between frames.
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

void Game::oldUpdateInput(GLFWwindow *window, Mesh &mesh)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, +0.01f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, -0.01f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(-0.01f, 0.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.01f, 0.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mesh.rotate(glm::vec3(0.f, -1.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        mesh.rotate(glm::vec3(0.f, 1.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        mesh.rotate(glm::vec3(-1.f, 0.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        mesh.rotate(glm::vec3(1.f, 0.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, 0.f, -0.05f));
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, 0.f, +0.05f));
    }
}
