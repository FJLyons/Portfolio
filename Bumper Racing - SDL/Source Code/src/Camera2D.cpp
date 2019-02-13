#include "Camera2D.h"

Camera2D::Camera::Camera()
	: m_zoom(Vector2(1.f, 1.f))
	, m_zoomSpeed(DEFAULT_ZOOM_SPEED)
	, m_zoomToSpeed(DEFAULT_ZOOMTO_SPEED)
	, m_minZoom(DEFAULT_MIN_ZOOM)
	, m_maxZoom(DEFAULT_MAX_ZOOM)
	, m_bounds({0,0,0,0})
{
}

void Camera2D::Camera::init(int windowWidth, int windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_bounds = { 0, 0, m_windowWidth, m_windowHeight };
}


void Camera2D::Camera::setCentre(float x, float y)
{
	m_centre.x = x;
	m_centre.y = y;
	m_bounds.x = x - m_bounds.w * 0.5f;
	m_bounds.y = y - m_bounds.h * 0.5f;
}

void Camera2D::Camera::setCentre(const Point& p)
{
	m_centre.x = p.x;
	m_centre.y = p.y;
	m_bounds.x = m_centre.x - m_bounds.w * 0.5f;
	m_bounds.y = m_centre.y - m_bounds.h * 0.5f;
}

void Camera2D::Camera::setSize(int width, int height)
{
	m_bounds.w = width;
	m_bounds.h = height;
	m_bounds.x = m_centre.x - m_bounds.w * 0.5f;
	m_bounds.y = m_centre.y - m_bounds.h * 0.5f;
}

Camera2D::Vector2 Camera2D::Camera::getCentre() const
{
	return m_centre;
}

Camera2D::Vector2 Camera2D::Camera::getSize() const
{
	return Vector2(m_bounds.w, m_bounds.h);
}

SDL_Rect Camera2D::Camera::getBounds() const
{
	return m_bounds;
}

SDL_Rect Camera2D::Camera::worldToScreen(const SDL_Rect& r) const
{
	SDL_Rect screenR = r;

	float xScale = (float)m_windowWidth / m_bounds.w;
	float yScale = (float)m_windowHeight / m_bounds.h;

	Point screenP(screenR.x, screenR.y);
	screenP = worldToScreen(screenP);
	screenR.x = screenP.x;
	screenR.y = screenP.y;
	screenR.w = (int)(screenR.w * xScale);
	screenR.h = (int)(screenR.h * yScale);

	return screenR;
}

Camera2D::Point Camera2D::Camera::worldToScreen(const Point & p) const
{
	Point screenP = p;

	float xScale = (float)m_windowWidth / m_bounds.w;
	float yScale = (float)m_windowHeight / m_bounds.h;

	if (m_currentShake != nullptr && m_currentShake->getEnabled()) //if shaking the add shake offset
	{
		screenP.x -= (m_bounds.x + m_currentShake->getShakeOffset().x);
		screenP.y -= (m_bounds.y + m_currentShake->getShakeOffset().y);
	}
	else
	{
		screenP.x -= m_bounds.x;
		screenP.y -= m_bounds.y;
	}
	screenP.x *= xScale;
	screenP.y *= yScale;

	return screenP;
}

SDL_Rect Camera2D::Camera::screenToWorld(const SDL_Rect& sr) const
{
	float xScale = (float)m_bounds.w / m_windowWidth;
	float yScale = (float)m_bounds.h / m_windowHeight;
	SDL_Rect r = sr;

	Point p(r.x, r.y);
	p = screenToWorld(p);
	r.x = p.x;
	r.y = p.y;
	r.w = (int)(r.w / xScale);
	r.h = (int)(r.h / yScale);

	return r;
}

Camera2D::Point Camera2D::Camera::screenToWorld(const Point& sp) const
{
	float xScale = (float)m_bounds.w / m_windowWidth;
	float yScale = (float)m_bounds.h / m_windowHeight;
	Point p = sp;

	p.x *= xScale;
	p.y *= yScale;
	if (m_currentShake != nullptr && m_currentShake->getEnabled()) //if shaking the add shake offset
	{
		p.x += (m_bounds.x + m_currentShake->getShakeOffset().x);
		p.y += (m_bounds.y + m_currentShake->getShakeOffset().y);
	}
	else
	{
		p.x += m_bounds.x;
		p.y += m_bounds.y;
	}

	return p;
}

void Camera2D::Camera::setZoomProps(float zoomSpeed, float zoomToSpeed, float minZoom, float maxZoom)
{
	m_zoomSpeed = zoomSpeed;
	m_zoomToSpeed = zoomToSpeed;
	m_minZoom = minZoom;
	m_maxZoom = maxZoom;

	m_zoomTarget.x = clampZoom(m_zoom.x);
	m_zoomTarget.y = clampZoom(m_zoom.y);
	zoomTo(m_zoomTarget);
}

void Camera2D::Camera::setZoomMinMax(float min, float max)
{
	m_minZoom = min;
	m_maxZoom = max;

	m_zoomTarget.x = clampZoom(m_zoom.x);
	m_zoomTarget.y = clampZoom(m_zoom.y);
	zoomTo(m_zoomTarget);
}

void Camera2D::Camera::zoom(int dir)
{
	if (m_ratioResetting)
		return;
	m_zoomToActive = false;
	m_zoomDirection.x = dir;
	m_zoomDirection.y = dir;
	m_zoom += m_zoomDirection * m_zoomSpeed;

	m_zoom.x = clampZoom(m_zoom.x);
	m_zoom.y = clampZoom(m_zoom.y);

	changeBoundsZoom();
}

void Camera2D::Camera::zoomTo(const Vector2& target)
{
	zoomTo(target.x, target.y);
}

void Camera2D::Camera::zoomTo(float targetX, float targetY) 
{
	targetX = clampZoom(targetX);
	targetY = clampZoom(targetY);

	m_zoomTarget.x = targetX; 
	m_zoomTarget.y = targetY;

	m_zoomStart = m_zoom;

	m_zoomToActive = true;
	m_zoomToTime = 0.f;
	m_zoomToMaxTime = ((m_zoomTarget - m_zoomStart).length()) / m_zoomToSpeed;
}

void Camera2D::Camera::zoomTo(float target)
{
	target = (target <= 0.f) ? 0.1f : target;

	if (target < m_maxZoom)
	{
		target = m_maxZoom;
	}
	else if (target > m_minZoom && m_minZoom != -1)
	{
		target = m_minZoom;
	}
	m_zoomTarget.x = target; 
	m_zoomTarget.y = target;

	m_zoomStart = m_zoom;

	m_zoomToActive = true;
	m_zoomToTime = 0.f;
	m_zoomToMaxTime = ((m_zoomTarget - m_zoomStart).length()) / m_zoomToSpeed;
}

void Camera2D::Camera::update(float deltaTime)
{
	updateZoom(deltaTime);
	updateShake(deltaTime);
}

void Camera2D::Camera::startShake(const std::string & name)
{
	endShake(name);

	for (int i = 0; i < m_shakeEffects.size(); i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			m_currentShake = &m_shakeEffects[i];
			m_currentShake->setEnabled(true);
			break;
		}
	}
}

void Camera2D::Camera::endShake(const std::string & name, bool remove)
{
	if (m_currentShake != nullptr)
	{
		for (int i = 0; i < m_shakeEffects.size(); i++)
		{
			if (m_shakeEffects[i].getName() == name)
			{
				m_currentShake->setEnabled(false);
				if (remove)
					removeShake(m_currentShake->getName());
				m_currentShake = nullptr;
				break;
			}
		}
	}
}

Camera2D::Shake* Camera2D::Camera::findShake(const std::string& name)
{
	for (int i = 0; i < m_shakeEffects.size(); i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			return &m_shakeEffects[i];
		}
	}

	return nullptr;
}

void Camera2D::Camera::removeShake(const std::string & name)
{
	endShake(name);

	for (int i = 0; i < m_shakeEffects.size(); i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			m_shakeEffects.erase(m_shakeEffects.begin() + i);
			break;
		}
	}
} 

void Camera2D::Camera::updateZoom(float deltaTime)
{ 
	if (m_zoomToActive)
	{
		if (m_zoomToMaxTime == 0.f)
		{
			m_zoom = m_zoomTarget;
			m_zoomToActive = false;
			m_ratioResetting = false;
			return;
		}
		float percent = m_zoomToTime / m_zoomToMaxTime;
		if (percent > 1.f)
		{
			m_zoom = m_zoomTarget;
			m_zoomToActive = false;
			m_ratioResetting = false;
		}
		else
		{
			m_zoom = lerp(m_zoomStart, m_zoomTarget, percent);
			m_zoomToTime += deltaTime;
		}
		m_zoom.x = roundf(m_zoom.x * 1000) / 1000;
		m_zoom.y = roundf(m_zoom.y * 1000) / 1000;
		changeBoundsZoom();
	}
}

void Camera2D::Camera::updateShake(float deltaTime)
{
	if (m_currentShake != nullptr)
	{
		m_currentShake->update(deltaTime);
		if (m_currentShake->getEnabled() == false)
		{
			m_currentShake = nullptr;
		}
	}
}

Camera2D::Vector2 Camera2D::Camera::lerp(const Vector2 & v1, const Vector2 & v2, float percent)
{
	if (percent > 1.f)
		percent = 1.f;
	else if (percent < 0.f)
		percent = 0.f;
	Vector2 v = v2 - v1;
	float distanceAlong = v.length() * percent;
	
	v = v1 + (v.normalize() * distanceAlong);
	return v;
}

float Camera2D::Camera::lerp(float start, float end, float percent)
{
	if (percent > 1.f)
		percent = 1.f;
	else if (percent < 0.f)
		percent = 0.f;
	return start + ((end - start) * percent);
}

void Camera2D::Camera::changeBoundsZoom()
{
	m_bounds.w = (int)m_windowWidth * m_zoom.x;
	m_bounds.h = (int)m_windowHeight * m_zoom.y;
}

float Camera2D::Camera::clampZoom(float num)
{
	if (num < m_maxZoom)
	{
		num = m_maxZoom;
	}
	else if (num > m_minZoom  && m_minZoom != -1)
	{
		num = m_minZoom;
	}
	if (num < m_zoomSpeed)
	{
		num = m_zoomSpeed;
	}
	return num;
}

void Camera2D::Camera::addShake(Shake& effect, const std::string & name)
{
	if (effect.getName() == "") //name hasnt been set yet
		effect.setName(name);
	m_shakeEffects.push_back((static_cast<Shake&>(effect)));
	m_shakeEffects.back().init();
}

bool Camera2D::Camera::intersects(const SDL_Rect & r) const
{
	return (r.x < m_bounds.x + m_bounds.w && r.x + r.w > m_bounds.x &&
			r.y < m_bounds.y + m_bounds.h && r.y + r.h > m_bounds.y);
}

bool Camera2D::Camera::intersects(const Point & p) const
{
	return (p.x > m_bounds.x && p.x < m_bounds.x + m_bounds.w &&
			p.y > m_bounds.y && p.y < m_bounds.y + m_bounds.y);;
}
