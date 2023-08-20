#include "Preferences.h"

#include "ProjectsManager.h"
#include "nlohmann/json.hpp"
#include "Editor.h"
#include "imgui.h"
#include "Game.h"

#include <fstream>


using nlohmann::json;
using nlohmann::ordered_json;

namespace ShyEditor {

	Preferences* Preferences::instance = nullptr;

	Preferences::Preferences() : Window("Preferences", 0)
	{
		instance = this;

		open = false;
		visible = false;
		windowWidth = 700;
		windowHeight = 700;

		canBeDisplayedOnTop = true;

		data.width = 1920;
		data.height = 1080;
		data.initialScene = "scene";
		data.name = Editor::getInstance()->getProjectInfo().name;
		data.windowTitle = "ShyIcon.png";
		data.icon = "ShyIcon.png";
		data.creator = "Yojhan";
		data.windowTitle = "La casa de los espiritus";
		data.gravity_x = 0;
		data.gravity_y = 9.81f;
		data.vsync = true;
		data.fullScreen = false;
		data.closeWithEscape = true;
		data.timeToDoubleClick = 0.5f;
		data.timeToHoldClick = 0.1f;
	}

	void Preferences::Open()
	{
		instance->open = true;
	}


	void Preferences::render()
	{
		if (open) {

			open = false;
			visible = true;

			ImGui::SetNextWindowFocus();
		}

		if (!visible) return;

		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Once);
		ImGui::Begin(windowName.c_str(), &visible, flags);


		ImGui::TextWrapped("Hola! Estas son las preferencias. Aqui es donde estaran disponibles a modificar los "
			"distintos valores del editor. Vuelve pronto!");


		if (ImGui::CollapsingHeader("General")) {

			ImGui::Indent();

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Window")) {

			ImGui::Indent();

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Physics")) {

			ImGui::Indent();

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Input")) {

			ImGui::Indent();

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Audio")) {

			ImGui::Indent();

			ImGui::Unindent();
		}

		char buffer[256];
		memcpy(buffer, data.name.c_str(), 256);
		if (ImGui::InputText("Game name", buffer, 256)) {

			data.name = buffer;
		}
		memcpy(buffer, data.icon.c_str(), 256);
		//TODO: seleccionar un fichero en vez de poner la ruta
		if (ImGui::InputText("Game icon", buffer, 256)) {

			data.icon = buffer;
		}

		int gameSize[2]{ data.width , data.height };
		if (ImGui::DragInt2("Game size", &gameSize[0], 1, 10, INT_MAX, "%d")) {

			data.width = gameSize[0];
			data.height = gameSize[1];
		}

		memcpy(buffer, data.initialScene.c_str(), 256);
		//TODO: seleccionar un fichero en vez de poner la ruta
		if (ImGui::InputText("Initial Scene", buffer, 256)) {

			data.initialScene = buffer;
		}


		ImGui::End();
	}

	ProjectData& Preferences::GetData()
	{
		return instance->data;
	}


	void Preferences::GenerateDebug()
	{
		json root = instance->BasicData();

		root["splashScreen"] = false;
		root["debugPhysics"] = true;
		root["debugFrameRate"] = true;

		root["path"] = Editor::getInstance()->getProjectInfo().path;

		std::ofstream file("config.json");

		file << root.dump(4);

		file.close();
	}

	void Preferences::GenerateRelease()
	{
		json root = instance->BasicData();

		root["splashScreen"] = true;
		root["debugPhysics"] = false;
		root["debugFrameRate"] = false;

		root["path"] = Editor::getInstance()->getProjectInfo().path;

		std::ofstream file("config.json");

		file << root.dump(4);

		file.close();
	}


	nlohmann::ordered_json Preferences::BasicData()
	{
		nlohmann::ordered_json root = nlohmann::ordered_json();

		root["game"] = data.name;
		root["creator"] = data.creator;
		root["windowTitle"] = data.windowTitle;
		root["initialScene"] = data.initialScene;
		root["windowIcon"] = data.icon;
		root["windowSize"] = std::to_string(data.width) + ", " + std::to_string(data.height);
		root["gravity"] = std::to_string(data.gravity_x) + ", " + std::to_string(data.gravity_y);

		root["vsync"] = data.vsync;
		root["fullScreen"] = data.fullScreen;
		root["closeWithScape"] = data.closeWithEscape;
		root["timeToDoubleClick"] = data.timeToDoubleClick;
		root["timeToHoldClick"] = data.timeToHoldClick;

		return root;
	}

}