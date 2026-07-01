#ifndef HIRAETHENGINE_MESH_H
#define HIRAETHENGINE_MESH_H

#include <vector>
#include "Shaders.h"

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };


    struct Texture
    {
        GLuint id;
        std::string type;
        std::string path;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    glm::vec3 BaseDiffuse = glm::vec3(0.8f);
    glm::vec3 BaseSpecular = glm::vec3(0.5f);
    float BaseShininess = 32.0f;

    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures);
    void Draw(Shaders* shader);

private:
    unsigned int VBO, EBO;

    void setupMesh();
};

#endif //HIRAETHENGINE_MESH_H
