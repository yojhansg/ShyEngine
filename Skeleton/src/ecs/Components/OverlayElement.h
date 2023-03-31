#pragma once


#include <Component.h>
#include <Vector2D.h>


namespace ECS {


	class OverlayElement : public Component {

	public:

		enum class Placement {

			Positioned, Stretched
		};

		OverlayElement();

	private:

		reflect int placement;

		reflect Utilities::Vector2D position;
		reflect Utilities::Vector2D size;
		reflect Utilities::Vector2D anchor;

		reflect int left;
		reflect int top;
		reflect int right;
		reflect int bottom;

	publish:

		int GetPlacement();

		void SetPositioned(Utilities::Vector2D position, Utilities::Vector2D size);
		void SetStreched(int left, int top, int right, int bottom);

		void Move(Utilities::Vector2D position);
		void MoveTo(Utilities::Vector2D position);

		void SetSize(Utilities::Vector2D size);

		Utilities::Vector2D GetPosition();
		Utilities::Vector2D GetSize();

		int GetTop();
		int GetLeft();
		int GetRight();
		int GetBottom();

		void SetTop(int pos);
		void SetLeft(int pos);
		void SetRight(int pos);
		void SetBottom(int pos);


		Utilities::Vector2D GetAnchor();
		void SetAnchor(Utilities::Vector2D anchor);

		void SetAnchorCenter();
		void SetAnchorTopLeft();
		void SetAnchorTopRight();
		void SetAnchorBottomLeft();
		void SetAnchorBottomRight();

	public:

		void CalculateDestinationRect(int& x, int& y, int& w, int& h);
	};
}