#pragma once
#include <cmath>

class Vector2b
{
public:
	Vector2b(float pX = 0, float pY = 0)
		: x(pX), y(pY) 
	{ 
	}
	~Vector2b() {}

	float Vector2b::Length() const
	{
		return sqrt(x * x + y * y);
	}

	Vector2b Vector2b::Normalize() const
	{
		Vector2b vector;
		float length = this->Length();

		if (length != 0)
		{
			vector.x = float(x) / length;
			vector.y = float(y) / length;
		}

		return vector;
	}

	Vector2b operator * (const float val) const
	{
		return Vector2b(x * val, y * val);
	}
	Vector2b operator - (const Vector2b pVec) const
	{
		return Vector2b(x - pVec.x, y - pVec.y);
	}
	Vector2b operator + (const Vector2b pVec) const
	{
		return Vector2b(x + pVec.x, y + pVec.y);
	}

	Vector2b operator / (const float& pRhs)
	{
		return Vector2b(x / pRhs, y / pRhs);
	}


	Vector2b& operator+=(const Vector2b pVec)
	{
		this->x += pVec.x;
		this->y += pVec.y;
		return *this;
	}

	bool operator!=(const Vector2b& rhs)
	{
		return (x != rhs.x || y != rhs.y);
	}

	bool operator==(const Vector2b& rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}

	Vector2b& operator*=(const float& x)
	{
		this->x *= x;
		this->y *= x;
		return *this;
	}

	static float CalculateDistance(const Vector2b& p1, const Vector2b p2)
	{
		float diffY = p1.y - p2.y;
		float diffX = p1.x - p2.x;
		return sqrt((diffY * diffY) + (diffX * diffX));
	}


	static Vector2b NormaliseVec(Vector2b vecA)
	{
		float lengthA = sqrt((vecA.x * vecA.x) + (vecA.y * vecA.y));
		if (lengthA != 0)
		{
			vecA.x /= lengthA;
			vecA.y /= lengthA;
		}
		return vecA;
	}


	float x;
	float y;
};