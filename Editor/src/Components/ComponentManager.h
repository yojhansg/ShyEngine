#pragma once

#include <string>
#include <unordered_map>

#include "ComponentInfo.h"

template <typename T>
using ordered_string_map = std::unordered_map<std::string, T>;

template <typename T>
using string_map = std::map<std::string, T>;

namespace Components {

	using cstring = std::string const&;

	class ComponentManager {

	private:

		static ComponentManager* instance;
		static void EnsureInitialised();

		/*
			Para almacenar componentes usamos un mapa en lugar de un 'unordered_map'
			para convervar el orden (alfabetico) de los componentes
		*/

		string_map<Component> components;
		string_map<Component> managers;

		static std::string scriptPath;
		std::vector<Script> scripts;

		ordered_string_map<std::string> info;
		string_map<std::vector<std::string>> enums;
	public:

		static void Initialise();
		static void Release();

		static void ReadComponents(cstring path);
		static void ReadManagers(cstring path);
		static void ReadInfo(cstring path);

		static void ReadScripts(cstring path);
		static void ReloadScripts();

		static string_map<Component>& GetAllComponents();
		static string_map<Component>& GetAllManagers();

		static bool GetComponentInfo(std::string& value, cstring component);
		static bool GetComponentElementInfo(std::string* value, cstring component, cstring element, cstring info);

		static const std::vector<std::string>& GetEnum(cstring name);
		static std::vector<std::string> GetEnumNames();

		static std::vector<Script>& GetAllScripts();
	};
}
