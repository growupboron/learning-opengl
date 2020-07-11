#include "include_libraries.h"
#include "shader.h"
#include "texture.h"
#include "material.h"

Vertex vertices[] = {
    // POSITION                  // COLOR                  // Texcoords
    glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.8f, 1.f, 0.3f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, -1.f)};

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
        position.z -= 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        position.z += 0.05f;
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

int main()
{

    // INITIALIZE GLFW LIBRARY

    if (!glfwInit())
    {
        std::cout << "ERROR:APPLICATION.CPP::GLFW_COULD_NOT_BE_INITIALIZED" << std::endl;
        return -1;
    }

    // CREATE WINDOW

    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
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
    Shader core_program((char *)"res/shaders/vertex_core.glsl", (char *)"res/shaders/fragment_core.glsl");
    // GLuint core_program;
    // if (!loadShaders(core_program))
    // {
    //     glfwTerminate();
    // }

    // ############## VAO, VBO, EBO ###########################


    // Model Mesh


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

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    // unbind everything

    glBindVertexArray(0);

    // ################## TEXTURES #############################

 
    Texture texture_0("images/db_trans.png",GL_TEXTURE_2D,0);
    Texture texture_1("images/container.jpg", GL_TEXTURE_2D,1);
    
    
    
    // ################ MATERIALS ##############################
    glm::vec3 ambientColor(0.1f);
    glm::vec3 diffuseColor(1.f);
    glm::vec3 specularColor(1.f);

    Material material0(ambientColor,diffuseColor, specularColor, texture_0.getTextureUnit(), texture_1.getTextureUnit());
    
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

    // ################### LIGTHING ###################

    glm::vec3 lightPos0(0.f, 0.f, 1.f);
    core_program.use();
    // pass in model matrix to vertex shader
    core_program.setMat4v(ModelMatrix, "ModelMatrix", GL_FALSE);
    core_program.setMat4v(ViewMatrix, "ViewMatrix", GL_FALSE);
    core_program.setMat4v(ProjectionMatrix, "ProjectionMatrix", GL_FALSE);

    // // pass in the lighting data
    core_program.setVec3f(lightPos0, "lightPos0");
    core_program.setVec3f(camPositionVector, "cameraPos");
    core_program.unuse();
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
        core_program.use();
        // send uniforms (variables from cpu to gpu)
        core_program.set1i(texture_0.getTextureUnit(), "texture0");
        core_program.set1i(texture_1.getTextureUnit(), "texture1");

        material0.sendToShader(core_program);


        // move, rotate and scale

        // rotation.x += 1;

        ModelMatrix = glm::mat4(1.f);

        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(position));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
        ModelMatrix = glm::scale(ModelMatrix, scale);

        core_program.setMat4v(ModelMatrix, "ModelMatrix");

        // in case of resize ( to prevent stretching )
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

        ProjectionMatrix = glm::perspective(
            glm::radians(fov),
            static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight),
            nearPlane,
            farPlane);

        core_program.setMat4v(ViewMatrix, "ViewMatrix", GL_FALSE);
        core_program.setMat4v(ProjectionMatrix, "ProjectionMatrix", GL_FALSE);

        // activate texture
        texture_0.bind(); 
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture0);

        texture_1.bind();

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
        core_program.unuse();
        texture_1.unbind();
        texture_0.unbind();

        glfwPollEvents();
    }

    // END OF PROGRAM
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
