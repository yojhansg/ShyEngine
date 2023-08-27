#include "MenuBar.h"

#include "nlohmann/json.hpp"
#include "ProjectsManager.h"
#include "ColorPalette.h"
#include "Preferences.h"
#include "Entity.h"
#include "Editor.h"
#include "Scene.h"
#include "imgui.h"
#include "Game.h"
#include "WindowLayout.h"
#include "PrefabManager.h"
#include "Build.h"

#include <Windows.h>
#include <fstream>
#include <tchar.h>
#include "CheckML.h"


namespace ShyEditor {

    MenuBar::MenuBar()
    {
        editor = Editor::GetInstance();

        shouldOpenRenamePopup = false;
        shouldOpenNewScenePopup = false;
    }

    void MenuBar::Update()
    {

        Entity* entity = editor->GetScene()->GetSelectedEntity();

        if (ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New scene", NULL, false)) {

                    shouldOpenNewScenePopup = true;
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Save Scene", NULL, false)) {

                    editor->GetScene()->SaveScene();
                }


                ImGui::Separator();

                if (ImGui::MenuItem("Build", NULL, false))
                {
                    Editor::GetInstance()->GetBuildManager()->GenerateBuild();
                };


                ImGui::Separator();

                if (ImGui::MenuItem("Exit", NULL, false))
                {
                    Editor::GetInstance()->End();
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
                    editor->GetScene()->SaveScene();
                    Game::Play(true);
                };

                if (ImGui::MenuItem("Run release mode", NULL, false))
                {
                    Preferences::GenerateRelease();

                    //Todo: guardar escena actual
                    editor->GetScene()->SaveScene();
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

                    auto windowLayout = Editor::GetInstance()->GetWindowLayout();
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

                auto& windows = Editor::GetInstance()->GetAllWindows();

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



            if (entity != nullptr)
            {
                ImGui::Separator();
                if (ImGui::BeginMenu("Entity"))
                {
                    if (ImGui::MenuItem("Create prefab", NULL, false)) {
                        Entity* prefab = new Entity(*entity);
                        PrefabManager::AddPrefab(prefab);
                    }

                    if (ImGui::MenuItem("Add script", NULL, false)) {
                        editor->ChangeEditorState(Editor::EDITOR_STATE::SCRIPTING_WINDOW);
                    }

                    ImGui::Separator();

                    if (ImGui::MenuItem("Rename", NULL, false))
                    {
                        shouldOpenRenamePopup = true;
                    }

                    ImGui::Separator();

                    if (ImGui::MenuItem("Delete", NULL, false)) {
                        entity->ToDelete();
                    }

                    ImGui::EndMenu();
                }
            }

            ImGui::EndMainMenuBar();
        }

        ShowRenamePopup(entity);
        ShowNewScenePopup();
    }

    void MenuBar::ShowRenamePopup(Entity* entity)
    {
        if (entity == nullptr) return;

        if (shouldOpenRenamePopup)
        {
            ImGui::OpenPopup("Rename Entity##" + entity->GetId());
            shouldOpenRenamePopup = false;
        }

        if (ImGui::BeginPopup("Rename Entity##" + entity->GetId()))
        {
            ImGui::Text(("Insert new name for Entity: " + entity->GetName()).c_str());

            ImGui::Separator();

            static char nameBuffer[256];  // Buffer to hold the new name

            // Display an input text field for renaming
            if (ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer)))
            {
            }

            if (ImGui::Button("Ok"))
            {
                if (strlen(nameBuffer) > 0) {
                    entity->SetName(nameBuffer);
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void MenuBar::ShowNewScenePopup()
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

                    // Save current scene
                    editor->GetScene()->SaveScene();

                    // Create new scene
                    editor->GetScene()->NewScene(nameBuffer);

                    // Store the scene as the last opened one
                    auto name = std::string(nameBuffer) + ".scene";
                    editor->SetLastOpenedScene(name);

                    bool load = editor->GetScene()->LoadScene();
                    editor->SetAnySceneOpened(load);

                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
