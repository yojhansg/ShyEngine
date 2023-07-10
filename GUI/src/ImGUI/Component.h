#pragma once
#include <iostream>

enum  componentType { transform };

class Component
{
public:
	componentType type;
	virtual void drawEditor() = 0;
	virtual std::string toJson() = 0;
};

