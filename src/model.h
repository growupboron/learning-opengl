#pragma once

#include "mesh.h"
#include "material.h"
#include "shader.h"
#include "texture.h"

class Model
{
private:
    Material *material;
    std::vector<Mesh *> meshes;

    Texture *overrideTextureDiffuse;
    Texture *ovverideTextureSpecular;

    glm::vec3 position;

    // Functions

    void updateUniforms()
    {
    }

public:
    Model(
        glm::vec3 position,
        Material *material,
        Texture *overTexDiffuse,
        Texture *overTexSpec,
        std::vector<Mesh *> meshes)
    {
        this->position = position;
        this->material = material;
        this->ovverideTextureSpecular = overTexSpec;
        this->overrideTextureDiffuse = overTexDiffuse;

        for (auto *i : meshes)
        {
            this->meshes.push_back(new Mesh(*i));
        }

        for (auto&i : this->meshes){
            i->move(this->position);
            i->setOrigin(this->position);
        }
    }

    ~Model()
    {
        for (auto *&i : this->meshes)
        {
            delete i;
        }
    }

    // Functions

    void rotate(const glm::vec3 rotation){
            for(auto&i : this->meshes){
                i->rotate(rotation);
            }
    }



    void update()
    {
    }

    void render(Shader *shader)
    {
        this->updateUniforms();
        // update uniforms
        shader->use();

        this->material->sendToShader(*shader);
        // select program

        // activate texture
        this->overrideTextureDiffuse->bind(0);
        this->ovverideTextureSpecular->bind(1);


        for(auto&i : this->meshes){
            i->render(shader);
        }
        
    }
};