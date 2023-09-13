#pragma once

#include <string>
#include <vector>

namespace ShyEditor {

   
    class WindowLayout {

    public:

        WindowLayout();

        enum class Layout
        {

            Default, Wide, SceneView, _2_1_1
        };


        void SetLayout(Layout layout);


        /*
            Esto debe ser llamado justo al principio del frame
        */
        bool Update();


        /*
            Devuelve una lista con los nombres de todos los layouts
        */
        std::vector<std::string> GetAllLayouts();

    private:

        bool initialization;
        bool updateLayout;
        Layout current;
    };
}