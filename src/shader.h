#pragma once

#include <iostream>

#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class Shader
{
private:
    GLuint id; // program id

    std::string loadShaderSource(char *fileName);

    GLuint loadShader(GLenum type, char *fileName);

    void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);

public:
    Shader(char *vertexFile, char *fragmentFile, char *geometryFile = (char *)"")
    {
        // init
        GLuint vertexShader = 0;
        GLuint geometryShader = 0;
        GLuint fragmentShader = 0;

        // load
        vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

        if (geometryFile != "")
            geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);

        fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

        // link

        this->linkProgram(vertexShader, geometryShader, fragmentShader);

        // destroy unused
        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    };
    ~Shader() { glDeleteProgram(this->id); };

    void use()
    {
        glUseProgram(this->id);
    }

    void unuse()
    {
        glUseProgram(0);
    }
    
    void set1i(GLint value, const GLchar *name)
    {
        glUniform1i(glGetUniformLocation(this->id, name), value);
    }

    void set1f(GLfloat value, const GLchar *name)
    {
        glUniform1f(glGetUniformLocation(this->id, name), value);
    }

    void setVec2f(glm::fvec2 value, const GLchar *name)
    {
        glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

    }

    void setVec3f(glm::fvec3 value, const GLchar *name)
    {
        glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
    }
    
    void setVec4f(glm::fvec4 value, const GLchar *name)
    {
        glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
    }
    
    void setMat3v(glm::mat3 value, const GLchar *name, GLboolean transpose = GL_FALSE)
    {
        glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
    }
    
    void setMat4v(glm::mat4 value, const GLchar *name, GLboolean transpose = GL_FALSE)
    {
        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
    }
};

std::string Shader::loadShaderSource(char *fileName)

{

    std::string currentLine = "";
    std::string finalsrc = "";

    std::ifstream input_file;

    input_file.open(fileName);

    if (input_file.is_open())
    {
        while (std::getline(input_file, currentLine))
        {
            finalsrc += currentLine + "\n";
        }
    }
    else
        std::cout << "ERROR:SHADER::COULD_NOT_OPEN_SHADER_FILE: " << fileName << std::endl;

    input_file.close();
    return finalsrc;
}

GLuint Shader::loadShader(GLenum type, char *fileName)
{

    char infoLog[512];
    GLint compileResult;

    GLuint theShader = glCreateShader(type);

    std::string shaderSrc = this->loadShaderSource(fileName);
    const GLchar *theShadersrc = shaderSrc.c_str();

    glShaderSource(theShader, 1, &theShadersrc, NULL);
    glCompileShader(theShader);

    // error checking

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &compileResult);
    if (!compileResult)
    {
        glGetShaderInfoLog(theShader, 512, NULL, infoLog);
        std::cout << "ERROR:LOADSHADERS::COULD_NOT_COMPILE_SHADER: " << fileName << std::endl;
        std::cout << infoLog << std::endl;
    }
    return theShader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
{

    this->id = glCreateProgram();

    glAttachShader(this->id, vertexShader);

    if (geometryShader)
        glAttachShader(this->id, geometryShader);

    glAttachShader(this->id, fragmentShader);
    glLinkProgram(this->id);

    // error checking

    char infoLog[512];
    GLint compileResult;

    glGetProgramiv(this->id, GL_LINK_STATUS, &compileResult);

    if (!compileResult)
    {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::COULD_NOT_LINK_PROGRAM" << std::endl;
        std::cout << infoLog << std::endl;
    }

    // unlink program
    glUseProgram(0);
}
