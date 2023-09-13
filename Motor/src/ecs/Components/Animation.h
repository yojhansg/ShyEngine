#pragma once

#include "Component.h"
#include <RendererManager.h>

namespace ECS {

	class Transform;

	class Image;
	class Animation : public Component {

	public:

		enum class AnimationSheet {
			Horizontal, Vertical
		};

		Animation();
		~Animation();

		void init() override;
		void start() override;
		void update(float dt) override;

	publish:

		void ChangeAnimationPath(cstring str, int frame, int animation, int column, int row);

		void AdvanceFrame();
		void AdvanceAnimation();

		void SetFrame(int idx);
		void SetAnimation(int idx);

		void SetAnimationSpeed(float speed);

		int GetFrame();
		int GetAnimation();

		void SetAnimationLenght(int lenght);
		int getAnimationLenght();

	private:

		void ChangeAnimationFrame();

		reflect float speed;

		reflect int currentFrame;
		reflect int currentAnimation;

		reflect int animationColumns;
		reflect int animationRows;

		reflect int animationSheetStyle;
		reflect int animationLenght;

		int frameWidth;
		int frameHeight;

		float animationTimer;
		Image* image;
	};

}
