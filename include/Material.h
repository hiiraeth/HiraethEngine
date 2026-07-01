#ifndef HIRAETHENGINE_MATERIAL_H
#define HIRAETHENGINE_MATERIAL_H
#include <string>
#include <vector>
#include <glm/vec3.hpp>

struct Material
{
    std::string tag;
    glm::vec3   ambient;
    glm::vec3   diffuse;
    glm::vec3   specular;
    float       shininess;
};

class MaterialLibrary
{
public:
    void LoadDefaults();
    void Add(const Material &material);

    const Material* Find(const std::string &tag) const;
    const std::vector<Material>& GetAll() const { return m_materials; }

private:
    std::vector<Material> m_materials;
};


#endif //HIRAETHENGINE_MATERIAL_H