#include "libs.h"

Vertex vertices[] = {
    // POSITION                         // COLOR                        // Texcoords
    glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f)

};

unsigned int numberOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indeces[] = {
    0, 1, 2};

unsigned int numberOfIndecs = sizeof(indeces) / sizeof(GLuint);

void updateInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_resize_callback(GLFWwindow *window, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
}

// LOADING THE VERTEX AND FRAGMENT SHADERS
bool loadShaders(GLuint &program)
{
    bool loadSuccess = true;
    //  common error checking variables
    char infoLog[512];
    GLint compileResult;

    std::string vertex_filepath = "res/shaders/vertex_core.glsl";
    std::string fragment_filepath = "res/shaders/fragment_core.glsl";

    std::string currentLine = "";
    std::string finalsrc = "";

    std::ifstream input_file1;

    // ##### VERTEX SHADER ##########

    // read source

    input_file1.open(vertex_filepath);

    if (input_file1.is_open())
    {
        while (std::getline(input_file1, currentLine))
        {
            finalsrc += currentLine + "\n";
        }
    }
    else
    {
        std::cout << "ERROR:APPLICATION.CPP::COULD_NOT_OPEN_VERTEX_SHADER_FILE" << std::endl;
        loadSuccess = false;
    }
    input_file1.close();

    // create shader

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertSrc = finalsrc.c_str();

    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);

    // error checking

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileResult);
    if (!compileResult)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR:APPLICATION.CPP::VERTEX_SHADER_COMPILE_FAILED" << std::endl;
        std::cout << infoLog << std::endl;
        loadSuccess = false;
    }

    // ########### FRAGMENT SHADER ##########333

    currentLine = "";
    finalsrc = "";

    // read source

    std::ifstream input_file2;
    input_file2.open(fragment_filepath);

    if (input_file2.is_open())
    {
        while (std::getline(input_file2, currentLine))
        {
            finalsrc += currentLine + "\n";
        }
    }
    else
    {
        std::cout << "ERROR:APPLICATION.CPP::COULD_NOT_OPEN_FRAGMENT_SHADER_FILE" << std::endl;
        loadSuccess = false;
    }
    input_file2.close();

    // create shader

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragSrc = finalsrc.c_str();

    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    // error checking

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileResult);
    if (!compileResult)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR:APPLICATION.CPP::FRAGMENT_SHADER_COMPILE_FAILED" << std::endl;
        std::cout << infoLog << std::endl;
        loadSuccess = false;
    }

    // ########### PROGRAM #################

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &compileResult);

    if (!compileResult)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR:APPLICATION.CPP::COULD_NOT_LINK_PROGRAM" << std::endl;
        std::cout << infoLog << std::endl;
        loadSuccess = false;
    }

    // Free up memory used by indivdual shaders

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return loadSuccess;
}

int main()
{

    // INITIALIZE GLFW LIBRARY

    if (!glfwInit())
    {
        std::cout << "ERROR:APPLICATION.CPP::GLFW_COULD_NOT_BE_INITIALIZED" << std::endl;
        return -1;
    }

    // CREATE WINDOW

    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480;
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // SET OPENGL VERSION MAJOR.MINOR
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    // SET WHETHER OPENGL IS RESIZABLE OR NOT
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // macOS
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learnin' OpenGL", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        std::cout << "ERROR:APPLICATION.CPP::GLFW_WINDOW_COULD_NOT_BE_CREATED" << std::endl;
        return -1;
    }

    // FOR RESIZABLE WINDOWS
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    // FOR NON RESIZABLE WINDOWS
    // glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    // glViewport(0, 0, framebufferWidth, framebufferHeight);

    glfwMakeContextCurrent(window); // IMPORTANT FOR GLEW!!

    // PRINT OUT OPENGL VERSION IN USE
    std::cout << glGetString(GL_VERSION) << std::endl;

    // INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR:APPLICATION.CPP::GLEW_INIT_FAILED" << std::endl;
        glfwTerminate();
    }

    // ############### OPENGL OPTIONS ########################

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

    // ################## SHADERS ############################

    GLuint core_program;
    if (!loadShaders(core_program))
    {
        glfwTerminate();
    }

    // ############## VAO, VBO, EBO ###########################

    GLuint VAO; // Vertex Array Buffer
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO; // Vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO; // Element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);



    // ################# MAIN LOOP #############################

    while (!glfwWindowShouldClose(window))
    {

        // UPDATE
        updateInput(window);
        // DRAW CANVAS

        // CLEAR
        glClearColor(1.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // DRAW

        // ENDDRAW
        glfwSwapBuffers(window);

        // FORCE EXECUTION OF GL COMMANDS IN FINITE TIME
        glFlush();

        glfwPollEvents();
    }

    // END OF PROGRAM
    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteProgram(core_program);
    return 0;
}
