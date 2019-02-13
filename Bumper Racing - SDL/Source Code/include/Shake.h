#pragma once
#include <string>
#include "Vector2.h"
#include "PerlinNoise.h"
#include "SDL.h"

namespace Camera2D
{
	const float SHAKE_DEFAULT_DURATION = 5.f;
	const float SHAKE_DEFAULT_SPEED = 2.f;
	const float SHAKE_DEFAULT_MAGNITUDE = 1.f;
	const float SHAKE_DEFAULT_RANGE = 1000.f;

	class Shake
	{
	public:
		Shake() {};
		void init();
		void update(float deltaTime);
		void setProps(float duration = SHAKE_DEFAULT_DURATION, float speed = SHAKE_DEFAULT_SPEED, float magnitude = SHAKE_DEFAULT_MAGNITUDE, float range = SHAKE_DEFAULT_RANGE);
		void setDuration(float duration);
		float getDuration() const;
		void setSpeed(float speed);
		float getSpeed() const;
		void setMagnitude(float magnitude);
		float getMagnitude() const;
		void setRange(float range);
		float getRange() const;
		Vector2 getShakeOffset() const;
		bool getEnabled() const;
		void setEnabled(bool enabled);
		std::string getName() const;
		void setName(const std::string& name);
	private:

		inline float randomNumberF(float min, float max)
		{
			return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
		}
		inline float clamp(float value, float min, float max)
		{
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}

			return value;
		}

		std::string m_name;
		bool m_enabled;

		Vector2 m_shakeOffset;

		float m_elapsedTime;
		float m_duration;
		float m_speed;
		float m_magnitude;
		float m_startRange;
	};
}