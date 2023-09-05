#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include "Texture.h"

#include "nlohmann/json_fwd.hpp"

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

#define PI 3.14159265358979323846264338327950288f
namespace ShyEditor {

	class Camera;
	class Texture;
	class Font;

	class Transform;
	class Overlay;

	class Entity {

	public:
		
		// Static variable to store the last assigned id (Needed for ids assingment)
		static std::vector<int> unusedIds;
		static int lastId;

		Entity(std::string& path, bool isTransform);
		Entity(const Entity& entity, bool isPasting = false);

		~Entity();

		// Render, update and input
		void RenderTransform(SDL_Renderer* renderer, Camera* camera);
		void Update();
		bool HandleInput(SDL_Event* event, bool isMouseInsideEntity, ImVec2 mousePos);

		// Name, ID and texture getters/setters
		std::string GetName();
		void SetName(const std::string& newName);
		Texture* GetTexture();
		ImVec2& GetTextureSize();
		int GetId();
		void SetId(int id);

		// Prefab related getters/setters
		void SetPrefabId(int prefabId);
		int GetPrefabId();
		bool IsPrefab() const;
		bool IsPrefabInstance() const;
		Entity* GetTopParentPrefab();

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
		ImVec2 GetAdjustedPosition();
		ImVec2 GetLocalPosition();
		ImVec2 GetWorldPosition();

		float GetLocalRotation();
		float GetWorldRotation();

		ImVec2 GetLocalScale();
		ImVec2 GetWorldScale();

		void SetLocalPosition(ImVec2& newPos);
		void SetWorldPosition(ImVec2& pos);

		void SetLocalRotation(float r);
		void SetWorldRotation(float r);

		void SetLocalScale(ImVec2& newScale);
		void SetWorldScale(ImVec2& scale);

	
		Overlay* GetOverlay();


		// Deleting entity logic
		bool IsWaitingToDelete();
		void ToDelete();

		// Entity children and parent logic
		void SetParent(Entity* entity, bool adjustToParent = true);
		Entity* GetParent();
		void RemoveChild(Entity* entity);
		void AddChild(Entity* entity);
		std::unordered_map<int, Entity*> GetChildren();
		bool IsAscendant(Entity* entity);

		// Entity transform, components and scripts drawing
		void DrawTransformInEditor();
		void DrawOverlayInEditor();
		bool DrawComponentsInEditor();
		bool DrawScriptsInEditor();

		//Add an attribute that references this object
		void AddReferenceToEntity(Components::Attribute* attr);

		// Serialization and deseralization logic
		std::string ToJson();
		static Entity* FromJson(nlohmann::ordered_json& json);

	private:

		// Entity name
		std::string name;

		// Entity id
		int id;

		static void AssignId(Entity* entity);
		static bool IsIdAlreadyUsed(int id);

		// Entity components, scripts, children and parent
		std::unordered_map<std::string, ::Components::Component> components;
		std::unordered_map<std::string, Components::Script> scripts;
		std::unordered_map<int, Entity*> children;
		Entity* parent;

		// A reference to the editor singleton
		Editor* editor;

		// References to attributes of type entity that reference this entity
		std::list<Components::Attribute*> referencesToEntity;

		// Entity visibility
		bool visible;
		bool showGizmo;
		int renderOrder;

		int prefabId;

		//Positional components
		bool isTransform;
		Transform* transform;
		Overlay* overlay;

		ImVec2* textureSize;


		// Entity texture and gizmo, path to the image and image component
		Components::Component* imageComponent;
		std::string imagePath;

		Texture* texture;
		Texture* gizmo;

		// Some other needed information (Input and deleting entity logic)
		bool leftMouseButtonDown;
		bool rightMouseButtonDown;
		float previousMousePosX;
		float previousMousePosY;
		bool waitingToDelete;

		// Editor drawing methods
		bool DrawFloat(std::string attrName, Components::Attribute* attr);
		bool DrawVector2(std::string attrName, Components::Attribute* attr);
		bool DrawString(std::string attrName, Components::Attribute* attr, bool readOnly = false);
		bool DrawBool(std::string attrName, Components::Attribute* attr);
		bool DrawColor(std::string attrName, Components::Attribute* attr);
		bool DrawChar(std::string attrName, Components::Attribute* attr);
		bool DrawEntity(std::string attrName, Components::Attribute* attr);

		void DrawArrowButton(ImVec2& value, const ImVec2& dir);

		// Entity children settings
		void SetChildrenVisible(Entity* entity, bool visible);

		// Sets the relative transform when setting a newParent
		void SetTransformRelativeToNewParent();
		void SetTransformToWorldValues();
	};



//Components


	class Transform {

		Entity* obj;

		// Transform attributes
		ImVec2* scale;
		ImVec2* position;
		float rotation;

	public:
		Transform(Entity* obj);
		Transform(const Transform& tr, Entity* obj);
		~Transform();
		
		ImVec2 GetLocalPosition();
		ImVec2 GetWorldPosition();

		float GetLocalRotation();
		float GetWorldRotation();

		ImVec2 GetLocalScale();
		ImVec2 GetWorldScale();

		void SetLocalPosition(ImVec2& newPos);
		void SetWorldPosition(ImVec2& pos);


		void SetLocalRotation(float r); 
		void SetWorldRotation(float r);

		void SetLocalScale(ImVec2& newScale);
		void SetWorldScale(ImVec2& scale);

		static ImVec2& rotate(float degrees, ImVec2 vec);
	};



	class OverlayImage;
	class OverlayText;

	class Overlay {

		Entity* obj;


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

		Overlay(Entity* obj);
		Overlay(const Overlay& ov, Entity* obj);
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


		enum class Fit {
			Clamp, Overflow, WrapClamp, WrapOverflow, Expand
		};

		enum class HorizontalAlignment {
			Left, Center, Right
		};

		enum class VerticalAlignment {
			Top, Center, Bottom
		};


		std::string path;
		std::string text;


		Font* font;
		Texture* texture;

		int texture_w;
		int texture_h;

		int fontSize;

		int horizontalAlignment;
		int verticalAlignment;

		float lineSpacing;

		int width;

		int fit;

	public:

		OverlayText();
		~OverlayText();

		std::string GetPath();
		std::string GetText();

		void Clear();
		void SetText(const std::string text, const std::string path, int size, int fit, int hAlign, int vAlign, int width);
		Texture* GetTexture();

		void Render(SDL_Renderer* renderer, int x, int y, int w, int h);

	};

}