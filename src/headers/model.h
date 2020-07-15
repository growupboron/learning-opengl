#pragma once

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "material.h"
#include "objectLoader.h"
class Model
{
private:
	Material *material;
	Texture *overrideTextureDiffuse;
	Texture *overrideTextureSpecular;
	std::vector<Mesh *> meshes;
	glm::vec3 position;

	void updateUniforms()
	{
	}

public:
	Model(
		glm::vec3 position,
		Material *material,
		Texture *orTexDif,
		Texture *orTexSpec,
		std::vector<Mesh *> &meshes)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		for (auto *i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}

		for (auto &i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	Model(
		glm::vec3 position,
		glm::vec3 rotation,
		glm::vec3 scale,
		Material *material,
		Texture *orTexDif,
		Texture *orTexSpec,
		const char *objFile)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;

		std::vector<Vertex> mesh = loadObjFile(objFile);
		this->meshes.push_back(
			new Mesh(
				mesh.data(),
				mesh.size(),
				NULL,
				0, glm::vec3(0.f)));

		for (auto &i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}

		this->rotate(rotation);
		this->scaleUp(scale);
	}

	~Model()
	{
		for (auto *&i : this->meshes)
			delete i;
	}

	//Functions
	void rotate(const glm::vec3 rotation)
	{
		for (auto &i : this->meshes)
			i->rotate(rotation);
	}

	void move(const glm::vec3 direction)
	{
		for (auto &i : this->meshes)
			i->move(direction);
	}

	void scaleUp(const glm::vec3 scale)
	{
		for (auto &i : this->meshes)
			i->scaleUp(scale);
	}

	void update()
	{
	}

	void render(Shader *shader)
	{
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Use a program
		shader->use();

		//Activate texture

		//Draw
		for (auto &i : this->meshes)
		{
			this->overrideTextureDiffuse->bind(0);
			this->overrideTextureSpecular->bind(1);
			i->render(shader);
		}
	}
};