#include "ScriptCreation.h"
#include "imgui.h"
#include <fstream>
#include "ImGUIManager.h"
#include <iostream>


PEditor::ScriptCreation::ScriptCreation() : Window("", None)
{
    imGuiManager = ImGUIManager::getInstance();

    dropDownSelection = new ScriptCreationUtilities::ScriptDropdownSelection(this);


}

PEditor::ScriptCreation::~ScriptCreation()
{
    delete dropDownSelection;
}

void PEditor::ScriptCreation::AddNode(ScriptCreationUtilities::ScriptNode* node)
{
    node->SetID(nodes.size());
    nodes.push_back(node);
}

float offset = 0;

void PEditor::ScriptCreation::render()
{
    ImGui::OpenPopup("Create script");
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::BeginPopup("Create script", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        //ImGui::SetWindowFocus();

        if (ImGui::InputText("Name the script", nameBuffer, sizeof(nameBuffer)))
        {
        }

        ScriptCreationUtilities::Grid::SetOffset(offset += 0.2f, 0);
        ScriptCreationUtilities::Grid::SetColor(100, 100, 100, 255);
        ScriptCreationUtilities::Grid::Draw();

        ImVec2 mouse = ImGui::GetMousePos();
        ScriptCreationUtilities::Bezier::Draw(100, 100, mouse.x, mouse.y);

        for (auto node : nodes) {

            node->Render();
        }

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

            ClearScript();

            ImGUIManager::getInstance()->creatingScript(false);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Close"))
        {
            ClearScript();
            ImGUIManager::getInstance()->creatingScript(false);
            ImGui::CloseCurrentPopup();
        }

        dropDownSelection->Render();
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

void PEditor::ScriptCreation::ClearScript()
{
    nameBuffer[0] = '\0';

    for (auto node : nodes) {
        delete node;
    }
    nodes.clear();
}

void PEditor::ScriptCreation::setName(std::string name)
{
    strcpy_s(nameBuffer, name.c_str());
}
