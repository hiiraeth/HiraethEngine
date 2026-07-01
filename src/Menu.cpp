#include "../include/Menu.h"
#include <iostream>
#include <GLFW/glfw3.h>

#include "imgui_internal.h"
#include "../include/ModelLibrary.h"

using namespace ImGui;

Menu::Menu(GLFWwindow* window, const char* glslVersion, SceneParameters* sceneParams,
		Texture* texture, MaterialLibrary* material, ModelLibrary* modelLib)
{
    // init imgui context
    IMGUI_CHECKVERSION();
    if (!CreateContext())
        std::cerr << "[WARNING] Failed to initialize ImGui!\n";
    else
        std::cout << "[INFO] Successfully initialized ImGui!\n";

    ImGuiStyle &style = GetStyle();
    style.FrameBorderSize = 1.5f;
    style.FrameRounding = 3.f;
    style.GrabRounding = style.FrameRounding;

    // init scene params pointer
	m_sceneParams = sceneParams;

	// init scene pointer
	m_texture = texture;

	m_modelLib = modelLib;

	m_window = window;

    // set opengl and glsl
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

	// get textures
	m_items = m_texture->GetTextures();

	// get materials
	m_materials = material->GetAll();

	// remove any specular textures
	for (unsigned int i = 0; i < m_items.size(); i++)
	{
		if (m_items.at(i).tag.contains("Specular"))
		{
			m_items.erase(m_items.begin() + i);
		}
	}

}

Menu::~Menu()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    DestroyContext();
}

void Menu::ToggleMenuVisibility()
{
}

void Menu::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    NewFrame();
}

void Menu::EndFrame()
{
    End();
    Render();
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

void Menu::RenderMenu(Shaders* shaders, Scene* scene)
{
    BeginFrame();
	SetNextWindowSizeConstraints(ImVec2(225, 50), ImVec2(500, 1200));

	static int selectedModelIdx = -1;

    if (Begin("Engine")) {
        if (BeginTabBar("tabs")) {

        	// models ================================================= //
            if (BeginTabItem("Models")) {
            	// housekeeping vars
            	const float totalWidth = GetContentRegionAvail().x - GetFontSize() * 5;
            	const float spacing = GetStyle().ItemSpacing.x;
            	const float posSliderWidth = ((totalWidth - spacing * 2) / 3.0f);
            	int modelToDelete = -1;


            	// load in a new model
            	if (CollapsingHeader("Load New Model", ImGuiTreeNodeFlags_DefaultOpen))
            	{
            		static char filePath[256] = "../resources/models/cat/12221_Cat_v1_l3.obj";
            		static char modelName[128] = "New Model";

            		InputText("File Path", filePath, IM_ARRAYSIZE(filePath));
            		InputText("Model Name", modelName, IM_ARRAYSIZE(modelName));

            		static bool nameExistsError = false; // Tracks if we need to show an error

            		if (Button("Load Model", ImVec2(-FLT_MIN, 0)))
            		{
            			std::string tag(modelName);
            			bool exists = false;

            			// Check if the name already exists in the scene
            			for (const auto& existingModel : m_sceneParams->models)
            			{
            				if (existingModel.name == tag)
            				{
            					exists = true;
            					break;
            				}
            			}

            			if (!exists)
            			{
            				m_modelLib->LoadModel(filePath, tag);

            				SceneParameters::ModelParams newModel(tag);
            				newModel.modelTag = tag;
            				newModel.filePath = filePath;
            				m_sceneParams->models.push_back(newModel);

            				nameExistsError = false; // Clear error on success
            			}
            			else
            			{
            				nameExistsError = true; // Flag to show error message
            			}
            		}

            		// Display red error text if a duplicate was found
            		if (nameExistsError)
            		{
            			TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Model already exists!");
            		}
            		Separator();
            	}

            	for (size_t i = 0; i < m_sceneParams->models.size(); i++)
            	{
            		auto& model = m_sceneParams->models.at(i);
            		PushID(i);

            		if (CollapsingHeader(model.name.c_str()))
            		{
            			Checkbox("Visible", &model.visible); SameLine();
            			Checkbox("Uniform Scale", &model.uniformScale);
				        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.f, 4.f));

				        // Position
				        SetNextItemWidth(posSliderWidth);
				        DragFloat("##PosX", &model.position.x, 0.01f, -10.f, 10.f, "%.2f"); SameLine();
				        SetNextItemWidth(posSliderWidth);
				        DragFloat("##PosY", &model.position.y, 0.01f, -10.f, 10.f, "%.2f"); SameLine();
				        SetNextItemWidth(posSliderWidth);
				        DragFloat("##PosZ", &model.position.z, 0.01f, -10.f, 10.f, "%.2f");
				        SameLine(); Text("Position");

				        // Rotation
				        SetNextItemWidth(posSliderWidth);
				        DragFloat("##RotX", &model.rotation.x, 0.01f, -360.f, 360.f, "%.1f"); SameLine();
				        SetNextItemWidth(posSliderWidth);
				        DragFloat("##RotY", &model.rotation.y, 0.01f, -360.f, 360.f, "%.1f"); SameLine();
				        SetNextItemWidth(posSliderWidth);
				        DragFloat("##RotZ", &model.rotation.z, 0.01f, -360.f, 360.f, "%.1f");
				        SameLine(); Text("Rotation");

				        // Scale
				        SetNextItemWidth(posSliderWidth);
				        bool scaleXChanged = DragFloat("##ScaleX", &model.scale.x, 0.01f, 0.01f, 5.f, "%.2f"); SameLine();
				        SetNextItemWidth(posSliderWidth);
				        bool scaleYChanged = DragFloat("##ScaleY", &model.scale.y, 0.01f, 0.01f, 5.f, "%.2f"); SameLine();
				        SetNextItemWidth(posSliderWidth);
				        bool scaleZChanged = DragFloat("##ScaleZ", &model.scale.z, 0.01f, 0.01f, 5.f, "%.2f");
				        SameLine(); Text("Scale");
            			Separator();

				        // Sync values
				        if (model.uniformScale) {
				            if (scaleXChanged) {
				                model.scale.y = model.scale.x;
				                model.scale.z = model.scale.x;
				            } else if (scaleYChanged) {
				                model.scale.x = model.scale.y;
				                model.scale.z = model.scale.y;
				            } else if (scaleZChanged) {
				                model.scale.x = model.scale.z;
				                model.scale.y = model.scale.z;
				            }
				        }

            			PushStyleColor(ImGuiCol_Button,        ImVec4(0.74f, 0.11f, 0.11f, 1.0f));
            			PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.54f, 0.11f, 0.11f, 1.0f));
            			PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.34f, 0.11f, 0.11f, 1.0f));
            			if (Button("Delete Model", ImVec2(-FLT_MIN, 0)))
            			{
            				modelToDelete = i;
            			}
						PopStyleColor(3);
				        PopStyleVar();
				        Separator();
            		}
            		PopID();
            	}

            	if (modelToDelete != -1)
            		m_sceneParams->models.erase(m_sceneParams->models.begin() + modelToDelete);

        		EndTabItem();
            }

        	if (BeginTabItem("Lights"))
			{
        		int lightToDelete = -1;

        		if (CollapsingHeader("Add new light"), ImGuiTreeNodeFlags_DefaultOpen)
        		{
        			static char lightName[128] = "New Light";
        			static bool nameExistsError = false;

        			InputText("Light Name", lightName, IM_ARRAYSIZE(lightName));

        			// add new light
        			if (Button("Load Light", ImVec2(-FLT_MIN, 0)))
        			{
        				std::string name(lightName);
        				bool exists = false;

        				for (const auto& existingLight : m_sceneParams->lights)
        				{
        					if (existingLight.name == name)
        					{
        						exists = true;
        						break;
        					}
        				}

        				if (!exists)
        				{
        					m_sceneParams->lights.push_back(SceneParameters::LightParams(name));
        					nameExistsError = false;
        				}
        				else
        					nameExistsError = true;

        			}

        			if (nameExistsError)
        				TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Light already exists!");

        			Separator();
        		}

        		// light controls
			    if (m_sceneParams)
			    {
			        float totalWidth = GetContentRegionAvail().x - GetFontSize() * 5;
			        float spacing = GetStyle().ItemSpacing.x;
			        float posSliderWidth = ((totalWidth - spacing * 2) / 3.0f);

			        const char* lightTypes[] = { "Default Point", "Directional", "Custom Point", "Spotlight" };

			        for (size_t i = 0; i < m_sceneParams->lights.size(); i++)
			        {
			            auto& light = m_sceneParams->lights[i];
			            PushID(i);

			            if (CollapsingHeader(light.name.c_str()))
			            {
			                Checkbox("Enabled", &light.enabled);
			                Separator();

			                // color sliders
			                SetNextItemWidth(-FLT_MIN - GetFontSize() * 5);
			                ColorEdit3("Ambient", &light.ambient.x);

			                SetNextItemWidth(-FLT_MIN - GetFontSize() * 5);
			                ColorEdit3("Diffuse", &light.diffuse.x);

			                SetNextItemWidth(-FLT_MIN - GetFontSize() * 5);
			                ColorEdit3("Specular", &light.specular.x);
			                Separator();

			                // position
			                SetNextItemWidth(posSliderWidth);
			                DragFloat("##PosX", &light.position.x, 0.01f, -10.f, 10.f, "%.2f"); SameLine();
			                SetNextItemWidth(posSliderWidth);
			                DragFloat("##PosY", &light.position.y, 0.01f, -10.f, 10.f, "%.2f"); SameLine();
			                SetNextItemWidth(posSliderWidth);
			                DragFloat("##PosZ", &light.position.z, 0.01f, -10.f, 10.f, "%.2f");
			                SameLine(); Text("Position");
			                Separator();

			                // direction
			                SetNextItemWidth(posSliderWidth);
			                DragFloat("##DirX", &light.direction.x, 0.01f, -10.f, 10.f, "%.2f"); SameLine();
			                SetNextItemWidth(posSliderWidth);
			                DragFloat("##DirY", &light.direction.y, 0.01f, -10.f, 10.f, "%.2f"); SameLine();
			                SetNextItemWidth(posSliderWidth);
			                DragFloat("##DirZ", &light.direction.z, 0.01f, -10.f, 10.f, "%.2f");
			                SameLine(); Text("Direction");
			                Separator();

			                // light type
			                Combo("Type", &light.lightType, lightTypes, IM_ARRAYSIZE(lightTypes));
			                Separator();

			                SetNextItemWidth(-FLT_MIN - GetFontSize() * 5);
			                DragFloat("Distance", &light.distance, 1.0f, 1.f, 100.f, "%.1f");
			                Separator();

			            	PushStyleColor(ImGuiCol_Button,        ImVec4(0.74f, 0.11f, 0.11f, 1.0f));
			            	PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.54f, 0.11f, 0.11f, 1.0f));
			            	PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.34f, 0.11f, 0.11f, 1.0f));

			            	if (Button("Delete Light", ImVec2(-FLT_MIN, 0)))
			            	{
			            		lightToDelete = i;
			            		std::string base = "lights[" + std::to_string(lightToDelete) + "]";
			            		shaders->setVec3(base + ".diffuse", 0.0f, 0.0f, 0.0f);
			            	}
			            	PopStyleColor(3);
			            }
			            PopID();
			        }
			    }

        		if (lightToDelete != -1)
        		{
        			m_sceneParams->lights.erase(m_sceneParams->lights.begin() + lightToDelete);
        		}

        		EndTabItem();
			}

            if (BeginTabItem("Scene"))
            {
            	static std::vector<std::string> fileNames = HIRAETHENGINE_UTILS::GetSceneFiles();
            	static std::string currentScenePath = ""; // track the loaded scene path
            	static char saveAsTitle[64] = "";

            	// LOADING SCENE
            	SeparatorText("Load");

            	static int itemSelectIdx = 0;

            	if (Button("Refresh & Sort", ImVec2(-FLT_MIN, 0)))
            	{
            		fileNames = HIRAETHENGINE_UTILS::GetSceneFiles();
            		std::sort(fileNames.begin(), fileNames.end());
            	}

            	if (BeginListBox("##allScenes", ImVec2(-FLT_MIN, 0)))
            	{
            		for (unsigned int n = 0; n < fileNames.size(); n++)
            		{
            			bool isSelected = (itemSelectIdx == n);
            			std::string displayName = std::filesystem::path(fileNames[n]).filename().string();

            			if (Selectable(displayName.c_str(), &isSelected))
            			{
            				itemSelectIdx = n;
            				currentScenePath = fileNames[n];

            				for (const auto& model : m_sceneParams->models)
            					if (!model.modelTag.empty())
            						m_modelLib->UnloadModel(model.modelTag);

            				m_sceneParams->LoadFromFile(fileNames[n]);

            				for (const auto& model : m_sceneParams->models)
            					if (!model.modelTag.empty() && !model.filePath.empty())
            						m_modelLib->LoadModel(model.filePath, model.modelTag);
            			}
            		}
            		EndListBox();
            	}

            	// SAVING SCENE
            	SeparatorText("Save");

            	if (currentScenePath.empty())
            	{
            		// No scene loaded — grey out Save, force Save As
            		BeginDisabled();
            		Button("Save", ImVec2(-FLT_MIN, 0));
            		EndDisabled();
            	}
            	else
            	{
            		if (Button("Save", ImVec2(-FLT_MIN, 0)))
            		{
            			OpenPopup("Confirm Save");
            		}
            	}

            	if (Button("Save As", ImVec2(-FLT_MIN, 0)))
            	{
            		OpenPopup("Save as name");
            	}

            	// allows user to save file as custom name. must have a name and must be less than 64 chars
            	if (BeginPopupModal("Save as name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            	{
            		static bool fileExists = false;
            		static bool pathExists = true;

            		Text("Save as:");
            		InputTextWithHint("##SaveAsLabel", "Save as...", saveAsTitle, 64);

            		if (Button("Save"))
		            {
		            	std::string fullPath = "../resources/scenes/" + std::string(saveAsTitle) + ".json";
		            	currentScenePath = fullPath;

		            	if (HIRAETHENGINE_UTILS::CheckIfFileExists(fullPath))
		            	{
		            		fileExists = true;
		            	}
		            	else
		            	{
		            		if (strlen(saveAsTitle) > 0 && strlen(saveAsTitle) < 63)
		            		{
		            			m_sceneParams->SaveToFile(currentScenePath);
		            			fileNames = HIRAETHENGINE_UTILS::GetSceneFiles();
		            			std::sort(fileNames.begin(), fileNames.end());

		            			saveAsTitle[0] = '\0';
		            			CloseCurrentPopup();
		            		}
		            		else
		            			pathExists = false;
		            	}
		            }

            		SameLine();
            		if (Button("Cancel"))
            		{
            			saveAsTitle[0] = '\0';
            			CloseCurrentPopup();
            		}

            		if (fileExists)
            			TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "File already exists!");

            		if (!pathExists)
            			TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid length!");

		            EndPopup();
            	}

            	// alerts user save will overwrite current file, asks for confirmation
            	if (BeginPopupModal("Confirm Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            	{
            		Text("This will overwrite:");
            		TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "%s", currentScenePath.c_str());
            		Text("Are you sure?");
            		Separator();

            		if (Button("Yes", ImVec2(120, 0)))
            		{
            			m_sceneParams->SaveToFile(currentScenePath);
            			fileNames = HIRAETHENGINE_UTILS::GetSceneFiles();
            			CloseCurrentPopup();
            		}
            		SameLine();
            		if (Button("Cancel", ImVec2(120, 0)))
            		{
            			CloseCurrentPopup();
            		}
            		EndPopup();
            	}

            	EndTabItem();
				}
        	EndTabBar();
        }
    }
    EndFrame();
}