//
// Created by owen geltz on 2/22/26.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::Draw(Shaders* shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    shader->setInt("bUseDiffuseMap", false);
    shader->setInt("bUseSpecularMap", false);

    // Send fallback material properties just in case textures are missing!
    shader->setVec3("material.diffuse", BaseDiffuse);
    shader->setVec3("material.specular", BaseSpecular);
    shader->setFloat("material.shininess", BaseShininess);

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures[i].type;

        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
            shader->setInt("bUseDiffuseMap", true); // Toggle flag in your shader
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
            shader->setInt("bUseSpecularMap", true); // Toggle flag in your shader
        }

        // Set the sampler to the correct texture unit (e.g., material.texture_diffuse1)
        shader->setInt("material." + name + number, i);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Reset active texture to default
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normal (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Texture Coords (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}