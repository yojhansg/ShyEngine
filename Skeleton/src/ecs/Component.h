#pragma once

class Entity;

class Component {

	friend Entity;

public:

	Component();

	virtual ~Component() {};

private:

	virtual void init() {};
	
	virtual void update() {};

	virtual void fixedUpdate() {};


	Entity* entity;

	bool enabled;

};

