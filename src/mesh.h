#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "vertex.h"
#include "texture.h"
#include "material.h"
#include "primitives.h"

class Mesh
{
private:
    unsigned noOfVertices;
    unsigned noOfIndeces;
    Vertex *vertexArray;
    GLuint *indexArray;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 origin;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 ModelMatrix;

    void initVAO()
    {
        // Create VAO
        glCreateVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->noOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->noOfIndeces * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);

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
    }

    void updateUniforms(Shader *shader)
    {
        shader->setMat4v(this->ModelMatrix, "ModelMatrix");
    }

public:
    Mesh(
        Vertex *vertexArray,
        const unsigned &noOfVertices,
        GLuint *indexArray,
        const unsigned &noOfIndecs,
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 origin = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f))
    {
        this->origin = origin;
        this->ModelMatrix = glm::mat4(1.f);
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;

        this->noOfVertices = noOfVertices;
        this->noOfIndeces = noOfIndecs;

        this->vertexArray = new Vertex[this->noOfVertices];

        for (size_t i = 0; i < this->noOfVertices; i++)
        {
            this->vertexArray[i] = vertexArray[i];
        }

        this->indexArray = new GLuint[this->noOfIndeces];

        for (size_t i = 0; i < this->noOfIndeces; i++)
        {
            this->indexArray[i] = indexArray[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    Mesh(
        Primitive *primitive,
        glm::vec3 position = glm::vec3(0.f),
        glm::vec3 origin = glm::vec3(0.f),
        glm::vec3 rotation = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f))
    {

        this->ModelMatrix = glm::mat4(1.f);
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        this->origin = origin;

        this->noOfVertices = primitive->getNoOfVertices();
        this->noOfIndeces = primitive->getNoOfIndeces();

        this->vertexArray = new Vertex[this->noOfVertices];

        for (size_t i = 0; i < this->noOfVertices; i++)
        {
            this->vertexArray[i] = primitive->getVertices()[i];
        }

        this->indexArray = new GLuint[this->noOfIndeces];

        for (size_t i = 0; i < this->noOfIndeces; i++)
        {
            this->indexArray[i] = primitive->getIndeces()[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);

        delete[] this->vertexArray;
        delete[] this->indexArray;
    }

    Mesh(const Mesh &obj)
    {
        this->ModelMatrix = glm::mat4(1.f);
        this->position = obj.position;
        this->rotation = obj.rotation;
        this->scale = obj.scale;
        this->origin = obj.origin;

        this->noOfVertices = obj.noOfVertices;
        this->noOfIndeces = obj.noOfIndeces;

        this->vertexArray = new Vertex[this->noOfVertices];

        for (size_t i = 0; i < this->noOfVertices; i++)
        {
            this->vertexArray[i] = obj.vertexArray[i];
        }

        this->indexArray = new GLuint[this->noOfIndeces];

        for (size_t i = 0; i < this->noOfIndeces; i++)
        {
            this->indexArray[i] = obj.indexArray[i];
        }

        this->initVAO();
        this->updateModelMatrix();
    }

    // Modifiers
    void setOrigin(const glm::vec3 origin){
        this->origin = origin;
    }

    void setPosition(const glm::vec3 position)
    {
        this->position = position;
    }
    void setRotation(const glm::vec3 rotation)
    {
        this->rotation = rotation;
    }
    void setScale(const glm::vec3 scale)
    {
        this->scale = scale;
    }

    // Functions
    void move(const glm::vec3 position)
    {
        this->position += position;
    }

    void rotate(const glm::vec3 rotation)
    {
        this->rotation += rotation;
    }

    void scaleUpd(const glm::vec3 scale)
    {
        this->scale += scale;
    }

    void update()
    {
    }

    void render(Shader *shader)
    {
        this->updateModelMatrix();
        this->updateUniforms(shader);
        shader->use();

        // Bind vertex array object (data for trinangle)
        glBindVertexArray(this->VAO);

        // DRAW

        // glDrawArrays() draw all arrays without index buffer
        if (this->noOfIndeces == 0)
        {
            glDrawArrays(GL_TRIANGLES, 0, this->noOfVertices);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, this->noOfIndeces, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void updateModelMatrix()
    {
        this->ModelMatrix = glm::mat4(1.f);

        this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
        
        this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
        
        this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
    }
};
