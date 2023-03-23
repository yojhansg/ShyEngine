#pragma once
enum  componentType { transform };

class Component
{
public:
	componentType type;
	virtual void drawEditor() = 0;
};

