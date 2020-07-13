#pragma once
#include <iostream>
#include <vector>
#include "vertex.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Primitive
{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indeces;

public:
    Primitive() {}
    virtual ~Primitive() {}

    // Functions

    void set(
        const Vertex *vertices,
        const unsigned noOfVertices,
        const GLuint *indeces,
        const unsigned noOfindeces)
    {
        for (size_t i = 0; i < noOfVertices; i++)
        {
            this->vertices.push_back(vertices[i]);
        }

        for (size_t i = 0; i < noOfindeces; i++)
        {
            this->indeces.push_back(indeces[i]);
        }
    }

    inline Vertex *getVertices() { return this->vertices.data(); };

    inline GLuint *getIndeces() { return this->indeces.data(); };

    inline const unsigned getNoOfVertices() { return this->vertices.size(); };

    inline const unsigned getNoOfIndeces() { return this->indeces.size(); };
};

class Quad : public Primitive
{
public:
    Quad()
        : Primitive()
    {
        Vertex vertices[] = {
            // POSITION                  // COLOR                  // Texcoords
            glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.8f, 1.f, 0.3f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)};

        unsigned int numberOfVertices = sizeof(vertices) / sizeof(Vertex);

        GLuint indeces[] = {
            0, 1, 2, 0, 2, 3};

        unsigned int numberOfIndecs = sizeof(indeces) / sizeof(GLuint);

        this->set(vertices, numberOfVertices, indeces, numberOfIndecs);
    }
};

class Triangle : public Primitive
{
public:
    Triangle()
        : Primitive()
    {
        Vertex vertices[] = {
            // POSITION                  // COLOR                  // Texcoords
            glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.8f, 1.f, 0.3f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)};

        unsigned int numberOfVertices = sizeof(vertices) / sizeof(Vertex);

        GLuint indeces[] = {
            0, 1, 2};

        unsigned int numberOfIndecs = sizeof(indeces) / sizeof(GLuint);

        this->set(vertices, numberOfVertices, indeces, numberOfIndecs);
    }
};

class Pyramid : public Primitive
{
public:
    Pyramid()
        : Primitive()
    {
        Vertex vertices[] = {
            // Triangle Front
            // POSITION                  // COLOR                  // Texcoords
            glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f),

            // Triangle left
            // POSITION                  // COLOR                  // Texcoords
            glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
            glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(-1.f, 0.f, 0.f),

            // Triangle back
            // POSITION                  // COLOR                  // Texcoords
            glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, -1.f),

            // Triangle right
            // POSITION                  // COLOR                  // Texcoords
            glm::vec3(0.f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(1.f, 0.f, 0.f),
            glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(1.f, 0.f, 0.f)

        };

        unsigned int numberOfVertices = sizeof(vertices) / sizeof(Vertex);

        this->set(vertices, numberOfVertices, nullptr, 0);
    }
};
