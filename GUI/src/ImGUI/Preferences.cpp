#include "Preferences.h"

#include "ProjectsManager.h"
#include "nlohmann/json.hpp"
#include "Editor.h"
#include "imgui.h"
#include "Game.h"
#include "PrefabManager.h"

#include <fstream>

#include "CheckML.h"

using nlohmann::json;
using nlohmann::ordered_json;

const int MAX_COLLISION_LAYERS = 16;

namespace ShyEditor {

	Preferences* Preferences::instance = nullptr;

	Preferences::Preferences() : Window("Preferences", 0)
	{
		Hide();
		instance = this;

		open = false;
		windowWidth = 700;
		windowHeight = 700;
		canBeDisplayedOnTop = true;
		updateMatrix = false;
		showError = false;
		errorMsg = L"Default error message";
		defaultLayer = "Default";
		nLayers = 1;

		// General
		data.initialScene = "\\Scene";
		data.creator = "Yojhan, Pablo e Ivan";
		data.name = Editor::GetInstance()->GetProjectInfo().name;

		// Window
		data.windowTitle = Editor::GetInstance()->GetProjectInfo().name;
		data.icon = "";
		data.width = 1920;
		data.height = 1080;
		data.vsync = true;
		data.showCursor = true;
		data.fullscreen = false;

		// Physics
		data.gravity_x = 0;
		data.gravity_y = 9.81f;
		data.collisionMatrix = std::vector<std::vector<bool>>(MAX_COLLISION_LAYERS, std::vector<bool>(MAX_COLLISION_LAYERS, false));
		data.layers.push_back("Default");

		// Input
		data.closeWithEscape = true;

		// Overlay
		data.timeToDoubleClick = 0.5f;
		data.timeToHoldClick = 0.1f;

		// Audio
		data.frequency = 44100;
		data.channels = 1;
		data.chunksize = 2048;


	}

	void Preferences::Open()
	{
		instance->Show();

	}


	void Preferences::Behaviour()
	{
		if (ImGui::CollapsingHeader("General")) {
			GeneralHeader();
		}

		if (ImGui::CollapsingHeader("Window")) {
			WindowHeader();
		}

		if (ImGui::CollapsingHeader("Physics")) {
			PhysicsHeader();
		}

		if (ImGui::CollapsingHeader("Input")) {
			InputHeader();
		}

		if (ImGui::CollapsingHeader("Overlay")) {
			OverlayHeader();
		}

		if (ImGui::CollapsingHeader("Audio")) {
			AudioHeader();
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


		// Window size
		ImGui::Text("Game Window Size");
		ImGui::SameLine();
		int gameSize[2]{ data.width , data.height };
		if (ImGui::DragInt2("##Gamesize", &gameSize[0], 1, 10, INT_MAX, "%d")) {
			data.width = gameSize[0];
			data.height = gameSize[1];
		}

		// Start fullscreen
		ImGui::Text("Start fullscreen");
		ImGui::SameLine();
		ImGui::Checkbox("##fullscreen", &data.fullscreen);


		// Start showing cursor
		ImGui::Text("Start showing cursor");
		ImGui::SameLine();
		ImGui::Checkbox("##showcursor", &data.showCursor);

		ImGui::Unindent();

	}

	void Preferences::AudioHeader() {

		ImGui::Indent();

		ImGui::Text("Frequency");
		ImGui::SameLine();
		ImGui::InputInt("##frequency", &data.frequency);

		ImGui::Text("Channels");
		ImGui::SameLine();
		ImGui::RadioButton("Mono", &data.channels, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Stereo", &data.channels, 1);

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

		ImGui::Unindent();
	}

	void Preferences::OverlayHeader() {
		ImGui::Indent();

		ImGui::Text("Time to Double Click");
		ImGui::SameLine();
		ImGui::DragFloat("##timetodoubleclick", &data.timeToDoubleClick);

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

		root["path"] = Editor::GetInstance()->GetProjectInfo().path + "\\Assets";

		PrefabManager::SavePrefabs(root["path"]);

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

		root["path"] = instance->data.buildPath;
		PrefabManager::SavePrefabs(root["path"]);

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

		// Window
		root["windowTitle"] = data.windowTitle;
		root["windowIcon"] = data.icon;
		root["windowSize"] = std::to_string(data.width) + ", " + std::to_string(data.height);
		root["vsync"] = data.vsync;
		root["fullScreen"] = data.fullscreen;
		root["showcursor"] = data.showCursor;

		// Physics
		root["gravity"] = std::to_string(data.gravity_x) + ", " + std::to_string(data.gravity_y);
		root["layers"] = data.layers;
		root["matrix"] = data.collisionMatrix;

		// Input
		root["closeWithEscape"] = data.closeWithEscape;

		// Overlay
		root["timeToDoubleClick"] = data.timeToDoubleClick;
		root["timeToHoldClick"] = data.timeToHoldClick;

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

}
