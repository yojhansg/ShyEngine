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

    std::wstring ProjectsManager::projectfileExtension = L".shyproject";
    std::wstring ProjectsManager::projectsfileFolder = L"\\ShyEngine\\RecentProjects";
    std::wstring ProjectsManager::projectsfileName = L"\\recentprojects.json";

    ProjectsManager::ProjectsManager() {

        imGuiManager = ImGUIManager::getInstance();

        windowClosed = invalidProjectName = false;
        invalidNewProjectPath = invalidOpenProjectPath = false;
        showPopUpWindowNewProject = showPopUpWindowOpenProject = false;

        creationDate = name = createPath = openPath = "";

        w = h = 0.0f;
    }

    void ProjectsManager::StartImGuiFrame() {
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void ProjectsManager::ImGuiRender(SDL_Renderer* renderer) {
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    ProjectsManager::Result ProjectsManager::ManageProjectSelection(SDL_Renderer* renderer) {

        Result result = Result::CLOSED;

        while (!windowClosed) {

            HandleInput();

            StartImGuiFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 3.0f, ImGui::GetIO().DisplaySize.y * 4.0f / 5.0f));

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

            ImGuiRender(renderer);

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

        // Shows an error message
        if (invalidProjectName)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project name. Can not be empty!");
        else
            name = project_name;


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
        else
            createPath = std::string(create_project_path) + "\\" + std::string(project_name) + "\\" + std::string(project_name) + ".shyproject";


        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 2.0f));

        // Create project button logic
        if (ImGui::Button("Create Project", ImVec2(w / 2.0f, h / 15.0f)))
        {
            // Input validation
            bool valid = ValidateInputOnNewProject();

            if (valid) {

                if (!SaveProject())
                    showPopUpWindowNewProject = true;
                else {
                    windowClosed = true;
                    ImGui::End();
                    return Result::ENTERED;
                }
            }

        }

        if (showPopUpWindowNewProject)
            ShowErrorPopup(errorMessage);

        ImGui::End();
        return Result::CLOSED;
    }

    ProjectsManager::Result ProjectsManager::OpenProject() {

        ImGui::Begin("Open Project", (bool*)0, NoMove | NoResize | NoCollapse);

        ImVec2 size = ImGui::GetWindowSize();
        w = size.x; h = size.y;

        ImGui::SetCursorPosY(h / 12.0f);

        ImGui::Text("Project Path:");
        ImGui::InputText("##project_path", open_project_path, IM_ARRAYSIZE(open_project_path));

        // Select file logic button
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

          if (GetOpenFileNameA(&ofn))
              strncpy_s(open_project_path, sizeof(open_project_path), fileName, _TRUNCATE);

        }

        // Shows an error message
        if (invalidOpenProjectPath)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid project path. Cannot be empty and must be an existing .shyproject file!");
        else
            openPath = open_project_path;


        // Open project logic button
        ImGui::SetCursorPos(ImVec2(w / 2.0f - w / 4.0f, h / 3.5f));
        if (ImGui::Button("Open Project", ImVec2(w / 2.0f, h / 15.0f)))
        {
            // Input validation
            bool valid = ValidateInputOnOpenProject();

            if (valid) {

                if (!ReadInfoFromProjectFile(openPath) || !StoreProjectPath(openPath))
                    showPopUpWindowOpenProject = true;
                else {

                    imGuiManager->setProjectInfo(new ProjectInfo(name, creationDate, openPath));

                    windowClosed = true;
                    ImGui::End();
                    return Result::ENTERED;
                }
            }

        }

        ImGui::SetCursorPosY(h / 2.5f);
        ImGui::Text("Recent Projects:");

        // Listbox for the recent projects
        bool recentProjectOpened = false;
        std::string recentProjectOpenedPath;
        if (ImGui::BeginListBox("##", ImVec2(w - w /10, h / 2))) {

            for (int i = 0; i < recentProjectsInfo.size() && !recentProjectOpened; i++) {

                std::string t = "Project: " + recentProjectsInfo[i].name + "\t\tDate: " + recentProjectsInfo[i].date;
                if (ImGui::Button(t.c_str(), ImVec2(w, h / 8))) {
                    recentProjectOpened = true;
                    recentProjectOpenedPath = recentProjectsInfo[i].path;
                }
            }
        }

        ImGui::EndListBox();


        // On opened one of the recent projects
        if (recentProjectOpened) {

            if (!ReadInfoFromProjectFile(recentProjectOpenedPath))
                showPopUpWindowOpenProject = true;
            else {
                imGuiManager->setProjectInfo(new ProjectInfo(name, creationDate, openPath));

                windowClosed = true;
                ImGui::End();
                return Result::ENTERED;
            }
        }

        if (showPopUpWindowOpenProject) {
            ShowErrorPopup(errorMessage);
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
            windowClosed = true;

        ImGui::End();
        return Result::CLOSED;
    }

    bool ProjectsManager::MakeFolderToStoreRecentProjects() {

        PWSTR appDataPath;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &appDataPath))) {
            std::wstring appDataDir(appDataPath);
            CoTaskMemFree(appDataPath);

            std::wstring projectsDir = appDataDir + projectsfileFolder;

            recentProjectsFile = projectsDir + projectsfileName;

            // Check if folder already exists
            std::error_code ec;
            if (!std::filesystem::exists(projectsDir, ec)) {

                // Error handling for checking if the directory exists or not.
                if (ec) {
                    std::cout << "PROJECTS MANAGER: Error when checking if the directory to be created already exists in your AppData path." << std::endl;
                    return false;
                }

                // Ask for user permission. If denied the folder wont be created.
                if (!AskForPermission()) {
                    std::cout << "PROJECTS MANAGER: Without user permission, the necessary files wont be created, and therefore, the editor wont be opened.." << std::endl;
                    return false;
                }

                // The folder is created
                std::filesystem::create_directories(projectsDir, ec);

                // Error handling on folders creation
                if (ec) {
                    std::cout << "PROJECTS MANAGER: Error while creating the directory to host the necessary files for the proper functioning of the editor." << std::endl;
                    return false;
                }

                // The JSON is created with an empty array of paths to be dumped into the recent projects file.
                nlohmann::json j;
                j["Recent Projects Paths"] = json::array();

                std::ofstream jsonFile(recentProjectsFile);

                if (!jsonFile.is_open()) {
                    std::cout << "PROJECTS MANAGER: The recent projects file could not be opened to dump the content." << std::endl;
                    return false;
                }

                jsonFile << j.dump(2);
                jsonFile.close();

                // Both the directory and the file are currently created correctly.

            }
            // If folder already exists, reads the content of the recent projects file
            // to check for any missing ones.
            else {

                // Read recent projects file
                std::ifstream fileStreamIn(recentProjectsFile);

                if (!fileStreamIn.is_open()) {
                    std::cout << "PROJECTS MANAGER: The recent projects file could not be opened to read its content." << std::endl;
                    return false;
                }

                // Gets the json from the recent projects file
                json jsonData = json::parse(fileStreamIn);

                fileStreamIn.close();

                // Check if the 'recentProjectsFile' contains the expected format
                if (!jsonData.contains("Recent Projects Paths") || !jsonData["Recent Projects Paths"].is_array()) {
                    std::cout << "PROJECTS MANAGER: The recent projects file does not contain the expected format." << std::endl;
                    return false;
                }

                // Save the information of name, path, and date for all recent projects.
                int i = 0;
                std::vector<int> deletedProjects;
                for (const auto& project : jsonData["Recent Projects Paths"]) {

                    std::string projectPath(project);

                    std::ifstream fileStreamIn;
                    fileStreamIn.open(projectPath);

                    if (fileStreamIn.is_open()) {

                        json json = json::parse(fileStreamIn);

                        // Adds the project data to the recent projects info array
                        recentProjectsInfo.push_back(ProjectInfo(json["Project Name"], json["Creation Date"], json["Project Path"]));

                        fileStreamIn.close();

                    }
                    else deletedProjects.push_back(i); // Mark the proyect as deleted

                    i++;

                }

                // Remove the path from the recent projects file for those projects that couldn't be opened (marked as deleted).
                for (i = 0; i < deletedProjects.size(); i++) {
                    std::string a(jsonData["Recent Projects Paths"][deletedProjects[i]]);
                    jsonData["Recent Projects Paths"].erase(deletedProjects[i]);
                }

                // After updating de JSON, dumps its content to the 'recentProjectsFile'
                std::ofstream fileStreamOut(recentProjectsFile, std::ofstream::out | std::ofstream::trunc);

                if (!fileStreamOut.is_open()) {
                    std::cout << "PROJECTS MANAGER: The recent projects file cannot be opened for writing." << std::endl;
                    return false;
                }

                fileStreamOut << jsonData.dump(2);
                fileStreamOut.close();

                // The 'recentProjectsFile' is now updated correctly.
            }

        }
        else {
            std::cout << "PROJECTS MANAGER: Could not create the necessary directories in your AppData path! Restart the editor and try again." << std::endl;
            return false;
        }

        return true;

    }

    bool ProjectsManager::SaveProject() {

        std::string folder = std::string(create_project_path);
        std::string path = createPath;

        // If the create path is an empty folder with the same name as the project
        // the folder project wont be created, it uses the create path.
        if (GetLastPathComponent(folder, '\\') == name) {

            if (!IsDirectoryEmpty(folder)) {
                errorMessage = L"There is already a project with the same name in the selected directory.";
                return false;
            }
            else path = std::string(create_project_path) + "\\" + std::string(project_name) + ".shyproject";
        }
        else {

            if (HasNestedDirectoryWithName(create_project_path, name)) {
                errorMessage = L"There is already a project with the same name in the selected directory.";
                return false;
            }

            // The folder which contains the .shyproject file is created
            folder = std::string(create_project_path) + "\\" + std::string(project_name);

            std::error_code ec;
            std::filesystem::create_directories(folder, ec);

            if (ec) {
                errorMessage = L"Error while creating the project directory.";
                return false;
            }
        }

        if (!StoreProjectPath(path))
            return false;

        // Store the values in the .shyproject file
        nlohmann::json j;

        creationDate = getCurrentDate();

        j["Project Name"] = name;
        j["Project Path"] = path;
        j["Creation Date"] = creationDate;

        // Create the project file in the specified folder
        std::ofstream outputFile(path);

        if (!outputFile.is_open()) {
            errorMessage = L"Error while creating the project file in the selected directory.";
            return false;
        }

        outputFile << j.dump(4);
        outputFile.close();

        imGuiManager->setProjectInfo(new ProjectInfo(name, creationDate, createPath));

        return true;

    }

    bool ProjectsManager::StoreProjectPath(const std::string& path) {

        std::string folder = std::string(create_project_path) + "\\" + std::string(project_name);

        // Add the project path to the recent projects
        std::ifstream fileStreamIn(recentProjectsFile);

        if (!fileStreamIn.is_open()) {
            errorMessage = L"Error while opening the recent projects file for reading.";
            return false;
        }

        // Gets the json from the recent projects file
        json json = json::parse(fileStreamIn);

        fileStreamIn.close();

        // Check if the array already contains the path
        bool containsPath = false;
        for (const auto& p : json["Recent Projects Paths"]) {
            if (p == path) {
                containsPath = true;
                break;
            }
        }

        // Adds the path of the new project to the json
        if (!containsPath) {
            json["Recent Projects Paths"].push_back(path);

            // Write the json in the recent projects file
            std::ofstream fileStreamOut(recentProjectsFile, std::ofstream::out | std::ofstream::trunc);

            if (!fileStreamOut.is_open()) {
                errorMessage = L"Error while opening the recent projects file for writing.";
                return false;
            }

            fileStreamOut << json.dump(2);
            fileStreamOut.close();

        }

        return true;
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

    std::string ProjectsManager::GetLastPathComponent(const std::string& path, char delimiter) {

        std::istringstream iss(path);
        std::string token;

        std::vector<std::string> tokens;
        while (std::getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }

        if (!tokens.empty()) {
            return tokens.back();
        }

        return ""; // Devuelve una cadena vacía si no hay elementos

    }

    bool ProjectsManager::IsDirectoryEmpty(const std::string& directory) {

        WIN32_FIND_DATAA findData;
        HANDLE findHandle = FindFirstFileA((directory + "\\*").c_str(), &findData);

        if (findHandle == INVALID_HANDLE_VALUE) {
            // Error handling if directory doesn't exist or there was a problem opening it
            return false;
        }

        bool isEmpty = true;
        do {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                isEmpty = false; // At least one file or directory exists
                break;
            }
        } while (FindNextFileA(findHandle, &findData) != 0);

        FindClose(findHandle);
        return isEmpty;
    }

    bool ProjectsManager::ReadInfoFromProjectFile(const std::string& filepath) {

        std::ifstream fileStreamIn(filepath);

        if (!fileStreamIn.is_open()) {
            errorMessage = L"Error opening the project file for reading.";
            return false;
        }

        json json = json::parse(fileStreamIn);
        fileStreamIn.close();

        name = json["Project Name"];
        openPath = json["Project Path"];
        creationDate = json["Creation Date"];

        return true;

    }

    bool ProjectsManager::HasNestedDirectoryWithName(const std::string& parentDir, const std::string& dirName) {

        std::filesystem::path parentPath(parentDir);
        for (const auto& entry : std::filesystem::directory_iterator(parentPath)) {
            if (entry.is_directory() && entry.path().filename() == dirName) {
                return true;
            }
        }
        return false;

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

    void ProjectsManager::ShowErrorPopup(const std::wstring& errorMessage) {

        ImGui::OpenPopup("Error");

        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 1.2f, ImGui::GetIO().DisplaySize.y / 6.0f));

        if (ImGui::BeginPopupModal("Error")) {
            ImGui::Text("%ls", errorMessage.c_str());

            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2.0f - ImGui::GetContentRegionAvail().x / 16.0f);
            ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 0.8f);
            if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x / 4.0f, ImGui::GetContentRegionAvail().y / 1.5f))) {
                ImGui::CloseCurrentPopup();
                showPopUpWindowNewProject = showPopUpWindowOpenProject = false;
            }
            ImGui::EndPopup();
        }

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
