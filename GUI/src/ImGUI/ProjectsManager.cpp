#include "imgui_impl_sdlrenderer.h"
#include "ProjectsManager.h"
#include "imgui_impl_sdl.h"
#include "ImGUIManager.h"
#include "Window.h"
#include "imgui.h"
#include "SDL.h"

#include <nlohmann/json.hpp>
#include <shlwapi.h>
#include <Windows.h>
#include <Shlobj.h>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace nlohmann;

namespace PEditor {

    char ProjectsManager::project_name[128] = "";
    char ProjectsManager::create_project_path[256] = "";
    char ProjectsManager::open_project_path[256] = "";
    const std::wstring ProjectsManager::projectfileExtension = L".shyproject";
    const std::wstring ProjectsManager::projectsfileFolder = L"\\ShyEngine\\RecentProjects";
    const std::wstring ProjectsManager::projectsfileName = L"\\recentprojects.json";

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
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 3, ImGui::GetIO().DisplaySize.y * 4.0f / 5.0f));

            // New Project Window
            result = NewProject();

            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 3, 0));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 1.5f, ImGui::GetIO().DisplaySize.y * 4.0f / 5.0f));

            // Open Project Window
            if (!windowClosed)
                result = OpenProject();

            ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().DisplaySize.y / 5.0f));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y / 5.0f));

            // Close Editor
            if (!windowClosed)
                result = CloseEditor();

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

        ImGui::SetCursorPosY(h / 12.0f);

        ImGui::Text("Project Name:");
        ImGui::InputText("##project_name", project_name, IM_ARRAYSIZE(project_name));
        if (invalidProjectName)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project name. Can not be empty!");

        ImGui::SetCursorPosY(h / 4.0f);

        ImGui::Text("Project Path:");
        ImGui::InputText("##project_path", create_project_path, IM_ARRAYSIZE(create_project_path));

        // Select folder button logic
        if (ImGui::Button("Select Folder", ImVec2(w / 2.0f, h / 20.0f))) {
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

        // Shows an error message
        if (invalidNewProjectPath)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project path. Can not be empty and \nmust be an existing folder!");

        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 2.0f));
        // Create project button logic
        if (ImGui::Button("Create Project", ImVec2(w / 2.0f, h / 15.0f)))
        {
            // Input validation
            bool valid = ValidateInputOnNewProject();

            if (valid) {

                SaveProject();

                windowClosed = true;

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

        ImGui::SetCursorPosY(h / 12.0f);

        ImGui::Text("Project Path:");
        ImGui::InputText("##project_path", open_project_path, IM_ARRAYSIZE(open_project_path));

        if (ImGui::Button("Select File", ImVec2(w / 2.0f, h / 20.0f))) {
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

        // Shows an error message
        if (invalidOpenProjectPath)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project path. Cannot be empty and must be an existing .shyproject file!");

        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 3.5f));
        if (ImGui::Button("Open Project", ImVec2(w / 2.0f, h / 15.0f)))
        {
            // Input validation
            bool valid = ValidateInputOnOpenProject();

            if (valid) {

                // Logica de abrir un proyecto

                windowClosed = true;

                ImGui::End();

                return Result::OPENED;
            }

        }

        ImGui::SetCursorPosY(h / 2.5f);
        ImGui::Text("Recent Projects:");

        bool recentProjectOpened = false;
        std::string path;
        if (ImGui::BeginListBox("##", ImVec2(w - w /10, h / 2))) {

            for (int i = 0; i < recentProjectsInfo.size() && !recentProjectOpened; i++) {

                std::string t = "Project: " + recentProjectsInfo[i].name + "\t\tDate: " + recentProjectsInfo[i].date;
                if (ImGui::Button(t.c_str(), ImVec2(w, h / 8))) {
                    recentProjectOpened = true;
                    path = recentProjectsInfo[i].path;
                }
            }
        }

        ImGui::EndListBox();

        if (recentProjectOpened) {
            // Logica de abrir un proyecto

            windowClosed = true;

            ImGui::End();

            return Result::OPENED;
        }


        ImGui::End();

        return Result::CLOSED;
    }

    ProjectsManager::Result ProjectsManager::CloseEditor() {

        ImGui::Begin("CLOSE", (bool*)0, NoMove | NoResize | NoCollapse);

        ImVec2 size = ImGui::GetWindowSize();
        w = size.x; h = size.y;

        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 2.0f));
        if (ImGui::Button("Close", ImVec2(w / 2.0f, h / 5.0f)))
        {
            windowClosed = true;

            ImGui::End();

            return Result::CLOSED;

        }

        ImGui::End();

        return Result::CLOSED;
    }

    void ProjectsManager::MakeFolderToStoreRecentProjects() {

        PWSTR appDataPath;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &appDataPath))) {
            std::wstring appDataDir(appDataPath);
            CoTaskMemFree(appDataPath);

            std::wstring projectsDir = appDataDir + projectsfileFolder;

            recentProjectsFile = projectsDir + projectsfileName;

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
                    std::ofstream jsonFile(recentProjectsFile);

                    nlohmann::json j;
                    j["RecentProjectsPaths"] = json::array();

                    if (jsonFile.is_open()) {
                        jsonFile << j.dump(2); // Write an empty JSON object
                        jsonFile.close();
                        std::wcout << L"Empty JSON file created: " << recentProjectsFile << std::endl;
                    }

                }
            }
            // If folder already exists, reads the content of the recent projects file
            else {

                // Read recent projects file
                std::ifstream fileStreamIn(recentProjectsFile);

                // Gets the json from the recent projects file
                json jsonData = json::parse(fileStreamIn);
                fileStreamIn.close();

                if (!jsonData.contains("RecentProjectsPaths")) return;

                if (!jsonData["RecentProjectsPaths"].is_array()) return;

                int i = 0;
                std::vector<int> deletedProjects;
                for (const auto& project : jsonData["RecentProjectsPaths"]) {

                    std::string projectPath(project);

                    std::ifstream fileStreamIn;
                    fileStreamIn.open(projectPath);

                    if (fileStreamIn.is_open()) {

                        json json = json::parse(fileStreamIn);
                        fileStreamIn.close();

                        recentProjectsInfo.push_back(ProjectInfo(json["Project Name"], json["Current Date"], json["Project Path"]));

                    }
                    else deletedProjects.push_back(i);

                    i++;

                }

                for (i = 0; i < deletedProjects.size(); i++)
                    jsonData["RecentProjectsPaths"].erase(i);

                std::ofstream fileStreamOut(recentProjectsFile, std::ofstream::out | std::ofstream::trunc);
                if (fileStreamOut.is_open()) {
                    fileStreamOut << jsonData.dump(2);
                    fileStreamOut.close();
                }
                else {
                    //ERROR HANDLING
                }
            }
        }

    }

    void ProjectsManager::SaveProject() {

        nlohmann::json j;

        j["Project Name"] = project_name;
        j["Project Path"] = create_project_path;
        j["Current Date"] = getCurrentDate();

        std::string folder = std::string(create_project_path) + "\\" + std::string(project_name);

        // The folder is created
        std::error_code ec;
        std::filesystem::create_directories(folder, ec);

        if (!ec) {
            std::string path = folder + "\\" + project_name + ".shyproject";

            // Create the project file in the specified folder
            std::ofstream outputFile(path);
            if (outputFile.is_open()) {
                outputFile << j.dump(4);
                outputFile.close();
            }
            else {
                //ERROR HANDLING
            }

            StoreProjectPath(path);
        }

    }

    void ProjectsManager::StoreProjectPath(const std::string& path) {

        // Add the project path to the recent projects
        std::ifstream fileStreamIn;
        fileStreamIn.open(recentProjectsFile);

        // Gets the json from the recent projects file
        json json = json::parse(fileStreamIn);
        fileStreamIn.close();

        // Adds the path of the new project to the json
        json["RecentProjectsPaths"].push_back(path);

        // Write the json in the recent projects file
        std::ofstream fileStreamOut(recentProjectsFile, std::ofstream::out | std::ofstream::trunc);
        if (fileStreamOut.is_open()) {
            fileStreamOut << json.dump(2);
            fileStreamOut.close();
        }
        else {
            //ERROR HANDLING
        }

    }

    bool ProjectsManager::ValidateInputOnNewProject() {

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

    bool ProjectsManager::ValidateInputOnOpenProject() {

        DWORD fileAttributes = GetFileAttributesA(open_project_path);
        if (strlen(open_project_path) <= 0 || fileAttributes == INVALID_FILE_ATTRIBUTES) {

            invalidOpenProjectPath = true;
            return false;
        }
        else {

            std::wstring filePathWide(open_project_path, open_project_path + strlen(open_project_path));

            if (!HasExtension(filePathWide, projectfileExtension)) {
                invalidOpenProjectPath = true;
                return false;
            }
            else invalidOpenProjectPath = false;

        }

        return true;
    }

    bool ProjectsManager::HasExtension(const std::wstring& filePath, const std::wstring& extension) {

        return filePath.size() >= extension.size() &&
            filePath.compare(filePath.size() - extension.size(), extension.size(), extension) == 0;
    }

    bool ProjectsManager::AskForPermission() {

        int result = MessageBox(
            NULL,
            L"The engine needs to create a folder in your AppData path. Do you want to continue?",
            L"Permission confirmation",
            MB_YESNO | MB_ICONQUESTION
        );

        return (result == IDYES);
    }

    std::string ProjectsManager::getCurrentDate() {

        std::time_t currentTime = std::time(nullptr);

        std::tm localTime = {};

        localtime_s(&localTime, &currentTime);

        char formattedDate[50];
        strftime(formattedDate, sizeof(formattedDate), "%Y-%m-%d %H:%M:%S", &localTime);

        std::string formattedDateString(formattedDate);

        return formattedDateString;

    }

}
