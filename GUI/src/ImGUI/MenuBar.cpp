#include "MenuBar.h"
#include "imgui.h"
#include "Window.h"
#include "ImGUIManager.h"
#include "Scene.h"
#include "GameObject.h"
#include <fstream>

PEditor::MenuBar::MenuBar() : Window("", None)
{
    originalFramePadding = ImGui::GetStyle().FramePadding.y;
    
    //Width is 0 cause menu bar always take the whole screen width
    windowWidth = windowOriWidth = 0;
    windowHeight = windowOriHeight = 4;

    imGuiManager = ImGUIManager::getInstance();


    if (ImGui::BeginPopup("Rename Object")) {
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    shouldOpenRenamePopup = false;
}

void PEditor::MenuBar::render()
{
    // Change the menu bar height temporarily
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, windowHeight));  // Change the menu bar height to 10 pixels

    GameObject* gameObject = imGuiManager->getScene()->getSelectedGameObject();

    if (ImGui::BeginMainMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Scene", NULL, false);
            ImGui::MenuItem("Open Scene", NULL, false);
            ImGui::Separator();
            if (ImGui::MenuItem("Save Scene", NULL, false)) {
                ImGUIManager::getInstance()->getScene()->saveScene();
            }

            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit", NULL, false))
            {
                imGuiManager->exit();
            };

            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Play", NULL, false))
            {
                ImGUIManager::getInstance()->getScene()->saveScene();
                system("C:/Users/Ivanof18/Desktop/TFG/Skeleton/exes/Main_Debug.exe");
            };

            ImGui::MenuItem("Pause", NULL, false);
            ImGui::MenuItem("Stop", NULL, false);

            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("About us", NULL, false);
            ImGui::EndMenu();
        }

 
        if (gameObject != nullptr)
        {
            ImGui::Separator();
            if (ImGui::BeginMenu("GameObject"))
            {

                if (ImGui::MenuItem("Rename", NULL, false))
                {
                    shouldOpenRenamePopup = true;
                }

                ImGui::Separator();
 
                if (ImGui::MenuItem("Delete", NULL, false)) {
                    gameObject->toDelete();
                }
 
                if (ImGui::MenuItem("Add script", NULL, false)) {
                    ImGUIManager::getInstance()->creatingScript(true);
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMainMenuBar();
    }

    // Reset the menu bar height to the default value
    ImGui::PopStyleVar();  // Restore the default menu bar height

    if (shouldOpenRenamePopup)
    {
        ImGui::OpenPopup("Rename Object");
        shouldOpenRenamePopup = false;
    }

    if (ImGui::BeginPopup("Rename Object"))
    {
        ImGui::Text(("Insert new name for GameObject: " + gameObject->getName()).c_str());

        ImGui::Separator();

        static char nameBuffer[256];  // Buffer to hold the new name

        // Display an input text field for renaming
        if (ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer)))
        {
            // The text has been edited, update the game object's name
            gameObject->setName(nameBuffer);
        }

        if (ImGui::Button("Ok"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();

    }
}

void PEditor::MenuBar::update()
{
    ImGUIManager* manager = ImGUIManager::getInstance();
    ImVec2 originalSize = manager->getOriginalWindowSize();
    ImVec2 currentSize = manager->getMainWindowSize();

    float scaleFactorY = currentSize.y / originalSize.y;

    windowHeight = windowOriHeight * scaleFactorY * scaleFactorY * 0.88f;
}
