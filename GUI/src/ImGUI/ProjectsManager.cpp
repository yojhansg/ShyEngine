#include "ProjectsManager.h"
#include "ImGUIManager.h"
#include "imgui.h"

PEditor::ProjectsManager::ProjectsManager() : Window("Projects Management Window", NoResize | NoCollapse | NoMove) {

	imGuiManager = ImGUIManager::getInstance();

	ImVec2 mainWindowSize = imGuiManager->getMainWindowSize();

	setSize(ImVec2(mainWindowSize.x, mainWindowSize.y));

}

void PEditor::ProjectsManager::update() {

}

void PEditor::ProjectsManager::handleInput(SDL_Event* event) {

}

void PEditor::ProjectsManager::render() {

    ImGui::Begin("Crear Proyecto");

    static char project_name[128] = ""; // Nombre del proyecto
    static char project_path[256] = ""; // Ruta del proyecto
    static bool create_assets_folder = true; // Crear carpeta de activos
    static bool create_source_folder = true; // Crear carpeta de código fuente

    ImGui::Text("Nombre del Proyecto:");
    ImGui::InputText("##project_name", project_name, IM_ARRAYSIZE(project_name));

    ImGui::Text("Ruta del Proyecto:");
    ImGui::InputText("##project_path", project_path, IM_ARRAYSIZE(project_path));

    if (ImGui::Button("Crear Proyecto"))
    {

        imGuiManager->changeEditorState(ImGUIManager::EDITOR_STATE::EDITOR_WINDOW);

        // Lógica para crear el proyecto con los parámetros proporcionados
        // Puedes usar las funciones de sistema de archivos de tu plataforma para crear carpetas y archivos
        // ¡No olvides validar los campos antes de crear el proyecto!

        // Ejemplo de lógica para crear carpetas si las opciones están habilitadas
        if (create_assets_folder)
        {
            // Crea la carpeta de activos en project_path
        }

        if (create_source_folder)
        {
            // Crea la carpeta de código fuente en project_path
        }

        // ¡Proyecto creado exitosamente!
    }

    ImGui::End();

}
