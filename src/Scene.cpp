#define GLM_ENABLE_EXPERIMENTAL

#include "../include/Scene.h"
#include "../include/Camera.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "../include/ModelLibrary.h"

Scene::Scene(Shaders* shader, Shapes* shape, Camera* camera, SceneParameters* sceneParams, Texture* texture, MaterialLibrary* material,
             ModelLibrary* modelLib)
{
    m_shaders = shader;
    m_shapes = shape;
    m_camera = camera;
	m_sceneParams = sceneParams;
	m_lTexture = texture;
	m_materialLib = material;
	m_modelLib = modelLib;
}

void Scene::LoadTextures() const
{
	m_lTexture->LoadTexture(GL_TEXTURE_2D, "../resources/textures/redstone_lamp.jpg", "redstone");
	m_lTexture->LoadTexture(GL_TEXTURE_2D, "../resources/textures/CBCrate.jpg", "TTCashbot");
	m_lTexture->LoadTexture(GL_TEXTURE_2D, "../resources/textures/CBCrateSpecular.jpg", "TTCashbotSpecular");
	m_lTexture->LoadTexture(GL_TEXTURE_2D, "../resources/textures/steelcontainer.png", "container");
	m_lTexture->LoadTexture(GL_TEXTURE_2D, "../resources/textures/steelcontainer_specular.png", "containerSpecular");
	std::cout << std::endl;
}

void Scene::UseShader(const std::string& shader) const
{
	m_shaders->Use(shader);
	m_shaders->setMat4("projection", m_projection);
	m_shaders->setMat4("view", m_view);
	m_shaders->setVec3("viewPos", m_camera->Position);
}

void Scene::SetShaderTexture(const std::string& texTag, const std::string& specMapTag) const
{
	if (nullptr != m_shaders)
	{
		//m_shaders->setInt("bUseTexture", true);

		const int textureID = m_lTexture->FindTextureID(texTag);

		// Bind the texture to texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Tell shader to use texture unit 0
		m_shaders->setInt("material.texture1", 0);

		// if specular mapping enabled, find the ID of the map and set it to texture slot 1
		if (!specMapTag.empty())
		{
			const GLuint specMapID = m_lTexture->FindTextureID(specMapTag);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specMapID);

			m_shaders->setInt("material.specular1", 1);
			m_shaders->setInt("bUseSpecularMap", true); // enable flag
		}
		else // disable spec map flag
		{
			m_shaders->setInt("bUseSpecularMap", false);
		}
	}
}

void Scene::SetLights() const
{
	if (!m_sceneParams) return;

	unsigned int lightCount = 0;

	for (const auto& lightParam : m_sceneParams->lights)
	{
		LightObject obj;
		if (lightParam.enabled)
		{
			obj.lightType = lightParam.lightType;
			obj.distance = lightParam.distance;
			obj.lightPos = lightParam.position;
			obj.lightDirection = lightParam.direction;
			obj.ambientLight = lightParam.ambient;
			obj.diffuseLight = lightParam.diffuse;
			obj.specularLight = lightParam.specular;
		}
		else
		{
			obj.diffuseLight = glm::vec3(0.0f);
		}

		obj.SetLightUniforms(m_shaders, lightCount);
		lightCount++;
	}

	std::string base = "lights[" + std::to_string(lightCount) + "]";
	m_shaders->setVec3(base + ".ambient", 0.0f, 0.0f, 0.0f);
	m_shaders->setVec3(base + ".diffuse", 0.0f, 0.0f, 0.0f);
	m_shaders->setVec3(base + ".specular", 0.0f, 0.0f, 0.0f);
}

void Scene::Draw(SceneObject &object)
{
	if (object.object_mesh != AssimpModel)
	{
		if (!object.textureTag.empty())
			object.SetTexture(this);

		if (!object.materialTag.empty())
		{
			if (const Material* mat = m_materialLib->Find(object.materialTag))
				object.SetMaterial(mat);
		}
	}

	object.UpdateModelMatrix();
	object.SetModelMatrix();

	switch (object.object_mesh)
	{
	case Cube:
		m_shapes->DrawCubeMesh();
		break;
	case Plane:
		m_shapes->DrawPlaneMesh();
		break;
	case AssimpModel: if (object.assimpModel) object.assimpModel->Draw(object.shader); break;
	default:							   break;
	}
}

unsigned int cubemapID;

void Scene::PrepareScene()
{
	// get width/height of monitor
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const float WIDTH = mode->width;
	const float HEIGHT = mode->height;

	// translations
	m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -3.0f));
	m_projection = glm::perspective(glm::radians(ZOOM), WIDTH / HEIGHT, 0.1f, 100.f);

	// loading models


	m_shapes->LoadPlaneMesh();
	m_shapes->LoadCubeMesh();
	// load textures
	LoadTextures();
	// load materials
	m_materialLib->LoadDefaults();

	std::vector<std::string> faces;
	faces.push_back("../resources/textures/imageonline/right.png");
	faces.push_back("../resources/textures/imageonline/left.png");
	faces.push_back("../resources/textures/imageonline/top.png");
	faces.push_back("../resources/textures/imageonline/bottom.png");
	faces.push_back("../resources/textures/imageonline/front.png");
	faces.push_back("../resources/textures/imageonline/back.png");

	cubemapID = m_lTexture->LoadCubemap(faces);
}

void Scene::RenderScene()
{
	// get camera view matrix
	m_view = m_camera->GetViewMatrix();
	m_cameraPos = m_camera->Position;

	// set shaders
	UseShader("lighting");

	// set lights
	SetLights();

	// floor ====================================
	if (m_sceneParams->plane.visible) {
		SceneObject plane;
		plane.object_mesh        = Plane;
		plane.shader      = m_shaders;
		plane.materialTag = "darkGrayRubber";

		plane.position    = m_sceneParams->plane.position;
		plane.scale	      = m_sceneParams->plane.scale;
		plane.rotation    = m_sceneParams->plane.rotation;
		Draw(plane);
	}

	for (const auto& modelParam : m_sceneParams->models)
	{
		if (!modelParam.visible || modelParam.modelTag.empty())
			continue;

		SceneObject obj;
		obj.object_mesh = AssimpModel;
		obj.shader = m_shaders;
		obj.assimpModel = m_modelLib->GetModel(modelParam.modelTag);

		if (obj.assimpModel)
		{
			obj.position = modelParam.position;
			obj.rotation = modelParam.rotation;
			obj.scale = modelParam.scale;
			Draw(obj);
		}
	}

	// light cubes ==============================
	UseShader("lightCube");

	// back right light cube
	for (const auto& lightParam : m_sceneParams->lights)
	{
		if (!lightParam.enabled || lightParam.name.empty())
			continue;

		SceneObject obj;
		obj.object_mesh = Cube;
		obj.shader = m_shaders;
		obj.position = lightParam.position;
		obj.scale = glm::vec3(0.2f);
		Draw(obj);
	}

	// Draw skybox last
	glDepthFunc(GL_LEQUAL); // Change depth function so depth test passes when values are equal to depth buffer's content
	UseShader("skybox");

	// Remove translation from the view matrix
	glm::mat4 view = glm::mat4(glm::mat3(m_camera->GetViewMatrix()));
	m_shaders->setMat4("view", view);
	m_shaders->setMat4("projection", m_projection);

	// Bind cubemap texture
	glBindVertexArray(m_shapes->Cube.VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID); // Assuming you stored the ID here
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}