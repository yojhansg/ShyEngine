#pragma once
#include "Window.h"
#include <string>

#include <vector>
#include "ComponentInfo.h"
#include "ScriptCreationUtilities.h"

class ImGUIManager;

namespace PEditor {


    class ScriptCreation : public Window
    {
    private:

        static ScriptCreation* instance;

        int xpos, ypos;
        int scrollx, scrolly;

        bool scrolled;
        bool modified;

        std::vector<ScriptCreationUtilities::ScriptNode*> nodes;

        ScriptCreationUtilities::ScriptMenuBar* menuBar;

        ImGUIManager* imGuiManager;

        ScriptCreationUtilities::ScriptDropdownSelection* dropDownSelection;

        void RenderBox(const std::string& name, ImVec2 position, ImVec2 size);


    public:
        ScriptCreation();
        ~ScriptCreation();

        void AddNode(ScriptCreationUtilities::ScriptNode* node);

        void SetNodeCount(int count);
        void SetNode(int n, ScriptCreationUtilities::ScriptNode* node);

        void Load();

        static void GetScrollPosition(int* x, int* y);
        static bool ScrolledThisFrame();

        static bool IsFileModified();
        static void SetFileModified();
        static void ResetModified();

        std::vector<ScriptCreationUtilities::ScriptNode*>& GetNodes();
        
        void ClearScript();
        virtual void render();

        void SetName(const std::string& name);
    };
};
