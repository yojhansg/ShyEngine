#include "ScriptCreation.h"
#include "imgui.h"
#include <fstream>
#include "ImGUIManager.h"

PEditor::ScriptCreation::ScriptCreation() : Window("", None)
{
    imGuiManager = ImGUIManager::getInstance();
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

        if (ImGui::BeginChild("ChildWindow", ImVec2(300, 200), true, ImGuiWindowFlags_None))
        {

            ImGui::Text("Child Window Content Here");
        }
        ImGui::EndChild();

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

        ImGui::EndPopup();
    }
}

void PEditor::ScriptCreation::setName(std::string name)
{
    strcpy_s(nameBuffer, name.c_str());
}
