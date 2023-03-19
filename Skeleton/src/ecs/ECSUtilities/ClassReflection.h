#pragma once

#include <string>
#include <map>

//Creation time : Sun Mar 19 04:38:37 2023
namespace ECS {
	class Transform;
}
using namespace ECS;
class ClassReflection{
public:
	void hola( );
	static void ReflectTransform(Transform* self, std::map<std::string, std::string> const& map);
};
