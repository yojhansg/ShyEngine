#pragma once

#include <cmath>
#include <ostream>
#include <string>

namespace ShyEditor {

	class Vector2 {

	public:

		// Various constructors
		Vector2() noexcept :
			x_(), y_() {
		}

		Vector2(const Vector2& v) :
			x_(v.getX()), y_(v.getY()) {
		}

		Vector2(const Vector2* v) :
			x_(v->getX()), y_(v->getY()) {
		}

		Vector2(float x, float y) :
			x_(x), y_(y) {
		}

		~Vector2() {
		}

		// various getters
		inline float getX() const {
			return x_;
		}

		inline float getY() const {
			return y_;
		}

		// various setters
		inline void setX(float x) {
			x_ = x;
		}

		inline void setY(float y) {
			y_ = y;
		}

		inline void set(float x, float y) {
			x_ = x;
			y_ = y;
		}

		inline void set(const Vector2& v) {
			x_ = v.x_;
			y_ = v.y_;
		}

		inline void set(const Vector2* v) {
			x_ = v->x_;
			y_ = v->y_;
		}

		// copy assignment
		inline Vector2& operator=(const Vector2& v) {
			x_ = v.x_;
			y_ = v.y_;
			return *this;
		}

		// ** various operations

		// Length of the vector
		inline float magnitude() const {
			return sqrtf(powf(x_, 2) + powf(y_, 2));
		}

		// Square length of the vector
		inline float sqrMagnitude() const {
			return x_ * x_ + y_ * y_;
		}

		// Vector in the same direction of length 1
		inline Vector2 normalize() const {
			return *this / magnitude();
		}

		inline float area() const {
			return x_ * y_;
		}

		// Counter clockwise rotation in a normal coordinate system, and
		// it is clockwise rotation if we work with a coordinate system
		// in which the vertical axe is flipped (it is like a mirror over
		// the horizontal axe)-- which the case when working with the SDL.
		//
		Vector2 rotate(float degrees) const;

		// Computes the angle between 'this' and 'v'. The result is
		// between -180 and 180, and is such that the following holds:
		//
		//   this->rotate(angle) == v
		//
		float angle(const Vector2& v) const;

		// Vector subtraction
		inline Vector2 operator-(const Vector2& v) const {
			return Vector2(x_ - v.x_, y_ - v.y_);
		}

		inline Vector2 operator-=(const Vector2& v) {
			x_ -= v.x_;
			y_ -= v.y_;
			return *this;
		}

		// Vector addition
		inline Vector2 operator+(const Vector2& v) const {
			return Vector2(x_ + v.x_, y_ + v.y_);
		}

		inline Vector2 operator+=(const Vector2& v) {
			x_ += v.x_;
			y_ += v.y_;
			return *this;
		}

		inline bool operator!=(const Vector2& v) {
			return x_ != v.x_ || y_ != v.y_;
		}

		inline bool operator==(const Vector2& v) {
			return x_ == v.x_ && y_ == v.y_;
		}

		// Multiplication by constant (scaling)
		inline Vector2 operator*(float d) const {
			return Vector2(x_ * d, y_ * d);
		}

		// Multiplication by constant (scaling)
		inline Vector2 operator*=(float d) const {
			return Vector2(x_ * d, y_ * d);
		}

		inline Vector2 operator*=(float d) {
			x_ *= d;
			y_ *= d;
			return *this;
		}

		inline Vector2 operator*=(const Vector2& v) {
			x_ *= v.x_;
			y_ *= v.y_;
			return *this;
		}

		// Division by constant (scaling)
		inline Vector2 operator/(float d) const {
			return Vector2(x_ / d, y_ / d);
		}

		// Scalar multiplication
		inline float operator*(const Vector2& d) const {
			return d.x_ * x_ + d.y_ * y_;
		}

		// Scalar multiplication
		inline Vector2 mult(const Vector2& d) const {
			return Vector2(d.x_ * x_ , d.y_ * y_);
		}


		inline operator std::string() const{
			return "(" + std::to_string(x_)+ "," + std::to_string(y_) + ")";
		}


		float x_;  // first coordinate
		float y_;  // second coordinate
	};

	// Needed for printing a value of tyep Vector2 with std::cout.
	// The definition is in .cpp
	std::ostream& operator<<(std::ostream& os, const Vector2& v);

}