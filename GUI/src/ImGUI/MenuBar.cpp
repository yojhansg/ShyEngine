#include "MenuBar.h"
#include "imgui.h"
#include "Window.h"
#include "ImGUIManager.h"
#include "Scene.h"
#include "GameObject.h"
#include <fstream>
#include <Windows.h>
#include <tchar.h>

#include "Game.h"
#include "Preferences.h"
#include "ColorPalette.h"

PEditor::MenuBar::MenuBar() : Window("", None)
{

    imGuiManager = ImGUIManager::getInstance();

    shouldOpenRenamePopup = false;
}

void PEditor::MenuBar::render()
{

    GameObject* gameObject = imGuiManager->getScene()->getSelectedGameObject();

    if (ImGui::BeginMainMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Scene", NULL, false);
            ImGui::MenuItem("Open Scene", NULL, false);
            ImGui::Separator();
            if (ImGui::MenuItem("Save Scene", NULL, false)) {

                shouldOpenSaveScenePopup = true;
            }

            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit", NULL, false))
            {
                imGuiManager->exit();
            };

            ImGui::EndMenu();
        }

        ImGui::Separator();


        if (ImGui::BeginMenu("Edit")) {


            if (ImGui::MenuItem("Preferences")) {

                Preferences::Open();
            }

            if (ImGui::MenuItem("Theme selector")) {

                ColorPalette::Open();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Game"))
        {
            if (ImGui::MenuItem("Play", NULL, false))
            {
                ImGUIManager::getInstance()->getScene()->saveScene("Scenes/scene.scene");
                Game::Play();
            };

            if (ImGui::MenuItem("Stop", NULL, false)) {
                
                Game::Stop();
            }

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
                if (ImGui::MenuItem("Create prefab", NULL, false)) {

                }

                if (ImGui::MenuItem("Add script", NULL, false)) {
                    ImGUIManager::getInstance()->changeEditorState(ImGUIManager::EDITOR_STATE::SCRIPTING_WINDOW);
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Rename", NULL, false))
                {
                    shouldOpenRenamePopup = true;
                }

                ImGui::Separator();
 
                if (ImGui::MenuItem("Delete", NULL, false)) {
                    gameObject->toDelete();
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMainMenuBar();
    }

    showRenamePopup(gameObject);
    showSaveScenePopup();
}

void PEditor::MenuBar::showRenamePopup(GameObject* gameObject)
{
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
        }

        if (ImGui::Button("Ok"))
        {
            if (strlen(nameBuffer) > 0) {
                gameObject->setName(nameBuffer);
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void PEditor::MenuBar::showSaveScenePopup()
{
    if (shouldOpenSaveScenePopup)
    {
        ImGui::OpenPopup("Save scene");
        shouldOpenSaveScenePopup = false;
    }

    if (ImGui::BeginPopup("Save scene"))
    {
        ImGui::Text(("Insert name for the scene:"));

        ImGui::Separator();

        static char nameBuffer[256];  // Buffer to hold the new name

        // Display an input text field for renaming
        if (ImGui::InputText("Scene name", nameBuffer, sizeof(nameBuffer)))
        {
        }

        if (ImGui::Button("Ok"))
        {
            if (strlen(nameBuffer) > 0) {
                imGuiManager->getScene()->saveScene("Scenes/" + std::string(nameBuffer) + ".scene");
            }
            else {
                imGuiManager->getScene()->saveScene("Scenes/scene.scene");
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
