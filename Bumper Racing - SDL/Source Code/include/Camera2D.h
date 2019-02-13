#pragma once

#include "SDL.h"
#include <vector>
#include "Shake.h"

using namespace std;

namespace Camera2D
{
	const float DEFAULT_ACCEL = 100.f; //every second how much you change velocity by
	const float DEFAULT_MAX_VEL = 200.f; //how fast you move in a second
	const float DEFAULT_DRAG = 0.9f;
	const float MIN_VEL = 5.f;

	const float DEFAULT_ZOOM_SPEED = 0.01f;
	const float DEFAULT_ZOOMTO_SPEED = 1.f;
	const float DEFAULT_MIN_ZOOM = 1.5f;
	const float DEFAULT_MAX_ZOOM = 0.5f;

	class Camera
	{
	public:
		Camera();
		void init(int windowWidth, int windowHeight);
		void setCentre(float x, float y);
		void setCentre(const Point& p);
		void setSize(int width, int height);
		Vector2 getCentre() const;
		Vector2 getSize() const;
		SDL_Rect getBounds() const;
		SDL_Rect worldToScreen(const SDL_Rect& r) const;
		Point worldToScreen(const Point& p) const;

		SDL_Rect screenToWorld(const SDL_Rect& sr) const;
		Point screenToWorld(const Point& sp) const;

		bool intersects(const SDL_Rect& r) const; //check if world rect is within camera
		bool intersects(const Point& p) const; //check if world point is within camera

		void setZoomProps(float zoomSpeed = DEFAULT_ZOOM_SPEED, float zoomToSpeed = DEFAULT_ZOOMTO_SPEED, float minZoom = DEFAULT_MIN_ZOOM,
						  float maxZoom = DEFAULT_MAX_ZOOM);
		void setZoomMinMax(float min = -1, float max = -1); //default to unlimited
		void zoom(int zoomDir);
		void zoomTo(float target); 
		void zoomTo(const Vector2& target);
		void zoomTo(float targetX, float targetY);
		void update(float deltaTime);

		//effects
		void addShake(Shake& effect, const std::string& name = "");
		void removeShake(const std::string& name);
		void startShake(const std::string& name);
		void endShake(const std::string& name, bool remove = false);

		Shake* findShake(const std::string& name);

	private:
		void updateZoom(float deltaTime);
		void updateShake(float deltaTime);
		Vector2 lerp(const Vector2& v1, const Vector2& v2, float percent);
		float lerp(float start, float end, float percent);
		void changeBoundsZoom();
		float clampZoom(float num);

		//effects
		std::vector<Shake> m_shakeEffects;
		Shake* m_currentShake;

		SDL_Rect m_bounds;

		int m_windowWidth;
		int m_windowHeight;

		Vector2 m_centre;

		//zoom props
		Vector2 m_zoom;
		Vector2 m_zoomDirection;
		float m_zoomSpeed;
		float m_zoomToSpeed;
		float m_minZoom; //how far out we can zoom
		float m_maxZoom; //how far in to zoom

		//lerp centre
		bool m_zoomToFitActive;
		Vector2 m_zoomToFitStart;
		Vector2 m_zoomToFitTarget;
		float m_zoomToFitTime;
		float m_zoomToFitMaxTime;

		//lerp zoom
		bool m_zoomToActive;
		Vector2 m_zoomStart;
		Vector2 m_zoomTarget;
		float m_zoomToTime;
		float m_zoomToMaxTime;

		bool m_ratioResetting;
	};
}