#pragma once


#include <string>
#include <map>


namespace Components {

	using cstring = std::string const&;

	class ComponentManager {


	private:

		static ComponentManager* instance;
		static void EnsureInitialised();

		ComponentManager();
	public:

		static void Initialise();
		static void Release();

		static void ReadComponentInfo(cstring components, cstring functions, cstring attributes);
	};
}
