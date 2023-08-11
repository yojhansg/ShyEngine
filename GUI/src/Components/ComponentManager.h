#pragma once


#include <string>
#include <unordered_map>

#include "ComponentInfo.h"


namespace Components {

	using cstring = std::string const&;

	class ComponentManager {


	private:

		static ComponentManager* instance;
		static void EnsureInitialised();

		std::unordered_map<std::string, Component> components;
		std::unordered_map<std::string, Component> managers;

	public:

		static void Initialise();
		static void Release();

		static void ReadComponentInfo(cstring path);
		static void ReadManagerInfo(cstring path);

		static std::unordered_map<std::string, Component>& GetAllComponents();
		static std::unordered_map<std::string, Component>& GetAllManagers();
	};
}
