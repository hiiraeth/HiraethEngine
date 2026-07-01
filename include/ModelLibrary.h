#ifndef HIRAETHENGINE_MODELLIBRARY_H
#define HIRAETHENGINE_MODELLIBRARY_H

#include <string>
#include <unordered_map>
#include "Model.h"

class ModelLibrary
{
public:
    ModelLibrary() = default;
    ~ModelLibrary();

    void LoadModel(std::string path, std::string modelTag);
    Model* GetModel(std::string modelTag);
	void UnloadModel(std::string modelTag);

private:
    std::unordered_map<std::string, Model*> m_models;
};



#endif //HIRAETHENGINE_MODELLIBRARY_H
