#include "FileExplorer.h"

#include "ProjectsManager.h"
#include "nlohmann/json.hpp"
#include "ScriptCreation.h"
#include "Entity.h"
#include "Components.h"

#include "ResourcesManager.h"
#include "LogManager.h"
#include "Hierarchy.h"
#include "SDL_image.h"
#include "Texture.h"
#include "Editor.h"
#include "Scene.h"
#include "SDL.h"

#include <filesystem>
#include <direct.h>
#include <imgui.h>
#include <queue>
#include <fstream>
#include <Windows.h>

#include "ResourcesManager.h"

#include "CheckML.h"

#define FolderImage "folder.png"
#define FileImage "file.png"
#define ScriptImage "script.png"
#define SceneImage "scene3.png"

namespace fs = std::filesystem;

namespace ShyEditor {

	FileExplorer::FileExplorer() : Window("File Explorer", ImGuiWindowFlags_NoCollapse) {

		editor = Editor::GetInstance();
		assetPath = editor->GetProjectInfo().assetPath;
		currentPath = assetPath;
		relativePath = "";

		folder = ResourcesManager::GetInstance()->AddTexture(FolderImage, true);
		file = ResourcesManager::GetInstance()->AddTexture(FileImage, true);
		script = ResourcesManager::GetInstance()->AddTexture(ScriptImage, true);
		scene = ResourcesManager::GetInstance()->AddTexture(SceneImage, true);

		docked = true;
		viewMode = 0;

		shouldOpenDeleteFilePopup = false;
		shouldOpenFileMenu = false;

		ProcessPath();
	}


	void FileExplorer::ProcessPath() {

		currentlySelected = -1;
		entries.clear();

		fs::path explorerFolder(currentPath);

		// Checks if the current file explore path is a directory
		if (!fs::is_directory(explorerFolder)) {
			LogManager::LogError("The current file explorer path is not a directory");
			return;
		}

		relativePath = explorerFolder.lexically_relative(assetPath).string();

		if (relativePath == ".")
			relativePath = "";
		else relativePath += "\\";

		std::queue<Entry> files;

		// Iterate through the current file explorer directory
		for (auto& explorerFile : fs::directory_iterator(explorerFolder)) {

			std::filesystem::path path = explorerFile.path();

			Entry entry;
			entry.isFolder = explorerFile.is_directory();
			entry.path = path.string();
			entry.name = path.filename().string();

			if (entry.isFolder) {
				entry.texture = folder;
				entry.extension = "";
				entries.push_back(entry);
			}
			else {
				entry.extension = path.extension().string();

				std::string imageFile = entry.name;

				// Remove the file extension to keep only the name
				entry.name = entry.name.substr(0, entry.name.find_last_of('.'));

				if (entry.extension == ".png" || entry.extension == ".jpg")
					entry.texture = ResourcesManager::GetInstance()->AddTexture(relativePath + entry.name + entry.extension, false);
				else if (entry.extension == ".scene")
					entry.texture = scene;
				else if (entry.extension == ".script")
					entry.texture = script;
				else
					entry.texture = file;

				files.push(entry);
			}
		}


		while (!files.empty())
		{
			entries.push_back(files.front());
			files.pop();
		}
		
		shouldUpdate = false;
	}

	void FileExplorer::Behaviour() {

		if (shouldUpdate)
			ProcessPath();

		std::filesystem::path currentDirectory(currentPath);

		ImGui::Text("Folder: %s", currentPath.c_str());

		if (currentPath != assetPath) {

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
		if (ImGui::Button("Refresh"))
			shouldUpdate = true;

		ImGui::SameLine();

		if (ImGui::Button("Open file explorer"))
			ShellExecuteA(NULL, "explore", currentPath.c_str(), NULL, NULL, SW_SHOWNORMAL);

		ImGui::Separator();

		if (viewMode == 0)
			DrawList();
		else
			DrawIcons();

		ShowDeleteFilePopup();
		ShowFileMenuPopup();
	}


	void FileExplorer::DrawList() {

		const float iconSize = ImGui::GetTextLineHeight() + 8;

		// TODO: cambiar el color para las carpetas
		// ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 1.0f, 1.0f));

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

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1) && currentlySelected != -1) {
					shouldOpenFileMenu = true;
				}
			}


			if (ImGui::BeginDragDropSource()) {



				Asset asset{};
				memcpy(asset.extension, entry.extension.c_str(), 256);
				memcpy(asset.name, entry.name.c_str(), 256);
				memcpy(asset.path, entry.path.c_str(), 256);
				memcpy(asset.relativePath, (relativePath + asset.name + asset.extension).c_str(), 256);

				ImGui::SetDragDropPayload("Asset", &asset, sizeof(asset));


				ImGui::Image(entry.texture->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
				ImGui::SameLine();
				ImGui::Text(asset.name);


				ImGui::EndDragDropSource();
			}

			ImGui::SameLine();

			int yCursor = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(yCursor - 5);
			ImGui::Image(entry.texture->getSDLTexture(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1));
			ImGui::SetCursorPosY(yCursor);

			ImGui::SameLine();
			ImGui::Text((entry.name + entry.extension).c_str());

			idx++;
		}

		ImGui::Unindent();
	}

	void FileExplorer::DrawIcons() {

	}

	void FileExplorer::OnItemSelected(Entry& entry) {

		if (ImGui::IsMouseDoubleClicked(0)) {

			if (entry.isFolder) {

				if (currentPath != assetPath)
					relativePath += "\\" + entry.name;

				currentPath = entry.path;
				shouldUpdate = true;

			}

			else {

				if (entry.extension == ".script") {

					editor->OpenScript(entry.name);
				}
				else if (entry.extension == ".scene") {

					/*std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();*/
					editor->GetScene()->LoadScene(entry.name);
				}

				else {
					HINSTANCE hInstance = ShellExecuteA(NULL, "open", (LPCSTR) entry.path.c_str(), NULL, NULL, SW_SHOWNORMAL);

					// Check for ShellExecuteA() errors
					if ((intptr_t)hInstance <= 32) {
						DWORD errorCode = (DWORD)hInstance;
						LogManager::LogError("ShellExecuteA() failed with error code: " + errorCode);
					}
				}
			}
		}
	
	}

	void FileExplorer::ItemDrag(Entry& entry) {

		if (entry.isFolder) return;

		Asset asset;

		//asset.extension = entry.extension;
		//asset.name = entry.name;
		//asset.path = entry.path;
		//asset.relativePath = relativePath;
		//ResourcesManager::SelectAsset(asset);

	}

	void FileExplorer::ShowDeleteFilePopup()
	{
		if (shouldOpenDeleteFilePopup) {
			shouldOpenDeleteFilePopup = false;
			ImGui::OpenPopup(("Delete file" + entryToDelete.path).c_str());
		}

		if (ImGui::BeginPopup(("Delete file" + entryToDelete.path).c_str())) {

			ImGui::Text(("Are you sure you want to delete \"" + entryToDelete.name + "\"").c_str());

			if (ImGui::Button("YES", ImVec2(300, 40))){

				fs::remove_all(entryToDelete.path.c_str());
				entries.erase(entries.begin() + currentlySelected);

				shouldUpdate = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("NO", ImVec2(300, 40))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void FileExplorer::ShowFileMenuPopup()
	{
		if (shouldOpenFileMenu) {
			shouldOpenFileMenu = false;
			ImGui::OpenPopup(("File Menu##" + entryToDelete.path).c_str());
		}

		if (ImGui::BeginPopup(("File Menu##" + entryToDelete.path).c_str())) {

			if (ImGui::Button(("Delete##" + entryToDelete.path).c_str(), ImVec2(70, 40))) {
				entryToDelete = entries[currentlySelected];
				shouldOpenDeleteFilePopup = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}



	void FileExplorer::HandleInput(SDL_Event* event) {

		if (event->type == SDL_DROPFILE) {

			std::string sourcePath = event->drop.file;

			if (IsMouseHoveringWindow()) {

				std::filesystem::path src = fs::path(sourcePath);
				std::filesystem::path dst = fs::path(currentPath + "\\" + src.filename().string());

				if (!std::filesystem::exists(dst)) {
					bool ret = std::filesystem::copy_file(src, dst);

					if (ret) {

						shouldUpdate = true;
					}
					else {

						std::cout << "Error al copiar el fichero: " << std::endl;
						std::cout << "Origen: " << src.string() << std::endl;
						std::cout << "Destino: " << dst.string() << std::endl;
					}
				}
				else {
					std::cout << "File already exists in the destination." << std::endl;
				}
			}

			SDL_free(event->drop.file);

		}
		else if (event->type == SDL_KEYDOWN) {
			if (event->key.keysym.sym == SDLK_DELETE) {
				if (focused && !entries.empty() && currentlySelected >= 0 && currentlySelected < entries.size()) {
					entryToDelete = entries[currentlySelected];
					shouldOpenDeleteFilePopup = true;
				}
			}
		}
	}

	void FileExplorer::Refresh()
	{
		shouldUpdate = true;
	}

}