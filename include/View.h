#ifndef HIRAETHENGINE_VIEW_H
#define HIRAETHENGINE_VIEW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Camera.h"

class View
{
private:
    GLFWwindow* m_window;
    mutable bool isCursorDisabled = false;
public:
    View();
    ~View();

    GLFWwindow* CreateWindow(const char* title);
    GLFWwindow* GetHandle();
    bool ShouldClose();
    int GetWindowWidth();
    int GetWindowHeight();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void InitializeGLFW();
    void InitializeGLAD();

    void SetBGColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void ProcessInput(Camera* camera, float dt);
};


#endif //INC_3D_VIEW_H