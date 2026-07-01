#ifndef HIRAETHENGINE_SCENE_H
#define HIRAETHENGINE_SCENE_H
#include "Camera.h"
#include "SceneParameters.h"
#include "Shaders.h"
#include "Shapes.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"

class ModelLibrary;

class Scene
{

public:
	enum ObjectMeshType
	{
		Cube,
		Plane,
		AssimpModel
	};

	struct SceneObject
	{
		// Transform
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale    = glm::vec3(1.0f);

		std::string materialTag = "";
		ObjectMeshType object_mesh;

		// Cached model matrix
		glm::mat4 model = glm::mat4(1.0f);

		// Shader reference (optional, could be passed to Draw())
		Shaders* shader = nullptr;

		Model* assimpModel = nullptr;

		// Texture parameters
		bool useDiffuseMap = false;
		bool useSpecularMap = false;
		std::string textureTag = "";
		std::string specMapTag = "";

		void SetTexture(Scene* scene)
		{
			useDiffuseMap = !textureTag.empty();
			useSpecularMap = !specMapTag.empty();

			scene->SetShaderTexture(textureTag, specMapTag);
		}

		void UpdateModelMatrix()
		{
			model = glm::mat4(1.0f);  // RESET to identity first!
			model = glm::translate(model, position);
			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, scale);

			if (shader)
			{
				shader->setInt("bUseDiffuseMap", useDiffuseMap);
				shader->setInt("bUseSpecularMap", useSpecularMap);
			}

		}

		void SetMaterial(const Material* material) const
		{
			if (shader) {
				shader->setVec3("material.ambient", material->ambient);
				shader->setVec3("material.diffuse", material->diffuse);
				shader->setVec3("material.specular", material->specular);
				shader->setFloat("material.shininess", material->shininess);
			}
		}

		void SetModelMatrix() const
		{
			if (shader)
			{
				shader->setMat4("model", model);
			}
		}
	};

	struct LightObject
	{
		// light type
		//  - 0: default point
		//  - 1: directional
		//  - 2: custom point
		//  - 3: spotlight
		int		  lightType      = 0;

		glm::vec3 lightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 lightPos       = glm::vec3(1.0f);

		const float constant  =  1.000f;
		float       linear    =  0.090f;
		float       quadratic =  0.032f;
		float		distance  = 50.000f;
		float		cutoff    =  0.000f; // for spotlight

		glm::vec3 ambientLight   = glm::vec3(0.0f);
		glm::vec3 diffuseLight   = glm::vec3(0.0f);
		glm::vec3 specularLight  = glm::vec3(0.0f);

		void SetLightUniforms(Shaders* shader, int index)
		{
			std::string base = "lights[" + std::to_string(index) + "]";

			shader->setInt(base + ".type", lightType);

			switch (lightType)
			{
			case 1:
				shader->setVec3(base + ".direction", lightDirection);
				break;

			case 2:
				shader->setVec3( base + ".position",  lightPos);

				SetLightAttenuation();
				shader->setFloat(base + ".linear",    linear);
				shader->setFloat(base + ".quadratic", quadratic);
				break;

			case 3:
				SetLightAttenuation();
				shader->setVec3(base + ".direction", lightDirection);
				shader->setVec3( base + ".position",  lightPos);
				shader->setFloat(base + ".constant",  constant);
				shader->setFloat(base + ".linear",    linear);
				shader->setFloat(base + ".quadratic", quadratic);
				shader->setFloat(base + ".cutoff", cutoff);
				break;

			default:
				shader->setVec3(base + ".position", lightPos);
				break;
			}

			shader->setVec3(base + ".ambient", ambientLight);
			shader->setVec3(base + ".diffuse", diffuseLight);
			shader->setVec3(base + ".specular", specularLight);
		}

		void SetLightAttenuation()
		{
			linear    = 4.50f / distance;
			quadratic = 75.0f / glm::pow(distance, 2);
		}
	};

	Scene(Shaders* shader, Shapes* shape, Camera* camera, SceneParameters* sceneParams, Texture* texture, MaterialLibrary* material,
		ModelLibrary* modelLib);
	~Scene();

	void UseShader(const std::string& shader) const;

	void CreateTexture(const char *filePath, std::string tag);

	void LoadTextures() const;
	int FindTextureID(std::string tag);

	void AddToVec();

	void PrepareScene();
    void RenderScene();

	glm::mat4 GetProjMatrix() { return m_projection; }
	glm::mat4 GetViewMatrix() { return m_view;       }
	glm::vec3 GetCameraPos()  { return m_cameraPos;  }

private:
	Shaders* m_shaders;
	Shapes* m_shapes;
	Camera* m_camera;
	SceneParameters* m_sceneParams;
	Texture* m_lTexture;
	MaterialLibrary* m_materialLib;
	ModelLibrary* m_modelLib;

	glm::mat4 m_view{};
	glm::mat4 m_model{};
	glm::mat4 m_projection{};
	glm::vec3 m_cameraPos;

	GLuint m_texture{};
	int m_loadedTextures = 0;

	static void LoadObjectMaterials();

	void BindGLTextures() const;
	void SetShaderTexture(const std::string& texTag, const std::string& specMapTag) const;
	void SetLights() const;
	int FindTextureID(std::string tag) const;

	void SetObjects();



	void Draw(SceneObject &object);

};


#endif //INC_3D_SCENE_H