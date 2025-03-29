#pragma once
#include <cmath>

struct Vector2f {

	Vector2f() :x(0.0f), y(0.0f) {}
	Vector2f(float p_x, float p_y) :x(p_x), y(p_y) {}

	Vector2f	operator*(float scalar) const { return Vector2f(x * scalar, y * scalar); }
	Vector2f	operator-() const { return Vector2f{ -x, -y }; }

	float 		getAngle() { return static_cast<float>(atan2(-x, y) * (180.0 / M_PI)); }

	bool 		operator==(const Vector2f& rhs) const { return (rhs.x == this->x && this->y == rhs.y); }
	bool 		operator!=(const Vector2f& rhs) const { return (rhs.x != this->x && this->y != rhs.y); }

	float 		x;
	float 		y;
};