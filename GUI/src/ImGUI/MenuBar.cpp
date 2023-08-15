#include "MenuBar.h"
#include "imgui.h"
#include "Window.h"
#include "ImGUIManager.h"
#include "Scene.h"
#include "GameObject.h"
#include <fstream>
#include <Windows.h>
#include <tchar.h>
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
                ImGUIManager::getInstance()->getScene()->saveScene();
            }

            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit", NULL, false))
            {
                imGuiManager->exit();
            };

            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Play", NULL, false))
            {
                ImGUIManager::getInstance()->getScene()->saveScene();
                
                //system("Main_Debug.exe");



                //STARTUPINFO si;
                //PROCESS_INFORMATION pi;

                //ZeroMemory(&si, sizeof(si));
                //si.cb = sizeof(si);
                //ZeroMemory(&pi, sizeof(pi));

                //// Modifica la ruta al programa que deseas ejecutar
                //const char* programPath = "Main_Debug.exe";
                //LPTSTR szCmdline = _tcsdup(TEXT("Main_Debug.exe"));

                //// Redirección de salida
                //si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
                //si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
                //si.dwFlags |= STARTF_USESTDHANDLES;

                //// Crea el proceso separado
                //if (CreateProcess(NULL,   // Nombre del módulo
                //    szCmdline, // Comando a ejecutar
                //    NULL,   // Atributos de seguridad del proceso
                //    NULL,   // Atributos de seguridad del proceso
                //    TRUE,   // Heredar manipuladores
                //    0,      // Flags de creación
                //    NULL,   // Variables de entorno
                //    NULL,   // Directorio de inicio
                //    &si,    // Información de inicio
                //    &pi)) { // Información del proceso

                //    // Espera a que el proceso hijo termine
                //    //WaitForSingleObject(pi.hProcess, INFINITE);

                //    // Cierra los manipuladores
                //    //CloseHandle(pi.hProcess);
                //    //CloseHandle(pi.hThread);

                //}
                //else {
                //    std::cerr << "Error al crear el proceso." << std::endl;
                //}


                FILE* pipe = _popen("Main_Debug.exe", "r");
                if (!pipe) {
                    std::cerr << "Error opening pipe." << std::endl;
                    return;
                }

                char buffer[128];
                std::string result = "";

                // Leer la salida de la tubería y almacenarla en 'result'
                while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                    result += buffer;
                }

                // Cerrar la tubería
                _pclose(pipe);

                // Mostrar la salida capturada
                std::cout << "Output of the executed program:\n" << result << std::endl;


            };

            ImGui::MenuItem("Pause", NULL, false);
            ImGui::MenuItem("Stop", NULL, false);

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