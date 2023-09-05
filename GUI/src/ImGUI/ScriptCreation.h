#pragma once

#include "Window.h"
#include "ComponentInfo.h"

#include <unordered_map>
#include <string>
#include <vector>

class Editor;

namespace ShyEditor {

    namespace ScriptCreationUtilities {

        class ScriptNode;
        class ScriptEvent;
        class ScriptDropdownSelection;
        class ScriptMenuBar;
    }

    class ScriptCreation : public Window {

    private:

        static ScriptCreation* instance;

        int xpos, ypos;
        int scrollx, scrolly;

        bool scrolled;
        bool modified;

        bool lerping;
        float lerp_t;
        float lerpDuration;

        int initialx, initialy;
        int finalx, finaly;

        std::vector<ScriptCreationUtilities::ScriptNode*> nodes;
        std::unordered_map<std::string, ScriptCreationUtilities::ScriptEvent*> events;

        ScriptCreationUtilities::ScriptMenuBar* menuBar;

        Editor* editor;

        ScriptCreationUtilities::ScriptDropdownSelection* dropDownSelection;

        void RenderBox(const std::string& name, ImVec2 position, ImVec2 size);

        void ManageLerp();

        Components::AttributeValue SetScriptValuesFromJson(Components::AttributesType attrType, nlohmann::json& json);

    public:

        ScriptCreation();
        ~ScriptCreation();

        ScriptCreationUtilities::ScriptEvent* ContainsEvent(const std::string& event);
        void AddEvent(const std::string& name, ScriptCreationUtilities::ScriptEvent* event);
        void AddNode(ScriptCreationUtilities::ScriptNode* node);

        static void RemoveEvent(const std::string& name);

        void SetNodeCount(int count);
        void SetNode(int n, ScriptCreationUtilities::ScriptNode* node);

        void Save();
        void Load();

        static void GetScrollPosition(int* x, int* y);
        static bool ScrolledThisFrame();

        static bool IsFileModified();
        static void SetFileModified();
        static void ResetModified();
        static void Lerp(int x, int y, float lerpDuration);

        std::vector<ScriptCreationUtilities::ScriptNode*>& GetNodes();
        
        void ClearScript();
        void Behaviour() override;

        void SetName(const std::string& name);

    };
};
