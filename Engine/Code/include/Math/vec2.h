#pragma once
#include <iostream>
#include <cmath>

//Class for vec2
class vec2
{
public:
	// vector components
	float x;
	float y;

	// different ways of initializing a vector
	vec2() : x(0.f), y(0.f) {}
	vec2(const float _x, const float _y) : x(_x), y(_y) {}
	explicit vec2(const float coords) : x(coords), y(coords) {}
	~vec2() = default;

	// different print methods
	void Print() const;
	void BlankPrint() const;

	void Opposite(); // returns the vector's opposite
	void Invert(); // returns the vector's invert
	void AddNumber(float number); // adds a number to the vector
	void MultiplyNumber(float number); // multiplies the vector by a number

	float operator[](int index) const; // get a component with an index

	static vec2 Add(vec2 v1, vec2 v2); // adds two vectors
	static vec2 Multiply(vec2 v1, vec2 v2); // multiplies two vectors
	static vec2 MidPoint(vec2 v1, vec2 v2); // get the mid-point of two points
	static float Distance(vec2 p1, vec2 p2); // get the distance between two points
	static float SquaredNorm(vec2 v); // returns the squared norm of a vector
	static float Norm(vec2 v); // returns the norm of a vector
	static vec2 Normalize(vec2 v); //returns normalized vector
	static float DotProduct(vec2 v1, vec2 v2); // returns the dot product of two vectors
	static float CrossProduct(vec2 v1, vec2 v2); // returns the cross product of two vectors
	static float GetAngle(vec2 v1, vec2 v2); // returns the angle between two vectors
	static vec2 Rotate(vec2 p, float theta, vec2 anchor = vec2()); // rotates a point around another point
};

inline bool operator==(const vec2& v1, const vec2& v2)
{
	return fabsf(v1.x - v2.x) < std::numeric_limits<float>::epsilon()
	&& fabsf(v1.y - v2.y) < std::numeric_limits<float>::epsilon();
}

inline vec2 operator+(const vec2& v, const float& f)
{
	vec2 vR = v;
	vR.AddNumber(f);
	return vR;
}

inline vec2 operator+(const float& f, const vec2& v)
{
	return v + f;
}

inline vec2 operator-(const vec2& v, const float& f)
{
	vec2 vR = v;
	vR.AddNumber(-f);
	return vR;
}

inline vec2 operator*(const vec2& v, const float& f)
{
	vec2 vR = v;

	vR.MultiplyNumber(f);

	return vR;
}

inline vec2 operator*(const float& f, const vec2& v)
{
	return v * f;
}

inline vec2 operator/(const vec2& v, const float& f)
{
	vec2 vR = v;
	vR.MultiplyNumber(1.f / f);
	return vR;
}

inline vec2 operator+(const vec2& v1, const vec2& v2)
{
	return vec2::Add(v1, v2);
}

inline vec2 operator-(const vec2& v1, const vec2& v2)
{
	vec2 v3 = v2;
	v3.Opposite();
	return vec2::Add(v1, v3);
}

inline vec2 operator*(const vec2& v1, const vec2& v2)
{
	return vec2::Multiply(v1, v2);
}

inline vec2 operator/(const vec2& v1, const vec2& v2)
{
	vec2 v3 = v2;
	v3.Invert();
	return vec2::Multiply(v1, v3);
}

inline vec2 operator+=(const vec2& v, const float& f)
{
	return v + f;
}

inline vec2 operator-=(const vec2& v, const float& f)
{
	return v - f;
}

inline vec2 operator*=(const vec2& v, const float& f)
{
	return v * f;
}

inline vec2 operator/=(const vec2& v, const float& f)
{
	return v / f;
}

inline vec2& operator+=(vec2& v1, const vec2& v2)
{
	return v1 = v1 + v2;
}

inline vec2& operator-=(vec2& v1, const vec2& v2)
{
	return v1 = v1 - v2;
}

inline vec2& operator*=(vec2& v1, const vec2& v2)
{
	return v1 = v1 * v2;
}

inline vec2& operator/=(vec2& v1, const vec2& v2)
{
	return v1 = v1 / v2;
}

inline void vec2::Print() const
{
	std::cout << "vec2 : " << "x = " << x << ", y = " << y << '\n';
}

inline void vec2::BlankPrint() const
{
	std::cout << x << "  " << y << '\n';
}

inline void vec2::Opposite()
{
	x *= -1.f;
	y *= -1.f;
}

inline void vec2::Invert()
{
	x = 1 / x;
	y = 1 / y;
}

inline void vec2::AddNumber(const float number)
{
	x += number;
	y += number;
}

inline void vec2::MultiplyNumber(const float number)
{
	x *= number;
	y *= number;
}

inline vec2 vec2::Add(const vec2 v1, const vec2 v2)
{
	vec2 v;

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;

	return v;
}

inline vec2 vec2::Multiply(const vec2 v1, const vec2 v2)
{
	vec2 v;

	v.x = v1.x * v2.x;
	v.y = v1.y * v2.y;

	return v;
}

inline vec2 vec2::MidPoint(const vec2 v1, const vec2 v2)
{
	vec2 v3;

	v3.x = (v1.x + v2.x) / 2;
	v3.y = (v1.y + v2.y) / 2;

	return v3;
}

inline float vec2::Distance(const vec2 p1, const vec2 p2)
{
	const float xSquared = powf(p1.x - p2.x, 2.f);
	const float ySquared = powf(p1.y - p2.y, 2.f);
	const float dist = sqrtf(xSquared + ySquared);

	return dist;
}

inline float vec2::SquaredNorm(const vec2 v)
{
	const float xSquared = powf(v.x, 2.f);
	const float ySquared = powf(v.y, 2.f);
	const float squaredNorm = xSquared + ySquared;

	return squaredNorm;
}

inline float vec2::Norm(const vec2 v)
{
	const float norm = sqrtf(SquaredNorm(v));

	return norm;
}

inline vec2 vec2::Normalize(const vec2 v)
{
	const float norm = Norm(v);

	return { v.x / norm, v.y / norm };
}

inline float vec2::DotProduct(const vec2 v1, const vec2 v2)
{
	const float dotProduct = (v1.x * v2.x) + (v1.y * v2.y);

	return dotProduct;
}

inline float vec2::CrossProduct(const vec2 v1, const vec2 v2)
{
	const float crossProduct = (v1.x * v2.y) - (v2.x * v1.y);

	return crossProduct;
}

inline vec2 vec2::Rotate(const vec2 p, const float theta, const vec2 anchor)
{
	const vec2 pTemp = p - anchor;

	const float x = (pTemp.x * cosf(theta)) - (pTemp.y * sinf(theta));
	const float y = (pTemp.x * sinf(theta)) + (pTemp.y * cosf(theta));

	vec2 vectorRotated(x, y);
	vectorRotated = vectorRotated + anchor;

	return vectorRotated;
}