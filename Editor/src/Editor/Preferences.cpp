#include "Preferences.h"

#include "ComponentManager.h"
#include "ResourcesManager.h"
#include "nlohmann/json.hpp"
#include "ProjectsManager.h"
#include "PrefabManager.h"
#include "LogManager.h"
#include "Texture.h"
#include "Editor.h"
#include "imgui.h"
#include "Game.h"

#include <fstream>
#include <sstream>

#include "CheckML.h"

#define UpArrowImage "upArrow.png"
#define DownArrowImage "downArrow.png"
#define LeftArrowImage "leftArrow.png"
#define RightArrowImage "rightArrow.png"

#define VERTICALSPACING 10

using nlohmann::json;
using nlohmann::ordered_json;

const int MAX_COLLISION_LAYERS = 16;

namespace ShyEditor {

	Preferences* Preferences::instance = nullptr;

	Preferences::Preferences() : Window("Preferences", 0)
	{
		canBeDisplayedOnTop = true;

		instance = this;

		open = false;
		windowWidth = 700;
		windowHeight = 700;
		canBeDisplayedOnTop = true;

		upArrow = ResourcesManager::GetInstance()->AddTexture(UpArrowImage, true);
		downArrow = ResourcesManager::GetInstance()->AddTexture(DownArrowImage, true);
		leftArrow = ResourcesManager::GetInstance()->AddTexture(LeftArrowImage, true);
		rightArrow = ResourcesManager::GetInstance()->AddTexture(RightArrowImage, true);

		// Physics attributes for ImGUI
		updateMatrix = false;
		showError = false;
		errorMsg = L"Default error message";
		defaultLayer = "Default";
		nLayers = 1;

		// Input attributes for ImGUI

			// Keyboard

			specialKeys = Components::ComponentManager::GetEnum("KB_SPECIALKEYS");
			KB_jumpPreview = specialKeys[4]; // 4 is SPACE
			KB_jumpSelected = 4;

			letterKeys = Components::ComponentManager::GetEnum("KB_LETTERS");
			KB_movementSelected = 0; // 0 is A

			KB_actionSelected = 4; // 4 is E
			KB_actionPreview = letterKeys[4];

			// Controller
			ps4Buttons = Components::ComponentManager::GetEnum("PS4Controller");
			xboxButtons = Components::ComponentManager::GetEnum("XboxController");

			CT_jumpSelected = 0;
			CT_jumpPreview = ps4Buttons[CT_jumpSelected];

			CT_actionSelected = 2;
			CT_actionPreview = ps4Buttons[CT_actionSelected];

		// General
		data.initialScene = "Scene";
		data.creator = "Yojhan, Pablo e Ivan";
		data.name = Editor::GetInstance()->GetProjectInfo().name;
		data.splashScreen = false;

		// Window
		data.windowTitle = Editor::GetInstance()->GetProjectInfo().name;
		data.icon = "";
		data.width = 1080;
		data.height = 720;
		data.vsync = true;
		data.showCursor = true;
		data.fullscreen = false;
		data.bgColor = PreferencesColor();

		// Physics
		data.gravity_x = 0;
		data.gravity_y = -9.81f;
		data.collisionMatrix = std::vector<std::vector<bool>>(MAX_COLLISION_LAYERS, std::vector<bool>(MAX_COLLISION_LAYERS, false));
		data.layers.push_back("Default");

		// Input
		data.closeWithEscape = true;

			// Keyboard
			data.KB_Jump = 4;
			data.KB_Action = 4;
			data.KB_movements = std::vector<int>(4, 0);
			data.KB_movements[0] = 0; data.KB_movements[1] = 3; 
			data.KB_movements[2] = 22; data.KB_movements[3] = 18;

			// Controller
			data.CT_Jump = 0;
			data.CT_Action = 2;
				

		// Overlay
		data.timeToDoubleClick = 0.5f;
		data.timeToHoldClick = 0.1f;

		// Audio
		data.frequency = 44100;
		data.channels = 1;
		data.chunksize = 2048;

		LoadData();

	}

	void Preferences::Open() {
		instance->Show();
	}


	void Preferences::Behaviour()
	{

		if (ImGui::BeginTabBar("PreferencesTab")) {



			if (ImGui::BeginTabItem("General")) {
				GeneralHeader();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Window")) {
				WindowHeader();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Physics")) {
				PhysicsHeader();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Input")) {
				InputHeader();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Overlay")) {
				OverlayHeader();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Audio")) {
				AudioHeader();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();

		}
		// BUILD PATH


	}

	void Preferences::GeneralHeader() {

		ImGui::Indent();

		char buffer[256];

		// Game name
		ImGui::Text("Game name");
		ImGui::SameLine();
		memcpy(buffer, data.name.c_str(), 256);
		if (ImGui::InputText("##Gamename", buffer, 256))
			data.name = buffer;

		ImGui::Dummy({ 0, VERTICALSPACING });

		// Window size	TODO: Seleccionar un fichero en vez de poner la ruta
		ImGui::Text("Game Initial Scene");
		ImGui::SameLine();
		memcpy(buffer, data.initialScene.c_str(), 256);
		if (ImGui::InputText("##Initialscene", buffer, 256))
			data.initialScene = buffer;

		ImGui::Unindent();

	}

	void Preferences::WindowHeader() {

		ImGui::Indent();

		char buffer[256];

		// Game icon	TODO: Seleccionar un fichero en vez de poner la ruta
		ImGui::Text("Game Window Icon");
		ImGui::SameLine();
		memcpy(buffer, data.icon.c_str(), 256);
		if (ImGui::InputText("##Gameicon", buffer, 256))
			data.icon = buffer;

		ImGui::Dummy({ 0, VERTICALSPACING });

		// Window size
		ImGui::Text("Game Window Size");
		ImGui::SameLine();
		int gameSize[2]{ data.width , data.height };
		if (ImGui::DragInt2("##Gamesize", &gameSize[0], 1, 10, INT_MAX, "%d")) {
			data.width = gameSize[0];
			data.height = gameSize[1];
		}

		ImGui::Dummy({ 0, VERTICALSPACING });

		// Start fullscreen
		ImGui::Text("Start fullscreen");
		ImGui::SameLine();
		ImGui::Checkbox("##fullscreen", &data.fullscreen);

		ImGui::Dummy({ 0, VERTICALSPACING });

		// Start showing cursor
		ImGui::Text("Start showing cursor");
		ImGui::SameLine();
		ImGui::Checkbox("##showcursor", &data.showCursor);

		ImGui::Dummy({ 0, VERTICALSPACING });

		// Background Color
		ImGui::Text("Background Color");
		ImGui::SameLine();
		ImGui::ColorEdit4("##background", (float*)&data.bgColor);


		ImGui::Unindent();

	}

	void Preferences::AudioHeader() {

		ImGui::Indent();

		ImGui::Text("Frequency");
		ImGui::SameLine();
		ImGui::InputInt("##frequency", &data.frequency);

		ImGui::Dummy({ 0, VERTICALSPACING });

		ImGui::Text("Channels");
		ImGui::SameLine();
		ImGui::RadioButton("Mono", &data.channels, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Stereo", &data.channels, 1);

		ImGui::Dummy({ 0, VERTICALSPACING });

		ImGui::Text("Chunk Size");
		ImGui::SameLine();
		ImGui::InputInt("##chunksize", &data.chunksize);

		ImGui::Unindent();

	}

	void Preferences::InputHeader() {
		ImGui::Indent();

		ImGui::Text("Close Game With Escape");
		ImGui::SameLine();
		ImGui::Checkbox("##closewithescape", &data.closeWithEscape);

		ImGui::Dummy({ 0, VERTICALSPACING });

		// KeyBoard
		if (ImGui::CollapsingHeader("KeyBoard")) {

			ImGui::Indent();

			// Jump
			ImGui::Text("Jump");
			ImGui::SameLine();

			KB_jumpPreview = specialKeys[data.KB_Jump];

			if (ImGui::BeginCombo("##JumpSpecialKeys", KB_jumpPreview.c_str())) {

				for (int i = 0; i < specialKeys.size(); i++) {

					bool isSelected = i == KB_jumpSelected;

					if (ImGui::Selectable(specialKeys[i].c_str(), isSelected)) {

						KB_jumpSelected = i;
						data.KB_Jump = KB_jumpSelected;
					}

				}

				ImGui::EndCombo();
			}

			ImGui::Dummy({ 0, VERTICALSPACING });

			// Movement
			ImGui::Text("Movement");

			ImGui::Dummy({ 0, 10 });

			ImGui::ImageButton(leftArrow->getSDLTexture(), ImVec2(50, 50));
			ImGui::SameLine();
			ComboForArrow(LEFT);

			ImGui::ImageButton(rightArrow->getSDLTexture(), ImVec2(50, 50));
			ImGui::SameLine();
			ComboForArrow(RIGHT);

			ImGui::ImageButton(upArrow->getSDLTexture(), ImVec2(50, 50));
			ImGui::SameLine();
			ComboForArrow(UP);

			ImGui::ImageButton(downArrow->getSDLTexture(), ImVec2(50, 50));
			ImGui::SameLine();
			ComboForArrow(DOWN);

			ImGui::Dummy({ 0, VERTICALSPACING });

			// Action
			ImGui::Text("Action");
			ImGui::SameLine();

			KB_actionPreview = letterKeys[data.KB_Action];

			if (ImGui::BeginCombo("##ActionSpecialKeys", KB_actionPreview.c_str())) {

				for (int i = 0; i < letterKeys.size(); i++) {

					bool isSelected = i == KB_actionSelected;

					if (ImGui::Selectable(letterKeys[i].c_str(), isSelected)) {

						KB_actionSelected = i;
						data.KB_Action = KB_actionSelected;
					}

				}

				ImGui::EndCombo();
			}

			ImGui::Unindent();

		}

		ImGui::Dummy({ 0, VERTICALSPACING });

		// Controller
		if (ImGui::CollapsingHeader("Controller")) {
			ImGui::Indent();

			ImGui::Text("Jump");
			ImGui::SameLine();

			CT_jumpPreview = ps4Buttons[data.CT_Jump] + " / " + xboxButtons[data.CT_Jump];

			if (ImGui::BeginCombo("##JumpButtons", CT_jumpPreview.c_str())) {

				for (int i = 0; i < ps4Buttons.size(); i++) {

					bool isSelected = i == CT_jumpSelected;

					std::string buttonName = ps4Buttons[i] + " / " + xboxButtons[i];
					if (ImGui::Selectable(buttonName.c_str(), isSelected)) {

						CT_jumpSelected = i;
						data.CT_Jump = CT_jumpSelected;
					}

				}

				ImGui::EndCombo();
			}

			ImGui::Dummy({ 0, VERTICALSPACING });

			ImGui::Text("Action");
			ImGui::SameLine();

			CT_actionPreview = ps4Buttons[data.CT_Action] + " / " + xboxButtons[data.CT_Action];

			if (ImGui::BeginCombo("##ActionButtons", CT_actionPreview.c_str())) {

				for (int i = 0; i < ps4Buttons.size(); i++) {

					bool isSelected = i == CT_actionSelected;

					std::string buttonName = ps4Buttons[i] + " / " + xboxButtons[i];
					if (ImGui::Selectable(buttonName.c_str(), isSelected)) {

						CT_actionSelected = i;
						data.CT_Action = CT_actionSelected;
					}

				}

				ImGui::EndCombo();
			}

			ImGui::Unindent();

		}


		ImGui::Unindent();
	}

	void Preferences::OverlayHeader() {
		ImGui::Indent();

		ImGui::Text("Time to Double Click");
		ImGui::SameLine();
		ImGui::DragFloat("##timetodoubleclick", &data.timeToDoubleClick);

		ImGui::Dummy({ 0, VERTICALSPACING });

		ImGui::Text("Time to Hold Click");
		ImGui::SameLine();
		ImGui::DragFloat("##timetoholdclick", &data.timeToHoldClick);

		ImGui::Unindent();
	}

	void Preferences::PhysicsHeader() {

		ImGui::Indent();

		// Gravity
		ImGui::Text("World Gravity");
		ImGui::SameLine();

		float gravity[2]{ data.gravity_x , data.gravity_y };
		if (ImGui::DragFloat2("##Gravity", &gravity[0])) {
			data.gravity_x = gravity[0];
			data.gravity_y = gravity[1];
		}

		ImGui::Dummy({ 0, VERTICALSPACING });

		char buffer[256];

		// Layers
		ImGui::Text("Physic Layers");

		ImGui::InputText("##readonly_text", const_cast<char*>(defaultLayer.c_str()), defaultLayer.size(), ImGuiInputTextFlags_ReadOnly);

		// Skip default layer
		auto it = data.layers.begin(); ++it;

		int index = 0;
		while (it != data.layers.end()) {

			memcpy(buffer, (*it).c_str(), 256);
			std::string label = "##" + (*it);
			if (ImGui::InputText(label.c_str(), buffer, 256))
				(*it) = buffer;

			ImGui::SameLine();

			label = "Delete##" + std::to_string(index);
			if (ImGui::Button(label.c_str())) {
				it = data.layers.erase(it);
				updateMatrix = true;
			}
			else
				++it;

			index++;
		}



		// Add layer
		buffer[0] = '\0';
		if (ImGui::InputText("##Addlayer", buffer, 256)) {
			newLayer = buffer;
		}

		ImGui::SameLine();
		if (ImGui::Button("Add Layer")) {

			showError = true;
			if (newLayer.empty()) {
				errorMsg = L"Can not add a layer with an empty name!";
			}
			else if (std::find(data.layers.begin(), data.layers.end(), newLayer) != data.layers.end()) {
				errorMsg = L"It's not possible to add the layer because one with the same name already exists!";
			}
			else if (nLayers >= MAX_COLLISION_LAYERS) {
				errorMsg = L"You can't add more collision layers. Maximum reached";
			}
			else {
				showError = false;
				data.layers.push_back(newLayer);
				updateMatrix = true;
			}

			newLayer = "";
		}

		// Collision matrix
		ImGui::Dummy(ImVec2(0, 15));
		ImGui::Text("Collision Matrix");
		ImGui::Dummy(ImVec2(0, 15));

		// Refresh logic
		if (updateMatrix) {
			nLayers = data.layers.size();
			updateMatrix = false;
		}

		// Collision matrix structure
		std::string label = "##";
		float checkboxwidth = 28.0f;
		for (int i = 0; i < nLayers; i++) {

			ImGui::Text(data.layers[i].c_str());

			for (int c = 0; c < nLayers; c++) {

				// Top-left diagonal only
				if (i + c < nLayers) {

					ImGui::SameLine(100.0f + c * checkboxwidth);

					label += std::to_string(i + c);
					bool value = data.collisionMatrix[i][c];
					if (ImGui::Checkbox(label.c_str(), &value)) {
						data.collisionMatrix[i][c] = value;
					}

				}
			}

			if (i > 0) {
				ImGui::SameLine();
				ImGui::Text(data.layers[i - 1].c_str());
			}

		}

		ImGui::Text("");
		ImGui::SameLine(100.0f);
		ImGui::Text(data.layers[nLayers - 1].c_str());

		ImGui::Dummy(ImVec2(0, 15));

		if (showError)
			ShowErrorPopup(errorMsg);

		ImGui::Unindent();

	}

	void Preferences::ComboForArrow(int arrowIndex) {

		std::string label = "##MovementSpecialKeys" + std::to_string(arrowIndex);

		if (ImGui::BeginCombo(label.c_str(), letterKeys[data.KB_movements[arrowIndex]].c_str())) {

			for (int i = 0; i < letterKeys.size(); i++) {

				bool isSelected = i == KB_movementSelected;

				if (ImGui::Selectable(letterKeys[i].c_str(), isSelected)) {

					KB_movementSelected = i;

					data.KB_movements[arrowIndex] = KB_movementSelected;
				}

			}

			ImGui::EndCombo();

		}
	}

	ProjectData& Preferences::GetData()
	{
		return instance->data;
	}


	void Preferences::GenerateDebug()
	{
		json root = instance->BasicData();

		// Specific configuration for debug
		root["splashScreen"] = false;
		root["debugPhysics"] = true;
		root["debugFrameRate"] = true;

		root["path"] = Editor::GetInstance()->GetProjectInfo().assetPath;
		root["projectFilePath"] = ProjectsManager::GetProjectFilePath();

		PrefabManager::SavePrefabs();

		std::ofstream file("Engine\\config.json");

		file << root.dump(4);

		file.close();
	}

	void Preferences::GenerateRelease()
	{
		json root = instance->BasicData();

		// Specific configuration for release
		root["splashScreen"] = true;
		root["debugPhysics"] = false;
		root["debugFrameRate"] = false;

		root["path"] = Editor::GetInstance()->GetProjectInfo().assetPath;
		root["projectFilePath"] = ProjectsManager::GetProjectFilePath();

		PrefabManager::SavePrefabs();

		std::ofstream file("Engine\\config.json");

		file << root.dump(4);

		file.close();
	}

	void Preferences::GenerateBuild()
	{
		json root = instance->BasicData();

		// Specific configuration for release
		root["splashScreen"] = true;
		root["debugPhysics"] = false;
		root["debugFrameRate"] = false;

		root["path"] = "Assets";
		root["projectFilePath"] = ProjectsManager::GetProjectFileName();

		PrefabManager::SavePrefabs();

		std::ofstream file("Engine\\config.json");

		file << root.dump(4);

		file.close();
	}

	nlohmann::ordered_json Preferences::BasicData()
	{
		nlohmann::ordered_json root = nlohmann::ordered_json();

		// General
		root["game"] = data.name;
		root["creator"] = data.creator;
		root["initialScene"] = data.initialScene;
		root["splashScreen"] = false;

		// Window
		root["windowTitle"] = data.windowTitle;
		root["windowIcon"] = data.icon;
		root["windowSize"] = std::to_string(data.width) + ", " + std::to_string(data.height);
		root["vsync"] = data.vsync;
		root["fullScreen"] = data.fullscreen;
		root["showcursor"] = data.showCursor;

		root["bgColor"] = std::to_string((int)(data.bgColor.r * 255)) + ", " + std::to_string((int)(data.bgColor.g * 255)) + ", " + std::to_string((int)(data.bgColor.b * 255));
		root["bgAlpha"] = (int)(data.bgColor.a * 255);

		// Physics
		root["gravity"] = std::to_string(data.gravity_x) + ", " + std::to_string(data.gravity_y);
		root["layers"] = data.layers;
		root["matrix"] = data.collisionMatrix;
		root["debugPhysics"] = true;

		// Input
		root["closeWithEscape"] = data.closeWithEscape;

			// KeyBoard

			root["KB_Jump"] = data.KB_Jump;
			root["KB_Action"] = data.KB_Action;
			root["KB_movements"] = data.KB_movements;

			// Controller

			root["CT_Jump"] = data.CT_Jump;
			root["CT_Action"] = data.CT_Action;

		// Overlay
		root["timeToDoubleClick"] = data.timeToDoubleClick;
		root["timeToHoldClick"] = data.timeToHoldClick;
		root["debugFrameRate"] = true;

		// Audio
		root["frequency"] = data.frequency;
		root["channels"] = data.channels;
		root["chuncksize"] = data.chunksize;

		return root;
	}

	void Preferences::ShowErrorPopup(const std::wstring& errorMessage) {

		ImGui::OpenPopup("Error");

		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 1.5f, ImGui::GetIO().DisplaySize.y / 8.0f));

		if (ImGui::BeginPopupModal("Error")) {
			ImGui::Text("%ls", errorMessage.c_str());

			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2.0f - ImGui::GetContentRegionAvail().x / 16.0f);
			ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 0.8f);
			if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x / 4.0f, ImGui::GetContentRegionAvail().y / 1.5f))) {
				ImGui::CloseCurrentPopup();
				showError = false;
			}
			ImGui::EndPopup();
		}

	}

	void Preferences::StoreData() {

		std::ifstream projectFile(ProjectsManager::GetProjectFilePath());

		if (!projectFile.good() || !json::accept(projectFile)) {
			LogManager::LogError("Could not open project file to store preferences data.");
			return;
		}

		projectFile.clear();
		projectFile.seekg(0);

		ordered_json project = ordered_json::parse(projectFile);

		ordered_json preferences = instance->BasicData();

		project["Preferences"] = preferences;

		std::ofstream output(ProjectsManager::GetProjectFilePath());
		output << project.dump(4);
		output.close();

	}

	void Preferences::LoadData() {

		std::ifstream projectFile(ProjectsManager::GetProjectFilePath());

		if (!projectFile.good() || !json::accept(projectFile)) {
			LogManager::LogError("Could not open project file to load preferences data.");
			return;
		}

		projectFile.clear();
		projectFile.seekg(0);

		ordered_json project = ordered_json::parse(projectFile);

		projectFile.close();

		if (!project.contains("Preferences")) return;

		ordered_json preferences = project["Preferences"];

		// General
		instance->data.name = preferences["game"];
		instance->data.creator = preferences["creator"];
		instance->data.initialScene = preferences["initialScene"];
		instance->data.splashScreen = preferences["splashScreen"];


		// Window
		instance->data.windowTitle = preferences["windowTitle"];
		instance->data.icon = preferences["windowIcon"];

		std::string size = preferences["windowSize"];
		std::stringstream windowSize(size);
		std::string x, y, z;
		std::getline(windowSize, x, ',');
		std::getline(windowSize, y, ',');

		instance->data.width = std::stoi(x);
		instance->data.height = std::stoi(y);

		instance->data.vsync = preferences["vsync"];
		instance->data.fullscreen = preferences["fullScreen"];
		instance->data.showCursor = preferences["showcursor"];

		std::string bgColor = preferences["bgColor"];
		std::stringstream backgroundColor(bgColor);
		std::getline(backgroundColor, x, ',');
		std::getline(backgroundColor, y, ',');
		std::getline(backgroundColor, z, ',');

		instance->data.bgColor.r = std::stoi(x) / 255.0f;
		instance->data.bgColor.g = std::stoi(y) / 255.0f;
		instance->data.bgColor.b = std::stoi(z) / 255.0f;
		instance->data.bgColor.a = preferences["bgAlpha"] / 255.0f;

		// Physics
		std::string gravity = preferences["gravity"];
		std::stringstream worldGravity(gravity);
		std::getline(worldGravity, x, ',');
		std::getline(worldGravity, y, ',');

		instance->data.gravity_x = std::stof(x);
		instance->data.gravity_y = std::stof(y);

		instance->data.layers = preferences["layers"];
		instance->nLayers = instance->data.layers.size();
		instance->data.collisionMatrix = preferences["matrix"];
		instance->data.debugPhysics = preferences["debugPhysics"];


		// Input
		instance->data.closeWithEscape = preferences["closeWithEscape"];

			// KeyBoard
			instance->data.KB_Jump = preferences["KB_Jump"];
			instance->data.KB_Action = preferences["KB_Action"];
			instance->data.KB_movements = preferences["KB_movements"].get<std::vector<int>>();

			// Controller
			instance->data.CT_Jump = preferences["CT_Jump"];
			instance->data.CT_Action = preferences["CT_Action"];

		// Overlay
		instance->data.timeToDoubleClick = preferences["timeToDoubleClick"];
		instance->data.timeToHoldClick = preferences["timeToHoldClick"];
		instance->data.debugFramerate = preferences["debugFrameRate"];


		// Audio
		instance->data.frequency = preferences["frequency"];
		instance->data.channels = preferences["channels"];
		instance->data.chunksize = preferences["chuncksize"];

	}

}
