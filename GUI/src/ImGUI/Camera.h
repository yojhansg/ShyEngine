#pragma once
#include <vector>

union SDL_Event;
struct ImVec2;
class Image;

struct SDL_Renderer;
struct SDL_Texture;

namespace ShyEditor {

	class Camera {

	private:

		float scale;
		float camera_position_x, camera_position_y;

		bool middleMouseButton;

		float minScale, maxScale;

		int texture_w, texture_h;
		SDL_Renderer* renderer = nullptr;
		SDL_Texture* targetTexture = nullptr;

		void GenerateTexture(int w, int h);

	public:

		Camera(ImVec2 position, float scale, SDL_Renderer* renderer);
		~Camera();

		void handleInput(SDL_Event* event, bool isMouseInsideWindow);

		void SetConstrains(float min, float max);

		ImVec2 GetPosition();
		float& GetScale();

		void AddPosition(float x, float y);

		float GetMinScale();
		float GetMaxScale();

		bool ShouldResize(int w, int h);
		void Resize(int w, int h);

		void PrepareCameraRender();
		void StopCameraRender();

		SDL_Texture* GetTexture();

		void CenterPosition(float& x, float& y);
	};

}

