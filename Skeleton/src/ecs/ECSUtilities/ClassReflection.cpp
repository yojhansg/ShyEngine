#include "ClassReflection.h"
#include <Components/Image.h>
#include <Components/Transform.h>

void ClassReflection::hola() {

}
void ClassReflection::ReflectTransform(Transform* self, std::map<std::string, std::string> const& map) {
	if (map.contains("x"))
	{
		self->x = std::stof(map.at("x"));
	}
}
