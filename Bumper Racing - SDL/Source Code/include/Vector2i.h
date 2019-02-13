#pragma once

class Vector2i
{
public:
	Vector2i(int pX = 0, int pY = 0) : x(pX), y(pY) { }
	~Vector2i() {}

	bool operator<(const Vector2i& b) const { return x < b.x; }

	Vector2i operator * (const float val) const
	{
		return Vector2i(x * val, y * val);
	}

	Vector2i operator + (const Vector2i val) const
	{
		return Vector2i(x + val.x, y * val.y);
	}

	Vector2i operator - (const Vector2i pVec) const
	{
		return Vector2i(x - pVec.x, y - pVec.y);
	}

	int x;
	int y;
};
