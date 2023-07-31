#pragma once

#include <string>

#include "Vector2D.h"
#include "SDL.h"

#define COLOREXP(color) \
	color.r, color.g, color.b, color.a

namespace Utilities {

	SDL_Color createColor(unsigned long num) {
		return
		{
			 static_cast<Uint8>((num >> 24) & 0xff),
			 static_cast<Uint8>((num >> 16) & 0xff),
			 static_cast<Uint8>((num >> 8) & 0xff),
			 static_cast<Uint8>(num & 0xff)
		};
	}

	SDL_Color createColor(std::string strnum) {
		return createColor(std::stoul(strnum.erase(0, 2), nullptr, 16));
	}

	SDL_Rect createRect(Vector2D& pos, float w, float h) {
		return
		{
			static_cast<int>(pos.getX()),
			static_cast<int>(pos.getY()),
			static_cast<int>(w),
			static_cast<int>(h)
		};
	}

	SDL_Rect createRect(float x, float y, float w, float h) {
		return
		{
			static_cast<int>(x),
			static_cast<int>(y),
			static_cast<int>(w),
			static_cast<int>(h)
		};
	}

	SDL_Rect createRect(int x, int y, int w, int h) {
		return { x, y, w, h };
	}

}



