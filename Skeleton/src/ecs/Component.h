#pragma once

#include "Scripting/Variable.h"
#include <map>
#include <vector>
#include <string>


#define ECS_Version 1.0
/*
	Para poder publicar un metodo basta con poner el modificador de
	acceso <publish> encima de los metodos que se quieran publicar.

	Condiciones para poder publicar un metodo:

		-No puede devolver un valor <const> o por referencia
		-No puede recibir un parametro <const> o por referencia
		-El nombre del metodo no puede contener una barra baja
		-El tipo de retorno o de los parametros no puede tener un nombre 
		compuesto por un espacio de nombres o una clase (pe. usar Vector2D en vez Utilities::Vector2d)
		-En caso de ser un puntero, el asterisco no puede estar separado del nombre mediante un espacio

*/
#define publish public

/*
	Para poder tener reflexion sobre un atributo basta con poner la marca <reflect>
	delante del atributo en cuestion

	Condiciones para poder publicar un metodo:

		-
		-No puede recibir un parametro <const> o por referencia
		-El nombre del metodo no puede contener una barra baja
		-El tipo de retorno o de los parametros no puede tener un nombre compuesto por un espacio de nombres o una clase (pe. usar Vector2D en vez Utilities::Vector2d)

*/
#define reflect friend class ClassReflection;
class ClassReflection;

namespace ECS {

	class Entity;
	class Scene;

	class Component {
		friend Entity;

	public:

		Component();

		virtual ~Component() {};

		Entity* getEntity();

		Scene* getScene();

		bool isActive();

		void setActive(bool ent_active);

		void remove();

		void remove(Component* c);

		Scripting::Variable CallMethod(std::string methodName, std::vector<Scripting::Variable> const& input);
		
		Scripting::Variable cacadevaca(std::vector<Scripting::Variable> const&);


	private:

		virtual void init() {};

		virtual void start() {};

		virtual void update(float deltaTime) {};

		virtual void lateUpdate(float deltaTime) {};

		virtual void render() {};

		virtual void fixedUpdate(float fixedDeltaTime) {};

		virtual void onActive() {};

		virtual void onDeacitve() {};

		virtual void onSceneUp() {};

		virtual void onSceneDown() {};

		virtual void onCollisionEnter(Entity* a, Entity* b) {};

		virtual void onCollisionStay() {};

		virtual void onCollisionExit(Entity* a, Entity* b) {};

		virtual void onTriggerEnter() {};

		virtual void onTriggerStay() {};

		virtual void onTriggerExit() {};

		
	protected:

		Entity* entity;
		bool active;
		
		typedef Scripting::Variable (*CallableMethod)(std::vector<Scripting::Variable> const&);
		std::map <std::string, CallableMethod> registeredMethods;
	};
}

