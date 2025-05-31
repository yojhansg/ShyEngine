# Final degree project | Desarrollo de videojuegos UCM

Final degree project repository

## Repository Directory Structure

The project is divided into two main sections: the engine and the editor,
which can be found in the "Motor" and "Editor" directories.

In the "Proyectos" directory, you'll find the game projects
developed by each member of the group. Each one contains a
“.shyproject” file that allows it to be opened later from the editor.

In the "Memoria" directory, you'll find the LaTeX template
used to create the project report.

In the "Documentacion" directory, you'll find the web template
used to write the documentation for the engine and the editor.

## Motor y Editor directories

These directories contain the engine and editor projects, respectively.
Each of them includes a "src" directory with the project's source code,
a "projects" directory with the Visual Studio projects that make up the solution,
a "dependencies" directory with the uncompiled libraries used,
and finally, a ".sln" file containing the solution.

## Description of the project

Shy is the name of the project we developed as a team of three for the final degree project (TFG). It is a 2D game engine and editor designed for non-programmers. The objective is to provide a tool for creating simple 2D games with a lower learning curve compared to the more commonly used engines today, focusing on individuals with limited experience in programming or game development in general. Additionally, the game can be executed directly from the editor, speeding up the development process, and executable versions can be generated automatically (Build).

The development of the project was divided into three main parts: engine, editor and scripting system:
The engine and editor of Shy are not integrated, so the solution to communicate them is a data-driven engine. This means that everything done in the editor is generated and stored in files, and the engine reads them to dynamically create scenes, entities, prefabs, etc., during execution. Additionally, it's worth mentioning that the entire project was developed in C++ using Visual Studio and a set of necessary libraries for both the engine and the editor.

## Engine

Regarding the engine, it is divided into several modules that contain the necessary technology for different purposes. The most important modules include a resource manager, audio, input, physics, rendering, gameplay architecture (based on entities and components), and native components.

The engine features a reflection system through tags (similar to Unreal) so that the editor's scripting system knows its API and can call functions from each of its modules.

The libraries used by the engine are SDL for rendering (images, text) and input, and Box2D for physics.

![Box2DSDL](/ReadmeAssets/Box2DSDL.png)

## Editor

As for the editor, it uses the ImGUI library for the user interface, with which we implemented a window system to organize its functionality. It is divided into three main views, project manager, main view, and script editor. The main view includes windows for the viewport (the game window), the entity hierarchy of the scene, entity components, project file explorer, project preferences, etc.

![ImGUI](/ReadmeAssets/imgui.png)

## Scripting system

This scripting language is a node-based system primarily designed to be used in a visual manner, inspired by the Blueprints system in Unreal Engine. The execution of the language follows a flow, allowing the order of node execution to be established, ensuring that the script behaves consistently.

![Scripting](/ReadmeAssets/scripting.jpg)

## Gallery

### Editor

Editor main view (viewport, hierarchy, components, file explorer, console):

![Editor](/ReadmeAssets/edit.PNG)

Project selection view (create project or open recent projects)

![ProjectSelection](/ReadmeAssets/Projects.png)

Scripting view (node-base visual scripting)

![Scripting](/ReadmeAssets/Script.png)

### Game developed to showcase engine's features

2D Platform game: [Gameplay](https://youtu.be/45KvW7YF6mU)

![Plataformas](/ReadmeAssets/Plataformas.png)

Space Invaders Clone: [Gameplay](https://youtu.be/KOFQnh82AHU)

![SpaceInvaders](/ReadmeAssets/SpaceInvaders.png)

## Contributions

### Pablo

My contribution to the project primarily focused on the engine. Initially, I researched potential libraries for both physics and audio integration into the engine. We decided to use SDL for graphics, as we were familiar with it from our degree and it posed no limitations for 2D game development.

Once we chose Box2D for physics and SDLMixer for audio, I began setting up the project using Visual Studio 2022. I organized the solution into several projects: physics, audio, input, render, and implemented each of them. I started with the Input project, which took some time as I implemented support for keyboard, controller, and multiple controllers. Once finished, I moved on to the audio project. With the help of SDLMixer documentation, I implemented support for playing sound effects/short sounds and music. Following that, I tackled the physics project, dedicating more time to learn about the library by reading articles and documentation due to its complexity. I also explored the option of visualizing the colliders of physical bodies, a feature beneficial for both engine development and user understanding. While Box2D's documentation had examples using OpenGL for drawing, I had to implement those drawing functions with SDL since our engine uses SDL.

Next, I implemented the ECS (EntityComponent-System) project, crucial for testing and visualizing initial scenes. I also implemented the main loop of the engine with a fixed time interval for the physical world. With the main projects implemented, I started developing the first basic components such as Transform, Image, PhysicBody, and SoundEmitter.

For a period, I focused on expanding and testing the components and functionality implemented in the projects. For example, I added a collision matrix to the physics project for collision filtering, implemented various types of PhysicBody (colliders with special shapes), synchronized physical bodies with entity transformations, collision detection, pixel-to-physics unit conversion, a new ParticleSystem component for particle systems, implemented a resource manager to avoid loading duplicate resources, enhanced sound components to include horizontal panning and 2D sound, among other features.

As the editor progressed, I implemented logic to read the data generated by the editor, such as prefabs. Additionally, I implemented the project management window in the editor, added error handling throughout the engine to ensure continuous and predictable execution by printing errors to the standard output. I also introduced error handling in the editor, logging error information during execution to a log file. I created a directory structure for both the editor and the engine, making the development cycle more comfortable. I implemented a preferences window to adjust engine parameters, such as gravity of the physical world, audio engine frequency, game window size, among many others. I implemented scene flow, saving the last opened scene, displaying the viewport in a special way if no scene is open, and showing the name of the current scene along with its corresponding content in the viewport.

Finally, I changed the SDL usage for controller implementation in the Input project from SDLJoystick to SDLGameController, as it is better suited for controllers. I added more parameters to the preferences window, especially for Input, quick key binding. I also improved the project management window to allow project deletion and sorting projects by last opening order.

## Ivan

My main responsibility in the project focused on the development of the editor. In the initial phase, my task was to carry out thorough research on similar projects to understand the graphics libraries they used and evaluate whether any of them could simplify our work. After a detailed analysis, I concluded that IMGUI was the ideal choice due to its wide range of user interface (UI) functionalities. Once this library was selected, I proceeded to create the project in Visual Studio 2022. I started by building a simple initial program to understand how IMGUI is initialized and works, which included a project with numerous examples. My initial focus was on designing the main windows, such as the menu bar, hierarchy, scene, components, and file explorer. This was done quickly and provisionally, solely for the purpose of familiarizing myself with IMGUI.

During this process, I discovered the Docking branch of ImGUI, which allowed windows to be docked to each other an invaluable feature for the project. I then restructured the code to achieve a more intuitive organization of the windows and make it easier to add new ones. To do this, I created the Window class, which serves as the parent class for each window and provides the basic functionality of an ImGUI window.

As the project progressed, I focused on building the scene, a process that was initially somewhat complicated due to the programming complexity, especially regarding texture handling and rendering with ImGUI. However, over time I managed to understand these aspects and refactor the code to obtain a simpler way to render the scene and its entities. With the scene functional, I worked on creating entities and rendering them within the texture. For that, I created the Entity class, responsible for managing its texture and storing information about its Transform, as well as assigning a unique ID to each entity to differentiate them.

Next, I focused on managing these entities through the hierarchy window, adding a list with selectable texts using ImGUI, which allowed entity selection. This also affected other windows, such as the components window, which displayed information about the selected entity. Initially, the components window only showed the Transform information with ImGUI inputs to modify its values.

Finally, I developed the file explorer window, which, although I initially considered non-essential, turned out to be important for viewing the project's assets and navigating between directories. Once we had a basic version of the editor, I worked on its integration with the project’s engine. This involved serializing scenes, entities, and their transforms into JSON format. Later, I had to adapt the serialization process to be compatible with the engine, as it used different ways to read the available components, their attributes, and their types.

Once the engine components were integrated into the editor, I focused on rendering and editing them so that each attribute type had its own representation in the components window something that was efficiently achieved thanks to ImGUI. I also enabled adding those components to entities.

After that, I implemented more advanced features such as the use of prefabs and the management of the hierarchy between entities. This was a significant challenge, as it involved reworking the ID assignment system so that prefabs had negative IDs and were referenced in all their instances. It was also necessary to take this ID system into account in many parts of the code to avoid conflicts with normal entity usage.

Hierarchy management also introduced complexity, as each entity contained references to its parent and children, which had to be considered throughout the codebase. The interaction between prefabs and the hierarchy was also challenging, as a prefab could have children. I then implemented a system so that parent entities would affect the Transform of their children, making it easier to interact with them in the scene for example, moving multiple entities together when moving the parent. Additionally, to manage prefabs, I created a window called PrefabManager, where prefabs could be viewed and edited.

Finally, together with the team, we worked on improving the editor and fixing its bugs. To test our work, I developed a basic version of the game Space Invaders using our own editor, identifying and fixing numerous bugs in the process.

## Yojhan

My main tasks involved the development of the scripting language, including its implementation in both the editor and the engine, as well as the integration between the editor and the engine.

Initially, I had to create an empty Visual Studio project to start prototyping the language. I didn’t want to use any external libraries that implemented logic for a node-based editor, as I preferred to develop it from scratch. I researched how other engines like Unity and Unreal handled scripting to guide our own implementation, though we didn’t want to copy their scripting systems or node structures. Our goal was to develop a custom language tailored to the specific needs of our engine.

In this initial phase, we had to make key decisions, such as whether the scripting language would be compiled into C++ code or interpreted. It was an iterative process in which functionality was gradually added, and various prototypes were created using Unity as a reference for the visual aspect. During these prototypes, we tested compiling scripts to improve performance. I developed logic to translate scripts into C++ code, and tested it successfully on small scripts. The next step was to compile this code into a DLL that could be linked with the engine. However, after researching how to handle the compilation process, we didn’t find any solution that fully convinced us most required the installation of additional tools or lacked proper DLL support. Eventually, after team discussions, we decided that scripts would be interpreted instead of compiled.

Once we had a robust enough implementation, we decided to merge the first two parts of our thesis: the engine and the scripting system. It wasn’t enough to just include the existing files (some of which needed to be converted from C# to C++ and required the addition of the nlohmann JSON library). To properly integrate the systems, I had to create a series of engine classes that acted as a bridge for scripting. This included classes like Script and ScriptManager, which were essential for integration. With scripting integrated into the engine’s game loop, it was time to run tests and confirm everything worked as expected. At this point, I created the ScriptFunctionality class to provide the scripting language with basic functionality like arithmetic operations and console output.

However, scripting hit a roadblock: it couldn’t access engine functionality. So I paused scripting development and focused on the integration between the editor and the engine. After researching possible solutions and identifying C++’s limitations, we decided to manually overcome those limitations, which led to the creation of the ECSReader project. This allowed the engine's contents to be read through special tags added in the code. These tags enabled developers to choose which parts of the code would be exposed to the editor. Once completed, ECSReader serialized all relevant engine information into a JSON file, allowing the editor to read values from that file.

Moreover, ECSReader was used not only to generate JSON files but also to automate various engine tasks, saving time that would’ve otherwise been spent on manual implementation. This included generating a component factory, enabling reflection on component attributes, and crucially for scripting creating a map of all engine functions so they could be accessed from scripts. With this done, I continued testing scripts, now with access to engine functionality, fixing bugs as I encountered them.

The best way to validate the scripting system was to use it extensively, so I developed a few small games to identify any limitations. These included a small Flappy Bird clone and a real-time clock (not quite a game, but rather a test). The downside was that scripting was only linked to the engine not the editor so every script had to be written manually in a text editor, which became tedious and error-prone as scripts grew in size. I planned to implement the script editor window, but since the editor wasn’t yet mature enough, I chose to help my teammate with engine development.

In the engine, I helped my teammate by implementing features that were partially developed. Given my previous experience with nlohmann JSON, the first thing I implemented was scene and entity serialization. After that, I developed a system for overlays, which are UI elements within the engine, along with their respective components (OverlayImage, OverlayText, and OverlayButton). I also implemented deserialization for engine configuration values like window size or game name, allowing future extensions. I added entity hierarchy functionality and implemented logic for a splash screen displaying the engine’s logo. With reflection and core engine features complete, and as I was ahead in my part of the project, I helped teammates with additional tasks. These included serializing scenes and entities, implementing the overlay system, splash screen, project info serialization (window size, icon), and adding camera movement and scaling logic. I also created the RenderManager within the ECS to control entity render order. Lastly, I created a ConsoleManager to display console messages in a unified format, showing which component or script generated the message and the time it occurred, along with proper error handling.

When the editor reached a more advanced state, I began implementing the visual node editor within the editor. Once everything was mostly ready script creation, serialization I refactored the window system to allow users to move and group them freely using ImGui’s experimental docking branch. I implemented reading of data generated by the engine using ComponentReader and ComponentManager. I also updated the scene window and entity rendering system, adding UI element rendering support.

With everything in place, I focused on adding more functionality to the editor. This included the ability to run the game in a new window, with the option to stop execution from the editor at any time. I also added a feature to create builds, gathering all necessary assets and the engine executable into a single folder, allowing users to change the name and icon of the executable. Other windows I added included a basic preferences window, a docking layout manager, and a color palette editor.

Finally, we used the engine and editor to develop games, fixing bugs as they appeared. I was also responsible for preparing and conducting user testing.
