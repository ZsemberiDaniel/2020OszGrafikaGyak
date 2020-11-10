#include <iostream>
#include "gCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

/// <summary>
/// Initializes a new instance of the <see cref="gCamera"/> class.
/// </summary>
gCamera::gCamera(void) : m_u(0.0f), m_v(0.0f), m_speed(16.0f), m_goFw(0), m_goRight(0), m_slow(false)
{
	UpdateUV(0.0f, 0.0f);
	// m_dist = glm::length( m_at - m_eye );	

	SetProj(45.0f, 640/480.0f, 0.001f, 1000.0f);
}

gCamera::~gCamera(void)
{
}


void gCamera::SetProj(float _angle, float _aspect, float _zn, float _zf)
{
	m_matProj = glm::perspective( _angle, _aspect, _zn, _zf);
	m_matViewProj = m_matProj * m_viewMatrix;
}

glm::mat4 gCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void gCamera::Update(float _deltaTime)
{
	m_viewMatrix = glm::lookAt( m_eye, m_at, m_up);
	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::UpdateUV(float du, float dv)
{
	m_u		+= du;
	m_v		+= dv;

	m_eye = radius * glm::vec3(cosf(m_u) * sinf(m_v),
		cosf(m_v),
		sinf(m_u) * sinf(m_v));
	m_at = glm::vec3(0);

	m_fw = glm::normalize( m_at - m_eye );
	m_up = glm::normalize(radius * glm::vec3(cosf(m_u) * sinf(m_v + 0.01f), cosf(m_v + 0.01f), sinf(m_u) * sinf(m_v + 0.01f)) - m_at);
	m_st = glm::normalize( glm::cross( m_fw, m_up ) );
}

void gCamera::SetSpeed(float _val)
{
	m_speed = _val;
}

void gCamera::Resize(int _w, int _h)
{
	m_matProj = glm::perspective(	45.0f, _w/(float)_h, 0.01f, 1000.0f);

	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::KeyboardDown(SDL_KeyboardEvent& key) { }

void gCamera::KeyboardUp(SDL_KeyboardEvent& key) { }

void gCamera::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if ( mouse.state & SDL_BUTTON_LMASK )
	{
		UpdateUV(mouse.xrel/50.0f, mouse.yrel/50.0f);
	}
}
