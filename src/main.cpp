#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../include/Camera.h"
#include "Menu.h"
#include "../include/ModelLibrary.h"
#include "../include/Scene.h"
#include "Shaders.h"
#include "../include/Shapes.h"
#include "View.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

namespace
{
    float width = 0;
    float height = 0;

    View*    view    = nullptr;
    Shaders* shaders = nullptr;
    Shapes*  shapes  = nullptr;
    Scene*   scene   = nullptr;
    Camera*  camera  = nullptr;
    Menu*    menu    = nullptr;
	SceneParameters* sceneParams = nullptr;
	Texture* textureLoader       = nullptr;
	MaterialLibrary* materialLib = nullptr;
	ModelLibrary* modelLib = nullptr;

	float gCameraSpeed = 0.05f;

    float lastX = width / 2.0;
    float lastY = height / 2.0;
    bool firstMouse = true;
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void Mouse_Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset);
void getTitleFPS(double &lastTime, int &nbFrames);

int main()
{
    // create new objects
    camera  	  = new Camera(glm::vec3(0.0f, 0.5f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f),
    			  			-90.0f, 0.0f);
    view    	  = new View();
    shaders 	  = new Shaders();
    shapes        = new Shapes(shaders);
	sceneParams   = new SceneParameters();
	textureLoader = new Texture();
	materialLib   = new MaterialLibrary();
	modelLib	  = new ModelLibrary();
    scene         = new Scene(shaders, shapes, camera,
							  sceneParams, textureLoader,
							  materialLib, modelLib);

    // fps and delta time values
    float lastFrame = 0.0f;
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // init GLFW, create window
    view->InitializeGLFW();
    view->CreateWindow("HiraethEngine");
    // init glad
    view->InitializeGLAD();

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    // callbacks
    glfwSetWindowUserPointer(view->GetHandle(), camera);
    glfwSetCursorPosCallback(view->GetHandle(), mouse_callback);
	glfwSetScrollCallback(view->GetHandle(), Mouse_Scroll_Callback);

    // load shaders
    shaders->LoadShaders("../resources/shaders/vert.glsl",
        "../resources/shaders/lightFrag.glsl",
        "lighting");
    shaders->LoadShaders("../resources/shaders/vert.glsl",
    	"../resources/shaders/lightCubeFrag.glsl",
    	"lightCube");
	shaders->LoadShaders("../resources/shaders/skyboxVert.glsl",
		"../resources/shaders/skyboxFrag.glsl",
		"skybox");

    scene->PrepareScene();

    // initialize imgui menu context
    menu = new Menu(view->GetHandle(), "#version 330 core", sceneParams, textureLoader, materialLib, modelLib);

    // main render loop
    while (!view->ShouldClose())
    {
        // calc delta time
        float currentFrame = glfwGetTime();
        if (lastFrame == 0.0f)
            lastFrame = currentFrame;
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        view->SetBGColor(0.25, 0.34, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render scene and get input
        scene->RenderScene();
        view->ProcessInput(camera, dt);

        // imgui menu
        menu->RenderMenu(shaders, scene);

        // update FPS, swap buffers and poll events
        getTitleFPS(lastTime, nbFrames);
        glfwSwapBuffers(view->GetHandle());
        glfwPollEvents();
    }

	delete shaders;
	delete shapes;
	delete camera;
	delete menu;
	delete sceneParams;
	delete textureLoader;
	delete materialLib;
	delete modelLib;
	delete scene;
	delete view;

    return 0;
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);

    if (camera)
    {
        if (cursorMode == GLFW_CURSOR_DISABLED)
            camera->ProcessMouseMovement(xOffset, yOffset);
    }
}

void getTitleFPS(double &lastTime, int &nbFrames)
{
    double currentTime = glfwGetTime();
    nbFrames++;

    if (currentTime - lastTime >= 1.0)
    {
        double fps = (double)nbFrames / (currentTime - lastTime);

        char title[64];
        snprintf(title, sizeof(title), "HiraethEngine | FPS: %.1f", fps);

        glfwSetWindowTitle(view->GetHandle(), title);

        nbFrames = 0;
        lastTime = currentTime;
    }
}

void Mouse_Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset)
{
	// calculate the camera speed based on the yoffset
	gCameraSpeed += yOffset * 0.2f;

	// set minimum speed
	if (gCameraSpeed < 1.0f)
	{
		gCameraSpeed = 1.0f;
	}

	// set maximum speed
	if (gCameraSpeed > 15.0f)
	{
		gCameraSpeed = 15.0f;
	}

	// update the MovementSpeed of the camera
	camera->MovementSpeed = gCameraSpeed;
}