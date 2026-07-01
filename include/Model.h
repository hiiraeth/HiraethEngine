#ifndef HIRAETHENGINE_MODEL_H
#define HIRAETHENGINE_MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <iostream>

class Model
{
public:
    Model(std::string const &path) { loadModel(path); }

    void Draw(Shaders* shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Mesh::Texture> textures_loaded;

    void loadModel(std::string const &path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene* scene);
    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};


#endif //HIRAETHENGINE_MODEL_H