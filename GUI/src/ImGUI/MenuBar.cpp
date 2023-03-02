#include "MenuBar.h"
#include "imgui.h"

PEditor::MenuBar::MenuBar() : Window("", None)
{

}

void PEditor::MenuBar::render()
{
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
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
