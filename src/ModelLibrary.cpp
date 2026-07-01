//
// Created by owen geltz on 2/22/26.
//

#include "../include/ModelLibrary.h"

#include "utils.h"

ModelLibrary::~ModelLibrary()
{
    m_models.clear();
}

void ModelLibrary::LoadModel(std::string path, std::string modelTag)
{
    if (m_models.find(modelTag) != m_models.end())
        return;

    Model* newModel = new Model(path);
    m_models[modelTag] = newModel;
}

Model* ModelLibrary::GetModel(std::string modelTag)
{
    auto it = m_models.find(modelTag);
    if (it != m_models.end())
        return it->second;

    LOG_WARNING("Assimp Model not loaded!");
    return nullptr;
}

void ModelLibrary::UnloadModel(std::string modelTag)
{
	auto it = m_models.find(modelTag);
	if (it != m_models.end())
	{
		delete it->second;
		m_models.erase(it);
	}
}
