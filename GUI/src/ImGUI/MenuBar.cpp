#include "MenuBar.h"
#include "imgui.h"
#include "Window.h"
#include "ImGUIManager.h"

PEditor::MenuBar::MenuBar() : Window("", None)
{
    originalFramePadding = ImGui::GetStyle().FramePadding.y;
    
    //Width is 0 cause menu bar always take the whole screen width
    width = oriWidth = 0;
    height = oriHeight = 4;  
}

void PEditor::MenuBar::render()
{
    // Change the menu bar height temporarily
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, height));  // Change the menu bar height to 10 pixels

    if (ImGui::BeginMainMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Scene", NULL, false);
            ImGui::MenuItem("Open Scene", NULL, false);
            ImGui::Separator();
            ImGui::MenuItem("Save Scene", NULL, false);
            ImGui::Separator();
            ImGui::MenuItem("Exit", NULL, false);

            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Play", NULL, false);
            ImGui::MenuItem("Pause", NULL, false);
            ImGui::MenuItem("Stop", NULL, false);
            ImGui::Separator();
            ImGui::MenuItem("Delete", NULL, false);

            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("About us", NULL, false);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Reset the menu bar height to the default value
    ImGui::PopStyleVar();  // Restore the default menu bar height
}

void PEditor::MenuBar::update()
{
    ImGUIManager* manager = ImGUIManager::getInstance();
    ImVec2 originalSize = manager->getOriginalWindowSize();
    ImVec2 currentSize = manager->getMainWindowSize();

    float scaleFactorY = currentSize.y / originalSize.y;

    height = oriHeight * scaleFactorY * scaleFactorY * 0.88f;
}
