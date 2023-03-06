#include "FileExplorer.h"
#include <imgui.h>
#include "ImGUIManager.h"
#include "GameObject.h"
#include "Scene.h"
#include <filesystem>
#include "imgui.h"
#include <direct.h>

namespace fs = std::filesystem;


PEditor::FileExplorer::FileExplorer() : Window("FileExplorer", NoResize | NoCollapse | NoMove)
{
	ImGUIManager* imGUIManager = ImGUIManager::getInstance();
	ImVec2 mainWindowSize = imGUIManager->getMainWindowSize();

	ImVec2 windowSize = ImVec2(mainWindowSize.x, 305 * mainWindowSize.y / 1080);

	setSize(ImVec2(windowSize.x, windowSize.y));
	setPosition(ImVec2(0, 776 * mainWindowSize.y / 1080));

    char buffer[FILENAME_MAX];
    _getcwd(buffer, FILENAME_MAX);
    std::string project_path(buffer);
    current_path = project_path;

}

void PEditor::FileExplorer::drawFileExplorerWindow()
{

    // Open the specified folder
    fs::path folder(current_path);


    if (!fs::is_directory(folder))
    {
        ImGui::Text("Invalid folder path: %s", current_path);
        ImGui::End();
        return;
    }

    ImGui::Text("Folder: %s", current_path.c_str());

    // Display buttons to navigate up and down the folder hierarchy
    ImGui::SameLine();
    if (ImGui::Button("^"))
    {
        // Navigate to parent folder
        current_path = folder.parent_path().string();
    }



    ImGui::Separator();

    // Display a list of files and directories in the folder
    for (auto& file : fs::directory_iterator(folder))
    {
        std::string filename = file.path().filename().string();

        std::string path = file.is_directory() ? "folder.png" : "file.png";
        PEditor::GameObject* gameObject = new  PEditor::GameObject(path);
        ImTextureID imageTextId = (ImTextureID)gameObject->getTexture();
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
                current_path = file.path().string();
            }

            ImGui::PopStyleColor(1);
        }
        else
        {
            // Display files in default color
            if (ImGui::Selectable(filename.c_str(), false))
            {
                // TODO: Handle file selection
            }
        }

        ImGui::SetWindowFontScale(1);
    }

}

void PEditor::FileExplorer::render()
{

    // Draw the file explorer
    ImGui::Begin(windowName.c_str(), (bool*)0, (ImGuiWindowFlags_)flags);

    ImGui::SetWindowSize(ImVec2(width, height));

    ImGui::SetWindowPos(ImVec2(posX, posY));


    //for (auto component : components)
    //{
    //    component->render();
    //}

    drawFileExplorerWindow();

    ImGui::End();

}
