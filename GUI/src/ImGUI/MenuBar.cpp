#include "MenuBar.h"
#include "imgui.h"

PEditor::MenuBar::MenuBar() : Window("", None)
{

}

void PEditor::MenuBar::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        bool test = false;

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Change Background color", NULL, &test);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
