#include "MenuBar.h"

#include "nlohmann/json.hpp"
#include "ColorPalette.h"
#include "Preferences.h"
#include "GameObject.h"
#include "Editor.h"
#include "Scene.h"
#include "imgui.h"
#include "Game.h"
#include "WindowLayout.h"
#include "PrefabManager.h"

#include <Windows.h>
#include <fstream>
#include <tchar.h>

#include "CheckML.h"


namespace ShyEditor {

    MenuBar::MenuBar()
    {
        editor = Editor::getInstance();

        shouldOpenRenamePopup = false;
        shouldOpenSavePrefabPopup = false;
        shouldOpenNewScenePopup = false;
    }

    void MenuBar::Update()
    {

        GameObject* gameObject = editor->getScene()->GetSelectedGameObject();

        if (ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New scene", NULL, false)) {

                    shouldOpenNewScenePopup = true;
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Save Scene", NULL, false)) {

                    editor->getScene()->saveScene(editor->getScene()->getSceneName());
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit", NULL, false))
                {

                    Editor::getInstance()->End();
                };

                ImGui::EndMenu();
            }

            ImGui::Separator();


            if (ImGui::BeginMenu("Edit")) {


                if (ImGui::MenuItem("Preferences")) {

                    Preferences::Open();
                }

                if (ImGui::MenuItem("Prefab manager")) {

                    PrefabManager::Open();
                }

                if (ImGui::MenuItem("Theme selector")) {

                    ColorPalette::Open();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Game"))
            {
                if (ImGui::MenuItem("Run debug mode", NULL, false))
                {
                    Preferences::GenerateDebug();

                    //Todo: guardar escena actual
                    editor->getScene()->saveScene(editor->getScene()->getSceneName());
                    Game::Play(true);
                };

                if (ImGui::MenuItem("Run release mode", NULL, false))
                {
                    Preferences::GenerateRelease();

                    //Todo: guardar escena actual
                    editor->getScene()->saveScene(editor->getScene()->getSceneName());
                    Game::Play(false);
                };

                ImGui::Separator();

                if (ImGui::MenuItem("Stop", NULL, false)) {

                    Game::Stop();
                }

                ImGui::EndMenu();
            }


            //Close all floating windows

            //Layouts

            //

            if (ImGui::BeginMenu("Window"))
            {

                if (ImGui::MenuItem("Close all floating windows", NULL, false)) {


                }



                if (ImGui::BeginMenu("Layout")) {

                    auto windowLayout = Editor::getInstance()->GetWindowLayout();
                    auto allLayouts = windowLayout->GetAllLayouts();


                    int idx = 0;
                    for (auto& layout : allLayouts) {


                        if (ImGui::MenuItem(layout.c_str(), NULL, false)) {

                            windowLayout->SetLayout((ShyEditor::WindowLayout::Layout)idx);
                        }

                        idx++;
                    }


                    ImGui::EndMenu();
                }



                ImGui::Separator();

                auto& windows = Editor::getInstance()->GetAllWindows();

                for (auto window : windows) {


                    if (ImGui::MenuItem(window->GetWindowName().c_str(), NULL, window->IsVisible())) {

                        window->Show();
                    }
                }

                ImGui::EndMenu();
            }


            ImGui::Separator();

            if (ImGui::BeginMenu("Help"))
            {
                ImGui::MenuItem("Documentation", NULL, false); // Link to the website
                ImGui::MenuItem("About us", NULL, false);
                ImGui::EndMenu();
            }



            if (gameObject != nullptr)
            {
                ImGui::Separator();
                if (ImGui::BeginMenu("GameObject"))
                {
                    if (ImGui::MenuItem("Create prefab", NULL, false)) {
                        shouldOpenSavePrefabPopup = true;
                    }

                    if (ImGui::MenuItem("Add script", NULL, false)) {
                        editor->changeEditorState(Editor::EDITOR_STATE::SCRIPTING_WINDOW);
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
        showNewScenePopup();
        showSavePrefabPopup(gameObject);
    }

    void MenuBar::showRenamePopup(GameObject* gameObject)
    {
        if (gameObject == nullptr) return;

        if (shouldOpenRenamePopup)
        {
            ImGui::OpenPopup("Rename Object##" + gameObject->getId());
            shouldOpenRenamePopup = false;
        }

        if (ImGui::BeginPopup("Rename Object##" + gameObject->getId()))
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

    void MenuBar::showNewScenePopup()
    {
        if (shouldOpenNewScenePopup)
        {
            ImGui::OpenPopup("New scene");
            shouldOpenNewScenePopup = false;
        }

        if (ImGui::BeginPopup("New scene"))
        {
            ImGui::Text(("Insert name for the scene:"));

            ImGui::Separator();

            static char nameBuffer[256];  // Buffer to hold the new name

            // Display an input text field for renaming
            if (ImGui::InputText("Scene name", nameBuffer, sizeof(nameBuffer))) {

            }

            if (ImGui::Button("Ok"))
            {
                if (strlen(nameBuffer) > 0) {
                    //Save current scene
                    editor->getScene()->saveScene(editor->getScene()->getSceneName());

                    //Create new scene
                    editor->getScene()->loadScene(nameBuffer);

                    //Save empty new scene
                    editor->getScene()->saveScene(editor->getScene()->getSceneName());
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void MenuBar::showSavePrefabPopup(GameObject* go)
    {
        if (go == nullptr) return;

        if (shouldOpenSavePrefabPopup)
        {
            ImGui::OpenPopup("Save prefab" + go->getId());
            shouldOpenSavePrefabPopup = false;
        }

        if (ImGui::BeginPopup("Save prefab" + go->getId()))
        {
            ImGui::Text(("Insert name for the prefab:"));

            ImGui::Separator();

            static char nameBuffer[256];

            if (ImGui::InputText("Prefab name", nameBuffer, sizeof(nameBuffer)))
            {
            }

            if (ImGui::Button("Ok"))
            {
                if (strlen(nameBuffer) > 0) {
                    GameObject* prefab = new GameObject(*go);
                    prefab->setName(nameBuffer);
                    
                    PrefabManager::AddPrefab(prefab);
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

}
