#ifndef HIRAETHENGINE_SCENEPARAMETERS_H
#define HIRAETHENGINE_SCENEPARAMETERS_H
#include <string>
#include <glm/vec3.hpp>
#include <fstream>
#include <iostream>

#include "json.hpp"
#include "utils.h"

using json = nlohmann::json;

// centralized struct for object params to enable control via imgui
struct SceneParameters
{
	struct ModelParams
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		bool visible;
		bool uniformScale;

		std::string modelTag;
		std::string name; // for imgui
		std::string filePath;

		ModelParams(std::string n = "Model")
		{
			position = glm::vec3(0.0f, 1.0f, 0.0f);
			rotation = glm::vec3(0.0f);
			scale    = glm::vec3(1.0f);
			visible  = true;
			uniformScale = false;
			modelTag = "";
			name = n;
		}
	};

    struct ObjectParams
    {
    	glm::vec3 position;
    	glm::vec3 rotation;
    	glm::vec3 scale;

    	std::string textureTag;
    	std::string specMapTag;
    	std::string matTag;

    	bool visible;

    	ObjectParams()
    	{
    		position = glm::vec3(0.0f);
    		rotation = glm::vec3(0.0f);
    		scale    = glm::vec3(1.0f);

    		textureTag = "";
    		specMapTag = "";
    		matTag     = "";

    		visible  = true;
    	}

    };

    struct LightParams
    {
    	glm::vec3 position;
    	glm::vec3 ambient;
    	glm::vec3 diffuse;
    	glm::vec3 specular;
    	glm::vec3 direction;
    	glm::vec3 scale;

		int lightType;
    	float distance;
    	bool enabled;

    	std::string name;

    	LightParams(std::string l = "Light")
    	{
    		position = glm::vec3(0.0f);
    		ambient  = glm::vec3(0.2f);
    		diffuse  = glm::vec3(0.8f);
    		specular = glm::vec3(1.0f);
    		direction = glm::vec3(0.0f, -1.0f, 0.0f);
    		scale = glm::vec3(0.2f);
    		lightType = 0;
    		distance  = 50.0f;
    		enabled   = true;
    		name = l;
    	}
    };

	std::vector<ModelParams> models;
	std::vector<LightParams> lights;

	// declare objects here
	ObjectParams plane;

	// set default values
    SceneParameters()
    {
		models.clear();
    }

	void LoadFromFile(const std::string& path)
    {
    	std::ifstream file(path);

    	if (!file.is_open())
    	{
    		LOG_ERROR("Failed to open scene file: " << path);
    	}

    	json data = json::parse(file);

    	// plane ====================================================== //
    	if (data.contains("plane"))
    	{
    		auto& p = data["plane"];
    		plane.visible    = p["visible"];
    		plane.position.x = p["position"][0];
    		plane.position.y = p["position"][1];
    		plane.position.z = p["position"][2];
    		plane.scale.x    = p["scale"][0];
    		plane.scale.y    = p["scale"][1];
    		plane.scale.z    = p["scale"][2];
    		plane.rotation.x = p["rotation"][0];
    		plane.rotation.y = p["rotation"][1];
    		plane.rotation.z = p["rotation"][2];
    	}

    	// lights ===================================================== //
    	if (data.contains("lights"))
    	{
    		auto parseLightParams = [](const json& l, LightParams& light)
    		{
    			light.enabled = l["enabled"];
    			light.lightType = l["type"];
    			light.distance = l["distance"];

    			light.scale.x = l["scale"][0];
    			light.scale.y = l["scale"][1];
    			light.scale.z = l["scale"][2];
    			light.position.x = l["position"][0];
    			light.position.y = l["position"][1];
    			light.position.z = l["position"][2];
    			light.direction.x = l["direction"][0];
    			light.direction.y = l["direction"][1];
    			light.direction.z = l["direction"][2];

    			light.ambient.x = l["ambient"][0];
    			light.ambient.y = l["ambient"][1];
    			light.ambient.z = l["ambient"][2];
    			light.diffuse.x = l["diffuse"][0];
    			light.diffuse.y = l["diffuse"][1];
    			light.diffuse.z = l["diffuse"][2];
    			light.specular.x = l["specular"][0];
    			light.specular.y = l["specular"][1];
    			light.specular.z = l["specular"][2];
    		};

    		lights.clear();
    		for (auto& l : data["lights"])
    		{
    			LightParams light(l["name"]);
    			parseLightParams(l, light);
    			lights.push_back(light);
    		}
    	}

    	// models ===================================================== //
    	if (data.contains("models"))
    	{
    		models.clear();
    		for (auto& m : data["models"])
    		{
    			ModelParams model(m["name"]);

    			model.modelTag = m["tag"];
    			model.visible = m["visible"];
    			model.position  = { m["position"][0], m["position"][1], m["position"][2] };
    			model.scale     = { m["scale"][0],    m["scale"][1],    m["scale"][2]    };
    			model.rotation  = { m["rotation"][0], m["rotation"][1], m["rotation"][2] };
    			model.filePath	= m["filePath"];
    			models.push_back(model);
    		}
    	}
    }

	void SaveToFile(const std::string& path) const
	{
	    json data;

	    // --- Plane ---
	    data["plane"]["visible"]     = plane.visible;
	    data["plane"]["position"]    = { plane.position.x, plane.position.y, plane.position.z };
	    data["plane"]["scale"]       = { plane.scale.x,    plane.scale.y,    plane.scale.z    };
	    data["plane"]["rotation"]    = { plane.rotation.x, plane.rotation.y, plane.rotation.z };

	    // --- Lights ---
	    for (auto& l : lights)
	    {
	        json light;
	        light["enabled"]   = l.enabled;
	        light["type"]      = l.lightType;
	    	light["name"]      = l.name;
	        light["distance"]  = l.distance;
	        light["position"]  = { l.position.x,  l.position.y,  l.position.z  };
	        light["direction"] = { l.direction.x, l.direction.y, l.direction.z };
	    	light["scale"]     = { l.scale.x,     l.scale.y,     l.scale.z     };
	        light["ambient"]   = { l.ambient.x,   l.ambient.y,   l.ambient.z   };
	        light["diffuse"]   = { l.diffuse.x,   l.diffuse.y,   l.diffuse.z   };
	        light["specular"]  = { l.specular.x,  l.specular.y,  l.specular.z  };
	    	data["lights"].push_back(light);
	    };

	    // --- Models ---
	    for (auto& m : models)
	    {
	        json model;
	        model["name"]     = m.name;
	        model["tag"]      = m.modelTag;
	    	model["filePath"] = m.filePath;
	        model["visible"]  = m.visible;
	        model["position"] = { m.position.x, m.position.y, m.position.z };
	        model["scale"]    = { m.scale.x,    m.scale.y,    m.scale.z    };
	        model["rotation"] = { m.rotation.x, m.rotation.y, m.rotation.z };
	        data["models"].push_back(model);
	    }

	    // --- Write to file ---
	    std::ofstream file(path);
	    if (!file.is_open())
	    {
	        LOG_ERROR("Failed to write scene file: " << path);
	        return;
	    }

	    file << data.dump(2);

    	//scenes.push_back()
	}
};

#endif //HIRAETHENGINE_SCENEPARAMETERS_H