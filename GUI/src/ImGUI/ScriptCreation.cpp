#include "ScriptCreation.h"
#include "imgui.h"
#include <fstream>
#include "ImGUIManager.h"
#include <iostream>

PEditor::ScriptCreation::ScriptCreation() : Window("", None)
{
    imGuiManager = ImGUIManager::getInstance();



    selection = (new ScriptCreationUtilities::ScriptDropdownSelection())->SetID(0)->SetPosition(600, 100);

}

void PEditor::ScriptCreation::render()
{
    ImGui::OpenPopup("Create script");
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::BeginPopup("Create script", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        if (ImGui::InputText("Name the script", nameBuffer, sizeof(nameBuffer)))
        {
        }

        ImVec2 a = ImVec2(100, 100);
        ImVec2 b = ImVec2(200, 100);;
        ImVec2 c = ImGui::GetMousePos();
        ImVec2 d = ImGui::GetMousePos();

        c.x -= 100;

        auto drawList = ImGui::GetWindowDrawList();
        drawList->AddBezierCubic(a, b, c, d, IM_COL32(255, 255, 255, 255), 1, 30);

        RenderBox("Primera", ImVec2(200, 300), ImVec2(200, 300));
        RenderBox("Segunda", ImVec2(460, 300), ImVec2(200, 300));
        RenderBox("Tercera", ImVec2(100, 700), ImVec2(200, 300));
        RenderBox("Cuarta", ImVec2(120, 30), ImVec2(200, 300));

        ImGui::SetCursorPos(ImVec2(0, 700));

        if (ImGui::Button("Save script"))
        {
            std::string filename = std::string(nameBuffer) + ".script";
            std::string filePath = "scripts/" + filename;

            std::ofstream outputFile(filePath);
            if (outputFile.is_open())
            {
                outputFile.close();
            }

            ImGUIManager::getInstance()->creatingScript(false);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Close"))
        {
            ImGUIManager::getInstance()->creatingScript(false);
            ImGui::CloseCurrentPopup();
        }

        selection->Render();
        ImGui::EndPopup();
    }


}


void PEditor::ScriptCreation::RenderBox(const std::string& name, ImVec2 position, ImVec2 size)
{

    ImGui::SetCursorPos(position);

    if (ImGui::BeginChild(name.c_str(), size, true, ImGuiWindowFlags_None))
    {

        ImGui::Text("Child Window Content Here");
    }
    ImGui::EndChild();

}



void PEditor::ScriptCreation::setName(std::string name)
{
    strcpy_s(nameBuffer, name.c_str());
}
