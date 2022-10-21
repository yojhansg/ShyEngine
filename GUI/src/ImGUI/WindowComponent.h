#pragma once

union SDL_Event;

class WindowComponent
{
public:

	virtual void render(){};
	virtual void handleInput(SDL_Event* event){};
};

