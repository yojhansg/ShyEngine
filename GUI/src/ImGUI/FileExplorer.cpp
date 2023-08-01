#include "FileExplorer.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "Scene.h"
#include <filesystem>
#include "imgui.h"
#include <direct.h>
#include "SDL.h"
#include "SDL_image.h"

namespace fs = std::filesystem;


PEditor::FileExplorer::FileExplorer() : Window("FileExplorer", NoResize | NoCollapse | NoMove)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(mainWindowSize.x, 297 * mainWindowSize.y / 1080);

    windowOriWidth = windowSize.x;
    windowOriHeight = windowSize.y;

	setSize(ImVec2(windowSize.x, windowSize.y));
	setPosition(ImVec2(0, 784 * mainWindowSize.y / 1080));

    windowOriPosX = windowPosX;
    windowOriPosY = windowPosY;

    char buffer[FILENAME_MAX];
    _getcwd(buffer, FILENAME_MAX);
    projectPath = buffer;
    currentPath = projectPath;

}

void PEditor::FileExplorer::drawFileExplorerWindow()
{

    // Open the specified folder
    fs::path folder(currentPath);


    if (!fs::is_directory(folder))
    {
        ImGui::Text("Invalid folder path: %s", currentPath);
        ImGui::End();
        return;
    }

    ImGui::Text("Folder: %s", currentPath.c_str());

    if (currentPath != projectPath) {

        // Display buttons to navigate up and down the folder hierarchy
        ImGui::SameLine();
        if (ImGui::Button("^"))
        {
            // Navigate to parent folder
            currentPath = folder.parent_path().string();
        }
    }

    ImGui::Separator();

    // Display a list of files and directories in the folder
    for (auto& file : fs::directory_iterator(folder))
    {
        std::string filename = file.path().filename().string();

        std::string path = file.is_directory() ? "folder.png" : "file.png";

        if (file.path().extension().string() == ".script") path = "script.png";


        SDL_Surface* surface = IMG_Load(path.c_str());
        ImTextureID imageTextId = SDL_CreateTextureFromSurface(ImGUIManager::getInstance()->getRenderer(), surface);

        SDL_FreeSurface(surface);

        const float iconSize = ImGui::GetTextLineHeight() + 8;
        ImGui::Image(imageTextId, ImVec2(iconSize, iconSize), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

        ImGui::SameLine();

        ImGui::SetWindowFontScale(1.5);

        if (file.is_directory())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 1.0f, 1.0f)); 

            // Display directories in blue color
            if (ImGui::Selectable(filename.c_str(), false))
            {
                currentPath = file.path().string();
            }

            ImGui::PopStyleColor(1);
        }
        else
        {
            // Display files in default color
            ImGui::Text(filename.c_str());
            std::string extension = file.path().extension().string();

            if (extension == ".png" || extension == ".jpg")
            {
                ImGui::SameLine();
                std::string buttonId = "Create GameObject##" + filename;
                if (ImGui::Button(buttonId.c_str())) {
                    ImGUIManager::getInstance()->getScene()->addGameObject(filename);
                }
            }

            if (extension == ".script")
            {
                ImGui::SameLine();
                std::string buttonId = "Open script##" + filename;
                if (ImGui::Button(buttonId.c_str())) {
                    ImGui::OpenPopup("Create script");
                    ImGUIManager::getInstance()->creatingScript(true);
                }
            }
           
        }

        ImGui::SetWindowFontScale(1);
    }

}

void PEditor::FileExplorer::render()
{

    // Draw the file explorer
    ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);

    ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

    ImGui::SetWindowPos(ImVec2(windowPosX, windowPosY));

    drawFileExplorerWindow();

    ImGui::End();

}
