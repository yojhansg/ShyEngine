#include "FileExplorer.h"

#include "ProjectsManager.h"
#include "nlohmann/json.hpp"
#include "ScriptCreation.h"
#include "GameObject.h"
#include "Components.h"

#include "ResourcesManager.h"
#include "Hierarchy.h"
#include "SDL_image.h"
#include "Texture.h"
#include "Editor.h"
#include "Scene.h"
#include "SDL.h"

#include <filesystem>
#include <direct.h>
#include <imgui.h>
#include <fstream>
#include "ResourcesManager.h"
#include <Windows.h>

#define FolderImage "folder.png"
#define FileImage "file.png"
#define ScriptImage "script.png"

namespace fs = std::filesystem;

namespace ShyEditor {

	FileExplorer::FileExplorer() : Window("File Explorer", ImGuiWindowFlags_NoCollapse) {

		editor = Editor::getInstance();
		projectPath = editor->getProjectInfo().path;
		currentPath = projectPath;

		folder = ResourcesManager::GetInstance()->AddTexture(FolderImage, true);
		file = ResourcesManager::GetInstance()->AddTexture(FileImage, true);
		script = ResourcesManager::GetInstance()->AddTexture(ScriptImage, true);

		docked = true;
		viewMode = 0;


		ProcessPath();
	}


	void FileExplorer::ProcessPath() {

		currentlySelected = -1;
		entries.clear();

		fs::path explorerFolder(currentPath);

		if (!fs::is_directory(explorerFolder))
		{
			//TODO: error message
			return;
		}


		//Recorremos el explorador de ficheros en dos bucles
		//El primero buscando las carpetas y el segundo el resto de ficheros


		for (auto& explorerFile : fs::directory_iterator(explorerFolder))
		{
			if (!explorerFile.is_directory()) continue;

			std::filesystem::path path = explorerFile.path();

			Entry entry;
			entry.isFolder = true;
			entry.path = path.string();
			entry.name = path.filename().string();
			entry.texture = folder;
			entries.push_back(entry);
		}


		for (auto& explorerFile : fs::directory_iterator(explorerFolder))
		{
			if (explorerFile.is_directory()) continue;

			std::filesystem::path path = explorerFile.path();

			Entry entry;
			entry.isFolder = false;
			entry.path = path.string();
			entry.name = path.filename().string();

			entry.name = entry.name.substr(0, entry.name.find_last_of('.'));

			entry.extension = path.extension().string();

			if (entry.extension == ".png" || entry.extension == ".jpg") {

				entry.texture = ResourcesManager::GetInstance()->AddTexture(entry.name + entry.extension, false);
			}
			else {
				entry.texture = file;
			}

			entries.push_back(entry);
		}

		shouldUpdate = false;
	}

	//void FileExplorer::drawFileExplorerWindow()
	//{
	//	// Open the specified folder

	//	fs::path explorerFolder(currentPath);

	//	if (!fs::is_directory(explorerFolder))
	//	{
	//		ImGui::Text("Invalid folder path: %s", currentPath);
	//		return;
	//	}

	//	ImGui::Text("Folder: %s", currentPath.c_str());

	//	if (currentPath != projectPath) {

	//		// Display buttons to navigate up and down the folder hierarchy
	//		ImGui::SameLine();
	//		if (ImGui::Button("^"))
	//		{
	//			// Navigate to parent folder
	//			currentPath = explorerFolder.parent_path().string();
	//		}
	//	}

	//	ImGui::Separator();

	//	// Display a list of all directories in the folder
	//	for (auto& explorerFile : fs::directory_iterator(explorerFolder))
	//	{
	//		if (explorerFile.is_directory())
	//		{
	//			std::string filename = explorerFile.path().filename().string();

	//			ImTextureID texture = explorerFile.is_directory() ? folder->getSDLTexture() : file->getSDLTexture();

	//			if (explorerFile.path().extension().string() == ".script")
	//				texture = script->getSDLTexture();


	//			ImGui::Image(texture, ImVec2(iconSize, iconSize), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

	//			ImGui::SameLine();

	//			ImGui::SetWindowFontScale(1.5);

	//			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 1.0f, 1.0f));

	//			// Display directories in blue color
	//			if (ImGui::Selectable(filename.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
	//			{

	//				if (ImGui::IsMouseDoubleClicked(0))
	//					currentPath = explorerFile.path().string();
	//			}

	//			ImGui::PopStyleColor(1);
	//		}

	//		ImGui::SetWindowFontScale(1);
	//	}

	//	// Display a list of all files in the folder
	//	for (auto& explorerFile : fs::directory_iterator(explorerFolder))
	//	{
	//		if (!explorerFile.is_directory())
	//		{
	//			std::string filename = explorerFile.path().filename().string();

	//			ImTextureID texture = explorerFile.is_directory() ? folder->getSDLTexture() : file->getSDLTexture();

	//			if (explorerFile.path().extension().string() == ".script")
	//				texture = script->getSDLTexture();

	//			const float iconSize = ImGui::GetTextLineHeight() + 8;
	//			ImGui::Image(texture, ImVec2(iconSize, iconSize), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

	//			ImGui::SameLine();

	//			ImGui::SetWindowFontScale(1.5);


	//			std::string path = explorerFile.path().string();
	//			size_t dotPos = filename.find_last_of(".");
	//			std::string filenameWithoutExtension = filename.substr(0, dotPos);

	//			std::string extension = explorerFile.path().extension().string();
	//			// Display files in default color
	//			if (ImGui::Selectable(filename.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {

	//				if (ImGui::IsMouseDoubleClicked(0)) {

	//					if (extension == ".script") {

	//						editor->OpenScript(filenameWithoutExtension);
	//					}
	//					else if (extension == ".scene") {

	//						std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();
	//						editor->getScene()->loadScene(relativePath);

	//					}

	//					else
	//						ShellExecuteA(NULL, "open", (LPCSTR)explorerFile.path().string().c_str(), NULL, NULL, SW_SHOWNORMAL);
	//				}

	//			}
	//			else {

	//				if (ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
	//				{
	//					std::string relativePath = explorerFile.path().lexically_relative(projectPath + "/Images").string();


	//					Asset asset;

	//					asset.extension = extension;
	//					asset.name = filenameWithoutExtension;
	//					asset.path = path;
	//					asset.relativePath = relativePath;
	//					ResourcesManager::SelectAsset(asset);

	//				}

	//			}



	//			if (extension == ".png" || extension == ".jpg")
	//			{
	//				int rel = fs::path(projectPath + "/Images").compare(explorerFile.path().parent_path());
	//				if (rel <= 0)
	//				{
	//					std::string relativePath = explorerFile.path().lexically_relative(projectPath + "/Images").string();
	//					std::string buttonId = "Create GameObject##" + relativePath;
	//					ImGui::SameLine();

	//					if (ImGui::Button(buttonId.c_str())) {
	//						editor->getScene()->AddGameObject(relativePath);
	//					}
	//				}
	//			}

	//			if (extension == ".script")
	//			{
	//				ImGui::SameLine();
	//				std::string buttonId = "Open script##" + filename;
	//				if (ImGui::Button(buttonId.c_str())) {

	//					ImGui::OpenPopup("Create script");
	//					size_t dotPos = filename.find_last_of(".");
	//					std::string filenameWithoutExtension = filename.substr(0, dotPos);
	//					editor->OpenScript(filenameWithoutExtension);
	//				}
	//			}

	//			if (extension == ".scene") {

	//				ImGui::SameLine();
	//				std::string buttonId = "Load scene##" + filename;
	//				std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();
	//				if (ImGui::Button(buttonId.c_str())) {
	//					editor->getScene()->loadScene(relativePath);
	//				}
	//			}

	//			if (extension == ".prefab") {

	//				ImGui::SameLine();
	//				std::string buttonId = "Create Gameobject from prefab##" + filename;
	//				if (ImGui::Button(buttonId.c_str())) {
	//					std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();

	//					std::ifstream inputFile(relativePath);

	//					if (!inputFile.is_open()) {
	//						std::cerr << "Error opening JSON file: " << relativePath << std::endl;
	//						return;
	//					}


	//					nlohmann::ordered_json jsonData;
	//					try {
	//						inputFile >> jsonData;
	//					}
	//					catch (const nlohmann::json::parse_error& e) {
	//						std::cerr << "JSON parse error: " << e.what() << std::endl;
	//						return;
	//					}

	//					inputFile.close();

	//					GameObject* go = GameObject::fromJson(jsonData.dump(), true);

	//					editor->getScene()->AddGameObject(go);
	//				}
	//			}
	//		}

	//		ImGui::SetWindowFontScale(1);
	//	}

	//}

	void FileExplorer::Behaviour()
	{
		if (shouldUpdate)
			ProcessPath();

		std::filesystem::path currentDirectory(currentPath);

		ImGui::Text("Folder: %s", currentPath.c_str());

		if (currentPath != projectPath) {

			// Display buttons to navigate up and down the folder hierarchy
			ImGui::SameLine();
			if (ImGui::Button("^"))
			{
				// Navigate to parent folder
				currentPath = currentDirectory.parent_path().string();
				shouldUpdate = true;
			}
		}

		ImGui::SameLine();
		ImGui::RadioButton("Show list", &viewMode, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Show icons", &viewMode, 1);

		ImGui::SameLine();
		if (ImGui::Button("Refresh")) {

			shouldUpdate = true;
		}

		ImGui::Separator();

		if (viewMode == 0)
			DrawList();
		else
			DrawIcons();
	}


	void FileExplorer::DrawList()
	{

		const float iconSize = ImGui::GetTextLineHeight() + 8;

		ImGui::SetWindowFontScale(1.5f);

		//TODO: cambiar el color para las carpetas
		//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 1.0f, 1.0f));


		ImGui::Indent();

		int idx = 0;
		for (auto& entry : entries) {

			ImGui::SetNextItemAllowOverlap();
			if (ImGui::Selectable(std::string("##" + entry.path).c_str(), currentlySelected == idx, ImGuiSelectableFlags_AllowDoubleClick)) {

				currentlySelected = idx;
				OnItemSelected(entry);
			}
			else {

				if (ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
					ItemDrag(entry);
			}

			ImGui::SameLine();

			int yCursor = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(yCursor - 5);
			ImGui::Image(entry.texture->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
			ImGui::SetCursorPosY(yCursor);

			ImGui::SameLine();
			ImGui::Text(entry.name.c_str());

			idx++;
		}

		ImGui::Unindent();
	}

	void FileExplorer::DrawIcons()
	{

	}

	void FileExplorer::OnItemSelected(Entry& entry)
	{
		if (ImGui::IsMouseDoubleClicked(0)) {

			if (entry.isFolder) {

				shouldUpdate = true;
				currentPath = entry.path;
			}

			else {

				if (entry.extension == ".script") {

					editor->OpenScript(entry.name);
				}
				else if (entry.extension == ".scene") {

					/*std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();*/
					editor->getScene()->loadScene(entry.name);
				}

				else
					ShellExecuteA(NULL, "open", (LPCSTR)entry.path.c_str(), NULL, NULL, SW_SHOWNORMAL);
			}

			return;
		}


	}

	void FileExplorer::ItemDrag(Entry& entry)
	{

		if (entry.isFolder) return;


		//TODO: guardar la ruta relativa
		std::string relativePath = std::filesystem::path(entry.path).lexically_relative(projectPath + "/Images").string();

		Asset asset;

		asset.extension = entry.extension;
		asset.name = entry.name;
		asset.path = entry.path;
		asset.relativePath = relativePath;
		ResourcesManager::SelectAsset(asset);

	}



	void FileExplorer::HandleInput(SDL_Event* event)
	{

		if (event->type == SDL_DROPFILE) {

			std::string sourcePath = event->drop.file;

			if (IsMouseHoveringWindow()) {

				std::filesystem::path src = fs::path(sourcePath);
				std::filesystem::path dst = fs::path(currentPath + "/" + src.filename().string());



				bool ret = std::filesystem::copy_file(src, dst);

				if (ret) {

					shouldUpdate = true;
				}
				else {

					std::cout << "Error al copiar el fichero: " << std::endl;
					std::cout << "Origen: "  << src.string() << std::endl;
					std::cout << "Destino: " << dst.string() << std::endl;
				}

			}
			SDL_free(event->drop.file);
		}

	}

}


