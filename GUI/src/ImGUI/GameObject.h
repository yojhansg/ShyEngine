#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include "Texture.h"

struct SDL_Renderer;
union SDL_Event;
struct ImVec2;
struct ImColor;
class Editor;

namespace Components {
	class Component;
	class Attribute;
	class Script;
}

namespace ShyEditor {

	class Camera;
	class Texture;
	class Font;

	class Transform;
	class Overlay;

	class GameObject {

	public:

		static std::vector<int> unusedIds;

		GameObject(std::string& path, bool isTransform);
		GameObject(const GameObject& go);

		~GameObject();

		// Render, update and input
		void RenderTransform(SDL_Renderer* renderer, Camera* camera);
		void Update();
		bool HandleInput(SDL_Event* event, bool isMouseInsideGameObject, ImVec2 mousePos);

		// Name, ID and texture getters/setters
		std::string GetName();
		void SetName(const std::string& newName);
		Texture* GetTexture();
		int GetId();
		void SetId(int id);

		// Prefab related getters/setters
		void SetPrefabId(int prefabId);
		int GetPrefabId();

		// Visibility getters/setters
		int GetRenderOrder();
		bool IsVisible();
		void SetVisible(bool visible);
		bool IsTransform();

		// Components and Scripts logic
		void AddComponent(Components::Component comp);
		void AddScript(Components::Script script);
		std::unordered_map<std::string, Components::Component>* GetComponents();
		std::unordered_map<std::string, Components::Component> GetComponentsCopy();
		std::unordered_map<std::string, Components::Script>* GetScripts();
		std::unordered_map<std::string, Components::Script> GetScriptsCopy();

		void SetComponents(std::unordered_map<std::string, ::Components::Component> components);
		void SetScripts(std::unordered_map<std::string, Components::Script> scripts);

		// Tranform attributes getters/setters
		void SetPosition(ImVec2 newPos);
		ImVec2 GetPosition();
		float GetRotation();
		void SetRotation(float r);
		ImVec2 GetAdjustedPosition();
		ImVec2 GetSize();
		float GetScaleX();
		float GetScaleY();
		void SetScale(float x, float y);

		Overlay* GetOverlay();

		// Deleting gameobject logic
		bool IsWaitingToDelete();
		void ToDelete();

		// Gameobject children and parent logic
		void SetParent(GameObject* go);
		GameObject* GetParent();
		void RemoveChild(GameObject* go);
		void AddChild(GameObject* go);
		std::unordered_map<int, GameObject*> GetChildren();
		bool IsAscendant(GameObject* go);

		// Gameobject transform, components and scripts drawing
		void DrawTransformInEditor();
		void DrawOverlayInEditor();
		bool DrawComponentsInEditor();
		bool DrawScriptsInEditor();


		// Serialization and deseralization logic
		std::string ToJson();
		static GameObject* FromJson(std::string json);

	private:

		// Static variable to store the last assigned id (Needed for ids assingment)
		static int lastId;
		
		// Gameobject name
		std::string name;

		// Gameobject id
		int id;

		static void AssignId(GameObject* go);

		// Gameobject components, scripts, children and parent
		std::unordered_map<std::string, ::Components::Component> components;
		std::unordered_map<std::string, Components::Script> scripts;
		std::unordered_map<int, GameObject*> children;
		GameObject* parent;

		// A reference to the editor singleton
		Editor* editor;

		// Gameobject visibility
		bool visible;
		bool showGizmo;
		int renderOrder;

		int prefabId;

		//Positional components
		bool isTransform;
		Transform* transform;
		Overlay* overlay;

		ImVec2* textureSize;


		// Gameobject texture and gizmo, path to the image and image component
		Components::Component* imageComponent;
		std::string imagePath;

		Texture* texture;
		Texture* gizmo;

		// Some other needed information (Input and deleting gameobject logic)
		bool leftMouseButtonDown;
		bool rightMouseButtonDown;
		float previousMousePosX;
		float previousMousePosY;
		bool waitingToDelete;

		// Gameobject draw methods
		bool DrawFloat(std::string attrName, Components::Attribute* attr);
		bool DrawVector2(std::string attrName, Components::Attribute* attr);
		bool DrawString(std::string attrName, Components::Attribute* attr);
		bool DrawBool(std::string attrName, Components::Attribute* attr);
		bool DrawColor(std::string attrName, Components::Attribute* attr);
		bool DrawChar(std::string attrName, Components::Attribute* attr);
		bool DrawGameobject(std::string attrName, Components::Attribute* attr);

		void DrawArrowButton(ImVec2& value, const ImVec2& dir);

		// Gameobject children settings (Transform and visibility)
		void TranslateChildren(GameObject* go, ImVec2* previousPos);
		void ScaleChildren(GameObject* go, int scaleFactor);
		void SetChildrenVisible(GameObject* go, bool visible);
		void RotateChildren(GameObject* go, GameObject* goCenter, float rotationAngle);
	};



//Components


	class Transform {

		GameObject* obj;

		// Transform attributes
		ImVec2* scale;
		ImVec2* position;
		float rotation;

	public:
		Transform(GameObject* obj);
		Transform(const Transform& tr, GameObject* obj);
		~Transform();
		
		ImVec2 &GetPosition();
		ImVec2 &GetScale();
		float &GetRotation();

		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		void SetRotation(float r);
	};



	class OverlayImage;
	class OverlayText;

	class Overlay {

		GameObject* obj;


		int placement;

		ImVec2* position;
		ImVec2* size;

		ImVec2* anchor;
		int left;
		int top;
		int right;
		int bottom;


		//TODO: escala, color e interactuable
		//ImColor* color;

		float scale;
		bool interactable;


		OverlayImage* image;
		OverlayText* text;
	public:

		enum class Placement {

			Positioned, Stretched
		};

		Overlay(GameObject* obj);
		Overlay(const Overlay& ov, GameObject* obj);
		~Overlay();

		int& GetPlacement();

		ImVec2& GetAnchor();

		float& GetScale();

		bool& GetInteractable();

		OverlayImage* GetImage();

		ImVec2& GetPosition();
		void SetPosition(ImVec2* pos);

		ImVec2& GetSize();


		int& GetLeft();
		int& GetRight();
		int& GetTop();
		int& GetBottom();



		void CalculateRectangle(int& x, int& y, int& w, int& h);
		ImVec2 CalculateCenterPoint();

		void Update();
		void Render(SDL_Renderer* renderer, int x, int y, int w, int h);
	};




	class OverlayImage {

	private:

		Texture* texture;
		std::string path;


	public:
		OverlayImage();

		void Render(SDL_Renderer* renderer, int x, int y, int w, int h);

		std::string GetPath();
		Texture* GetTexture();

		void SetTexture(std::string path, Texture* texture);
	};



	class OverlayText {

	private:
		std::string path;
		std::string text;


		Font* font;
		Texture* texture;

		int fontSize;
		int maxWidth;

	public:

		OverlayText();
		~OverlayText();

		std::string GetPath();
		std::string GetText();

		void Clear();
		void SetText(const std::string text, const std::string path, int size, int maxWidth);
		Texture* GetTexture();

		void Render(SDL_Renderer* renderer, int x, int y, int w, int h);

	};

}