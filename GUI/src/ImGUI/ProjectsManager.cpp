#include "ProjectsManager.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "ImGUIManager.h"
#include "imgui.h"
#include "SDL.h"

#include <nlohmann/json.hpp>
#include <Windows.h>
#include <Shlobj.h>
#include <shlwapi.h>
#include "Window.h"

#include <iostream>
#include <fstream>

namespace PEditor {

    char ProjectsManager::project_name[128] = "";
    char ProjectsManager::create_project_path[256] = "";
    char ProjectsManager::open_project_path[256] = "";

    ProjectsManager::ProjectsManager() {
        windowClosed = false;
        invalidProjectName = false;
        invalidNewProjectPath = false;
        invalidOpenProjectPath = false;

        w = h = 0.0f;

        MakeFolderToStoreRecentProjects();
    }

    ProjectsManager::Result ProjectsManager::ManageProjectSelection(SDL_Renderer* renderer) {

        Result result = Result::CLOSED;

        while (!windowClosed) {

            HandleInput();

            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            // Start the Dear ImGui frame
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 3, ImGui::GetIO().DisplaySize.y));

            // New Project Window
            result = NewProject();

            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 3, 0));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 1.5f, ImGui::GetIO().DisplaySize.y));

            // Open Project Window
            if (!windowClosed)
                result = OpenProject();

            // Rendering
            ImGui::Render();
            SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
            SDL_RenderClear(renderer);
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer);

        }

        return result;
    }


    void ProjectsManager::HandleInput() {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                windowClosed = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                windowClosed = true;
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                windowClosed = true;
        }

    }

    ProjectsManager::Result ProjectsManager::NewProject() {

        ImGui::Begin("Create Project", (bool*)0, NoMove | NoResize | NoCollapse);

        ImVec2 size = ImGui::GetWindowSize();
        w = size.x; h = size.y;

        ImGui::SetCursorPosY(h / 15.0f);

        ImGui::Text("Project Name:");
        ImGui::InputText("##project_name", project_name, IM_ARRAYSIZE(project_name));
        if (invalidProjectName)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project name. Can not be empty!");

        ImGui::SetCursorPosY(h / 6.0f);

        ImGui::Text("Project Path:");
        ImGui::InputText("##project_path", create_project_path, IM_ARRAYSIZE(create_project_path));

        // Select folder button logic
        if (ImGui::Button("Select Folder", ImVec2(w / 2.0f, h / 30.0f))) {
            BROWSEINFOA bi;
            ZeroMemory(&bi, sizeof(bi));
            bi.hwndOwner = GetForegroundWindow();
            bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
            LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);

            if (pidl) {
                char selectedPath[MAX_PATH];
                if (SHGetPathFromIDListA(pidl, selectedPath)) {
                    strcpy_s(create_project_path, sizeof(create_project_path), selectedPath);
                }
                CoTaskMemFree(pidl);
            }
        }

        if (invalidNewProjectPath)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project path. Can not be empty and must \nbe an existing folder!");

        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 3.3f));

        // Create project button logic
        if (ImGui::Button("Create Project", ImVec2(w / 2.0f, h / 20.0f)))
        {
            // Input validation
            bool valid = ValidateInputNew();

            if (valid) {
                windowClosed = true;

                SaveProject();

                ImGui::End();

                return Result::CREATED;
            }

        }

        ImGui::End();

        return Result::CLOSED;
    }

    ProjectsManager::Result ProjectsManager::OpenProject() {

        // Recent Projects tambien

        ImGui::Begin("Open Project", (bool*)0, NoMove | NoResize | NoCollapse);

        ImVec2 size = ImGui::GetWindowSize();
        w = size.x; h = size.y;

        ImGui::SetCursorPosY(h / 15.0f);

        ImGui::Text("Project Path:");
        ImGui::InputText("##project_path", open_project_path, IM_ARRAYSIZE(open_project_path));

        if (ImGui::Button("Select File", ImVec2(w / 2.0f, h / 30.0f))) {
          OPENFILENAMEA ofn;  // Estructura de diálogo de archivo
          char fileName[MAX_PATH] = "";
          ZeroMemory(&ofn, sizeof(ofn));
          ofn.lStructSize = sizeof(ofn);
          ofn.hwndOwner = GetForegroundWindow();
          ofn.lpstrFilter = "Todos los archivos\0*.*\0";
          ofn.lpstrFile = fileName;
          ofn.nMaxFile = MAX_PATH;
          ofn.lpstrTitle = "Seleccionar Archivo";
          ofn.Flags = OFN_FILEMUSTEXIST;

          if (GetOpenFileNameA(&ofn)) {
              strncpy_s(open_project_path, sizeof(open_project_path), fileName, _TRUNCATE);
          }
        }

        if (invalidOpenProjectPath)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project path. Can not be empty and must be an existing .shy project file!");

        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 5.0f));

        if (ImGui::Button("Open Project", ImVec2(w / 2.0f, h / 20.0f)))
        {

            // Input validation
            bool valid = ValidateInputOpen();

            if (valid) {
                windowClosed = true;

                ImGui::End();

                return Result::OPENED;
            }

        }

        ImGui::End();

        return Result::CLOSED;
    }

    void ProjectsManager::MakeFolderToStoreRecentProjects() {

        PWSTR appDataPath;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &appDataPath))) {
            std::wstring appDataDir(appDataPath);
            CoTaskMemFree(appDataPath);

            std::wstring editorDir = appDataDir + L"\\ShyEngine";
            std::wstring projectsDir = editorDir + L"\\RecentProjects";

            // Check if folder already exists
            std::error_code ec;
            if (!std::filesystem::exists(projectsDir, ec)) {

                // Ask for user permission. If denied the folder wont be created.
                if (!AskForPermission()) return;

                // The folder is created
                std::filesystem::create_directories(projectsDir, ec);
                if (!ec) {
                    std::wcout << L"Recent projects folder created in: " << projectsDir << std::endl;

                    // Create an empty JSON file
                    std::wstring jsonFilePath = projectsDir + L"\\recentprojects.json";
                    std::ofstream jsonFile(jsonFilePath);
                    if (jsonFile.is_open()) {
                        jsonFile << "{}"; // Write an empty JSON object
                        jsonFile.close();
                        std::wcout << L"Empty JSON file created: " << jsonFilePath << std::endl;
                    }
                    else {
                        std::wcerr << L"Could not create JSON file." << std::endl;
                    }

                }
                else {
                    std::wcerr << L"Could not create recent projects folder." << std::endl;
                }
            }
        }

    }

    void ProjectsManager::SaveProject() {

        nlohmann::json j;

        j["Project Name"] = project_name;
        j["Project Path"] = create_project_path;

        std::string path = std::string(create_project_path) + "/" + std::string(project_name) + ".shyproject";

        std::ofstream outputFile(path);
        if (outputFile.is_open()) {
            outputFile << j.dump(4);
            outputFile.close();
        }
        else {
            //ERROR HANDLING
        }

    }

    bool ProjectsManager::ValidateInputNew() {

        bool name, path;
        name = path = true;

        if (strlen(project_name) <= 0) {
            invalidProjectName = true;
            name = false;
        }
        else invalidProjectName = false;

        DWORD fileAttributes = GetFileAttributesA(create_project_path);
        if (strlen(create_project_path) <= 0 || fileAttributes == INVALID_FILE_ATTRIBUTES) {
            invalidNewProjectPath = true;
            path = false;
        }
        else invalidNewProjectPath = false;

        return name && path;
    }

    bool ProjectsManager::ValidateInputOpen() {

        DWORD fileAttributes = GetFileAttributesA(open_project_path);
        if (strlen(open_project_path) <= 0 || fileAttributes == INVALID_FILE_ATTRIBUTES) {
            invalidOpenProjectPath = true;
            return false;
        }
        else invalidOpenProjectPath = false;

        return true;
    }

    bool ProjectsManager::AskForPermission() {

        int result = MessageBox(
            NULL,
            L"La aplicación desea crear una carpeta y un archivo en la carpeta AppData. ¿Desea continuar?",
            L"Confirmación de permiso",
            MB_YESNO | MB_ICONQUESTION
        );

        return (result == IDYES);
    }

}
