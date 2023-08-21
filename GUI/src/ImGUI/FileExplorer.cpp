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

	FileExplorer::FileExplorer() : Window("File Explorer", NoCollapse) {

		editor = Editor::getInstance();
		ImVec2 mainWindowSize = editor->getMainWindowSize();

		windowOriWidth = mainWindowSize.x;
		windowOriHeight = mainWindowSize.y * FILEEXPLORER_WIN_HEIGHT_RATIO;

		windowOriPosX = 0;
		windowOriPosY = mainWindowSize.y - windowOriHeight;

		setSize(ImVec2(windowOriWidth, windowOriHeight));
		setPosition(ImVec2(windowOriPosX, windowOriPosY));

		projectPath = editor->getProjectInfo().path;
		currentPath = projectPath;

		folder = ResourcesManager::GetInstance()->AddTexture(FolderImage, true);
		file = ResourcesManager::GetInstance()->AddTexture(FileImage, true);
		script = ResourcesManager::GetInstance()->AddTexture(ScriptImage, true);
		
		docked = true;
	}

	void FileExplorer::drawFileExplorerWindow()
	{
		// Open the specified folder

		fs::path explorerFolder(currentPath);

		if (!fs::is_directory(explorerFolder))
		{
			ImGui::Text("Invalid folder path: %s", currentPath);
			return;
		}

		ImGui::Text("Folder: %s", currentPath.c_str());

		if (currentPath != projectPath) {

			// Display buttons to navigate up and down the folder hierarchy
			ImGui::SameLine();
			if (ImGui::Button("^"))
			{
				// Navigate to parent folder
				currentPath = explorerFolder.parent_path().string();
			}
		}

		ImGui::Separator();

		// Display a list of all directories in the folder
		for (auto& explorerFile : fs::directory_iterator(explorerFolder))
		{
			if (explorerFile.is_directory())
			{
				std::string filename = explorerFile.path().filename().string();

				ImTextureID texture = explorerFile.is_directory() ? folder->getSDLTexture() : file->getSDLTexture();

				if (explorerFile.path().extension().string() == ".script") 
					texture = script->getSDLTexture();


				const float iconSize = ImGui::GetTextLineHeight() + 8;
				ImGui::Image(texture, ImVec2(iconSize, iconSize), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

				ImGui::SameLine();

				ImGui::SetWindowFontScale(1.5);

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 1.0f, 1.0f));

				// Display directories in blue color
				if (ImGui::Selectable(filename.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
				{

					if (ImGui::IsMouseDoubleClicked(0))
						currentPath = explorerFile.path().string();
				}

				ImGui::PopStyleColor(1);
			}

			ImGui::SetWindowFontScale(1);
		}

		// Display a list of all files in the folder
		for (auto& explorerFile : fs::directory_iterator(explorerFolder))
		{
			if (!explorerFile.is_directory())
			{
				std::string filename = explorerFile.path().filename().string();

				ImTextureID texture = explorerFile.is_directory() ? folder->getSDLTexture() : file->getSDLTexture();

				if (explorerFile.path().extension().string() == ".script") 
					texture = script->getSDLTexture();

				const float iconSize = ImGui::GetTextLineHeight() + 8;
				ImGui::Image(texture, ImVec2(iconSize, iconSize), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

				ImGui::SameLine();

				ImGui::SetWindowFontScale(1.5);


				std::string path = explorerFile.path().string();
				size_t dotPos = filename.find_last_of(".");
				std::string filenameWithoutExtension = filename.substr(0, dotPos);

				std::string extension = explorerFile.path().extension().string();
				// Display files in default color
				if (ImGui::Selectable(filename.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {

					if (ImGui::IsMouseDoubleClicked(0)) {

						if (extension == ".script") {

							editor->OpenScript(filenameWithoutExtension);
						}
						else if (extension == ".scene") {

							std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();
							editor->getScene()->loadScene(relativePath);

						}

						else
							ShellExecuteA(NULL, "open", (LPCSTR)explorerFile.path().string().c_str(), NULL, NULL, SW_SHOWNORMAL);
					}

				}
				else {

					if (ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
					{
						std::string relativePath = explorerFile.path().lexically_relative(projectPath + "/Images").string();


						Asset asset;

						asset.extension = extension;
						asset.name = filenameWithoutExtension;
						asset.path = path;
						asset.relativePath = relativePath;
						ResourcesManager::SelectAsset(asset);

					}

				}



				if (extension == ".png" || extension == ".jpg")
				{
					int rel = fs::path(projectPath + "/Images").compare(explorerFile.path().parent_path());
					if (rel <= 0)
					{
						std::string relativePath = explorerFile.path().lexically_relative(projectPath + "/Images").string();
						std::string buttonId = "Create GameObject##" + relativePath;
						ImGui::SameLine();

						if (ImGui::Button(buttonId.c_str())) {
							editor->getScene()->addGameObject(relativePath);
						}
					}
				}

				if (extension == ".script")
				{
					ImGui::SameLine();
					std::string buttonId = "Open script##" + filename;
					if (ImGui::Button(buttonId.c_str())) {

						ImGui::OpenPopup("Create script");
						size_t dotPos = filename.find_last_of(".");
						std::string filenameWithoutExtension = filename.substr(0, dotPos);
						editor->OpenScript(filenameWithoutExtension);
					}
				}

				if (extension == ".scene") {

					ImGui::SameLine();
					std::string buttonId = "Load scene##" + filename;
					std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();
					if (ImGui::Button(buttonId.c_str())) {
						editor->getScene()->loadScene(relativePath);
					}
				}

				if (extension == ".prefab") {

					ImGui::SameLine();
					std::string buttonId = "Create Gameobject from prefab##" + filename;
					if (ImGui::Button(buttonId.c_str())) {
						std::string relativePath = explorerFile.path().lexically_relative(projectPath).string();

						std::ifstream inputFile(relativePath);

						if (!inputFile.is_open()) {
							std::cerr << "Error opening JSON file: " << relativePath << std::endl;
							return;
						}


						nlohmann::ordered_json jsonData;
						try {
							inputFile >> jsonData;
						}
						catch (const nlohmann::json::parse_error& e) {
							std::cerr << "JSON parse error: " << e.what() << std::endl;
							return;
						}

						inputFile.close();

						GameObject* go = GameObject::fromJson(jsonData.dump(), true);

						editor->getScene()->addGameObject(go);
					}
				}
			}

			ImGui::SetWindowFontScale(1);
		}

	}

	void FileExplorer::Behaviour()
	{
		drawFileExplorerWindow();
	}

}
