#pragma once

#include <iostream>

#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL2.h>

class Texture
{
private:
    GLuint id;
    int width;
    int height;
    int type;

public:
    Texture(const char *filename, GLenum type)
    {
        this->type = type;
        
        unsigned char *image = SOIL_load_image(filename, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

        // create and try to find the texture

        glGenTextures(1, &this->id);
        glBindTexture(type, this->id);

        // repeat when texture doesn't cover required bounds
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // magnification and minifying image behaviour (type of scaling)
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        if (image)
        {
            glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(type);
        }
        else
        {
            std::cout << "ERROR:APPLICATION.CPP::FAILED_TO_LOAD_TEXTURE: " << filename << std::endl;
        }

        // Remove all bound textures
        glActiveTexture(0);
        glBindTexture(type, 0);
        SOIL_free_image_data(image);
    }
    ~Texture()
    {
        glDeleteTextures(1, &this->id);
    }

    inline GLuint getId() const { return this->id; }
    
    void bind(const GLint textureUnit)
    {

        glActiveTexture(GL_TEXTURE0 + textureUnit);

        glBindTexture(this->type, this->id);
    }

    void unbind()
    {
        glActiveTexture(0);
        glBindTexture(this->type, 0);
    }

    void loadFromFile(const char *filename)
    {
        unsigned char *image = SOIL_load_image(filename, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

        if (this->id)
        {
            glDeleteTextures(1, &this->id);
        }
        glGenTextures(1, &this->id);
        glBindTexture(this->type, this->id);

        // repeat when texture doesn't cover required bounds
        glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // magnification and minifying image behaviour (type of scaling)
        glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        if (image)
        {
            glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(this->type);
        }
        else
        {
            std::cout << "ERROR:APPLICATION.CPP::FAILED_TO_LOAD_TEXTURE: " << filename << std::endl;
        }

        // Remove all bound textures
        glActiveTexture(0);
        glBindTexture(this->type, 0);
        SOIL_free_image_data(image);
    }
};
