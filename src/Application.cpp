#include "include_libraries.h"

Vertex vertices[] = {
    // POSITION                  // COLOR                  // Texcoords
    glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),
    glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.8f, 1.f, 0.3f), glm::vec2(1.f, 1.f)};

unsigned int numberOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indeces[] = {
    0, 1, 2, 0, 2, 3};

unsigned int numberOfIndecs = sizeof(indeces) / sizeof(GLuint);

void updateInput(GLFWwindow *window, glm::vec3 &position, glm::vec3 &rotation, glm::vec3 &scale)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position.y += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position.y -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position.x -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position.x += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        rotation.y -= 1.f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        rotation.y += 1.f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        scale -= 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        scale += 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(0);
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

    // enabling this to get initial window size, before we hit the while loop.
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
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

    // ############## INPUT ASSEMBLY ###########################

    // set VertexAttributePointers and enable

    // you can use this instead of zero if the core_program is available. ^^
    // GLuint attribLocation = glGetAttribLocation(core_program, "vertex_position");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    // unbind everything

    glBindVertexArray(0);

    // ################## TEXTURES #############################

    // texture 0 init

    int image_width = 0;
    int image_height = 0;

    unsigned char *image = SOIL_load_image("images/db_trans.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

    // create and try to find the texture
    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    // repeat when texture doesn't cover required bounds
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // magnification and minifying image behaviour (type of scaling)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR:APPLICATION.CPP::FAILED_TO_LOAD_TEXTURE0" << std::endl;
    }

    // Remove all bound textures
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    // texture 1 init

    int image_width1 = 0;
    int image_height1 = 0;

    unsigned char *image1 = SOIL_load_image("images/container.jpg", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);

    // create and try to find the texture
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // repeat when texture doesn't cover required bounds
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // magnification and minifying image behaviour (type of scaling)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR:APPLICATION.CPP::FAILED_TO_LOAD_TEXTURE1" << std::endl;
    }

    // Remove all bound textures
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image1);

    // ############## TRANSFORMATIONS ##########################

    // rotate, scale and translate

    // not initializing as identity will cause a black screen.
    glm::mat4 ModelMatrix(1.f); // Identitiy Model Matrix

    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);

    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(position));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, scale);

    // camera configration

    glm::vec3 worldUpVector = glm::vec3(0.f, 1.f, 0.f);     // straight up in y axis
    glm::vec3 camFrontVector = glm::vec3(0.f, 0.f, -1.f);   // straight ahead
    glm::vec3 camPositionVector = glm::vec3(0.f, 0.f, 1.f); // set to origin as default

    // not initializing as identity will cause a black screen.
    glm::mat4 ViewMatrix(1.f);
    ViewMatrix = glm::lookAt(camPositionVector, camPositionVector + camFrontVector, worldUpVector);

    float fov = 90.f;       // field of view (degrees, to be converted later)
    float nearPlane = 0.1f; // clip stuff slightly behind the camera, so we don't see it disappear
    float farPlane = 1000.f;

    // not initializing as identity will cause a black screen.
    glm::mat4 ProjectionMatrix(1.f);
    ProjectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight),
        nearPlane,
        farPlane);

    glUseProgram(core_program);

    // pass in model matrix to vertex shader
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

    glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    glUseProgram(0);

    // ################# MAIN LOOP #############################

    while (!glfwWindowShouldClose(window))
    {

        // UPDATE
        updateInput(window, position, rotation, scale);
        // DRAW CANVAS

        // CLEAR
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // select program
        glUseProgram(core_program);

        // send uniforms (variables from cpu to gpu)
        glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
        glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

        // move, rotate and scale

        // rotation.x += 1;

        ModelMatrix = glm::mat4(1.f);

        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(position));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
        ModelMatrix = glm::scale(ModelMatrix, scale);

        glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        // in case of resize ( to prevent stretching )
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

        ProjectionMatrix = glm::perspective(
            glm::radians(fov),
            static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight),
            nearPlane,
            farPlane);

        glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

        // activate texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // Bind vertex array object (data for trinangle)

        glBindVertexArray(VAO);

        // DRAW

        // glDrawArrays() draw all arrays without index buffer
        glDrawElements(GL_TRIANGLES, numberOfIndecs, GL_UNSIGNED_INT, 0);

        // ENDDRAW
        glfwSwapBuffers(window);

        // FORCE EXECUTION OF GL COMMANDS IN FINITE TIME
        glFlush();

        // reset all bindings to free up space
        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glfwPollEvents();
    }

    // END OF PROGRAM
    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteProgram(core_program);
    return 0;
}
