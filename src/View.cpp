#include "View.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>

namespace
{
	float gMouseSensMax = 0.15f;
	float gMouseSensMin = 0.01f;
}

View::View()
{
}

View::~View()
{
}

GLFWwindow* View::CreateWindow(const char* title)
{
	Camera* cam = new Camera();
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_window = glfwCreateWindow(mode->width, mode->height, title, nullptr, nullptr);

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetKeyCallback(m_window, key_callback);

    return m_window;
}

GLFWwindow* View::GetHandle() { return m_window; }

bool View::ShouldClose()
{
    if (!glfwWindowShouldClose(m_window))
        return false;

    return true;
}

int View::GetWindowWidth()
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    return mode->width;
}

int View::GetWindowHeight()
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    return mode->height;
}

void View::InitializeGLFW()
{
    if (!glfwInit())
        std::cerr << "Failed to init GLFW!";

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void View::InitializeGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cerr << "Failed to init GLAD!";
}

void View::SetBGColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
}

void View::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void View::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	View* view = static_cast<View*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		switch (view->isCursorDisabled)
		{
			case false:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				view->isCursorDisabled = true;
				break;

			case true:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				view->isCursorDisabled = false;
				break;
		}
	}
}

void View::ProcessInput(Camera* camera, float dt)
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, dt);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, dt);
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, dt);
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
		camera->ProcessKeyboard(DOWN, dt);

	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		if (!(camera->MouseSensitivity > gMouseSensMax))
		{
			// increase mouse sens
			camera->MouseSensitivity += camera->MouseSensitivity * 0.02;
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS)
	{
		if (!(camera->MouseSensitivity < gMouseSensMin))
		{
			// decrease mouse sens
			camera->MouseSensitivity -= camera->MouseSensitivity * 0.02;
		}
	}
}
