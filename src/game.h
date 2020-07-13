#pragma once
#include "include_libraries.h"

// Enumerations
enum shader_enum
{
    SHADER_CORE_PROGRAM = 0
};

enum texture_enum
{
    TEX_PUSHEEN = 0,
    TEX_PUSHEEN_SPECULAR = 1,
    TEX_CONTAINER = 2,
    TEX_CONTAINER_SPECULAR = 3
};

enum material_enum
{
    MAT_1 = 0
};

enum mesh_enum
{
    MESH_QUAD = 0,
    MESH_TRIANGLE = 1,
};

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

    // Delta Time
    float dt;
    float curTime;
    float lastTime;

    // Mouse input
    double lastMouseX;
    double lastMouseY;
    double mouseX;
    double mouseY;
    double mouseOffsetX;
    double mouseOffsetY;
    bool firstMouse;

    // matrices
    glm::mat4 ViewMatrix;
    glm::vec3 camPosition;
    glm::vec3 worldUp;
    glm::vec3 camFront;

    glm::mat4 ProjectionMatrix;
    float fov;
    float nearPlane;
    float farPlane;

    // Shaders
    std::vector<Shader *> shaders;

    // Textures
    std::vector<Texture *> textures;

    // Material
    std::vector<Material *> materials;

    // Meshes
    std::vector<Mesh *> meshes;

    // Lights (positions)
    std::vector<glm::vec3 *> lights;

    // private functions
    void initGLFW();
    void initWindow(const char *title, bool resizable);
    void initGLEW(); // AFTER CONTEXT CREATION !!
    void initOpenGLOptions();
    void initMatrices();
    void initShaders();
    void initTextures();
    void initMaterials();
    void initMeshes();
    void initLights();
    void initUniforms();

    void updateUniforms();

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

    void updateDeltaTime();
    void updateKeyboardInput();
    void updateMouseInput();

    // Static Functions
    static void framebuffer_resize_callback(GLFWwindow *window, int fbW, int fbH);
    static void changeRenderMode(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void oldUpdateInput(GLFWwindow *window, Mesh &mesh);
};