#ifndef HIRAETHENGINE_MENU_H
#define HIRAETHENGINE_MENU_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Scene.h"
#include "SceneParameters.h"
class ModelLibrary;

class Menu
{
public:
    Menu(GLFWwindow* window, const char* glslVersion, SceneParameters* sceneParams,
    	Texture* texture, MaterialLibrary* material, ModelLibrary* modelLib);
    ~Menu();

    void ToggleMenuVisibility();
    void RenderMenu(Shaders* shaders, Scene* scene);

private:
	SceneParameters* m_sceneParams;
	Texture* m_texture;
	ModelLibrary* m_modelLib;
	GLFWwindow* m_window;

	std::vector<Texture::TextureInfo> m_items;
	std::vector<Material> m_materials;

    static void BeginFrame();
    static void EndFrame();
    void MouseClick(int selectIdx, Scene* scene);
};


#endif //INC_3D_MENU_H