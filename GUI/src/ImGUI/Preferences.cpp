#include "Preferences.h"

#include "ProjectsManager.h"
#include "nlohmann/json.hpp"
#include "Editor.h"
#include "imgui.h"
#include "Game.h"

#include <fstream>

#include "CheckML.h"


using nlohmann::json;
using nlohmann::ordered_json;

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

		data.width = 1920;
		data.height = 1080;
		data.initialScene = "\\Scene";
		data.name = Editor::getInstance()->getProjectInfo().name;
		data.windowTitle = Editor::getInstance()->getProjectInfo().name;
		data.icon = "\\Images\\ShyIcon.png";
		data.creator = "Yojhan, Pablo e Ivan";
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
		instance->visible = true;
	}


	void Preferences::Behaviour()
	{
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

		// BUILD PATH


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

		root["path"] = Editor::getInstance()->getProjectInfo().path + "\\Assets";

		std::ofstream file("Engine\\config.json");

		file << root.dump(4);

		file.close();
	}

	void Preferences::GenerateRelease()
	{
		json root = instance->BasicData();

		root["splashScreen"] = true;
		root["debugPhysics"] = false;
		root["debugFrameRate"] = false;

		root["path"] = instance->data.buildPath;

		std::ofstream file(instance->data.buildPath);

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