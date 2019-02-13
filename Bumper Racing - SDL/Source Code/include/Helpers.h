#pragma once
#include "ConstHolder.h"
#include <random>

//clamps a value between min and max-1
inline int clamp(int value, int min, int max)
{
	if (value < 0)
	{
		return max - 1;
	}
	if (value >= max)
	{
		return 0;
	}

	return value;
}

inline float randomNumberF(float min, float max)
{
	return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

inline int randomNumber(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

inline float metersToPixels(float meters)
{
	return meters * PIXELS_PER_METER;
}

inline float pixelsToMeters(float pixels)
{
	return pixels / PIXELS_PER_METER;
}

inline float lerp(float start, float end, float percent)
{
	if (percent > 1.f)
		percent = 1.f;
	else if (percent < 0.f)
		percent = 0.f;

	return (1 - percent) * start + (percent * end);
}