#include "Camera.h"

Camera::Camera()
{
	// Initialise vectors
	m_position = DirectX::SimpleMath::Vector3(9.38f, 27.83f, 83.f);
	m_forward = DirectX::SimpleMath::Vector3(0.f, 0.f, 1.f);
	m_right = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
	m_up = DirectX::SimpleMath::Vector3(0.f, 1.f, 0.f);
	m_lookAt = DirectX::SimpleMath::Vector3(0.f, 0.f, -5.f);

	// Initialise rotation
	m_yaw = m_pitch = m_roll = 0.f;
	m_speed = 2.f;
	m_track = 0.04f;
}

void Camera::Update()
{
	// Cos/Sin temps
	float cosY, cosP, cosR;
	float sinY, sinP, sinR;
	cosY = cosf(m_yaw * PI / 180.f);
	cosP = cosf(m_pitch * PI / 180.f);
	cosR = cosf(m_roll * PI / 180.f);
	sinY = sinf(m_yaw * PI / 180.f);
	sinP = sinf(m_pitch * PI / 180.f);
	sinR = sinf(m_roll * PI / 180.f);
	
	// If not focussing at an object
	if (!m_focussing)
	{
		// Forward
		m_forward.x = sinY * cosP;
		m_forward.y = sinP;
		m_forward.z = cosP * -cosY;
		
		// Look At
		m_lookAt.x = m_position.x + m_forward.x;
		m_lookAt.y = m_position.y + m_forward.y;
		m_lookAt.z = m_position.z + m_forward.z;
	}

	// Else, if focussing on an object
	else
	{
		// Forward
		m_forward = DirectX::SimpleMath::Vector3(
			m_focusObject.GetPosition() - m_position);
		m_forward.Normalize();

		// Look At
		m_lookAt.x = m_focusObject.GetPosition().x;
		m_lookAt.y = m_focusObject.GetPosition().y;
		m_lookAt.z = m_focusObject.GetPosition().z;	
	}

	// Up
	m_up.x = -cosY * sinR - sinY * sinP * cosR;
	m_up.y = cosP * cosR;
	m_up.z = -sinY * sinR - sinP * cosR * -cosY;

	// Right
	m_right = m_forward.Cross(m_up);
}

void Camera::HandleInput(InputCommands * input, float deltaTime, float centre_x, float centre_y, POINT cursorPos)
{
	m_input = input;
	m_deltaTime = deltaTime;
	m_centreX = centre_x;
	m_centreY = centre_y;
	
	// If CTRL isn't pressed
	if (!m_input->CTRL) 
	{
		// Move up
		if (m_input->E) {
			MoveUp(deltaTime);
		}

		// Move down
		if (m_input->Q) {
			MoveDown(deltaTime);
		}

		// Left
		if (m_input->A) {
			MoveLeft(deltaTime);
		}

		// Right
		if (m_input->D) {
			MoveRight(deltaTime);
		}

		// Forward
		if (m_input->W) {
			MoveForward(deltaTime);
		}

		// Backward
		if (m_input->S) {
			MoveBackward(deltaTime);
		}

		// Rotation (if not focussing on an object)
		if (!m_focussing) {
			if (m_input->mouseWheel) {
				TrackMouse(centre_x, centre_y, cursorPos, deltaTime);
			}
		}
	}
}

void Camera::MoveUp(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera up
	temp = m_speed * m_up;

	// If not focussing
	if (!m_focussing) { m_position.operator+=(temp); }

	// Else, if focussing
	else { m_position.y += temp.y; }
	
}

void Camera::MoveDown(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera down
	temp = -m_speed * m_up;

	// If not focussing
	if (!m_focussing) { m_position.operator+=(temp); }

	// Else, if focussing
	else { m_position.y += temp.y; }
}

void Camera::MoveLeft(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera left
	temp = -m_speed * m_right;
	
	// If not focussing
	if (!m_focussing) { m_position.operator+=(temp); }

	// Else, if focussing
	else {
		m_position.x += temp.x;
		m_position.z += temp.z;
	}
}

void Camera::MoveRight(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera right
	temp = m_speed * m_right;
	
	// If not focussing
	if (!m_focussing) { m_position.operator+=(temp); }

	// Else, if focussing
	else {
		m_position.x += temp.x;
		m_position.z += temp.z;
	}
}

void Camera::MoveForward(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera forward
	temp = m_speed * m_forward;
	m_position.operator+=(temp);
}

void Camera::MoveBackward(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera backward
	temp = -m_speed * m_forward;
	m_position.operator+=(temp);
}

void Camera::TrackMouse(float centreX, float centreY, POINT cursorPos, float deltaTime)
{
	// Alter camera rotation
	m_yaw += (cursorPos.x - centreX) * m_track;
	m_pitch += (centreY - cursorPos.y) * m_track;
}

void Camera::TrackObject(float centreX, float centreY, DirectX::SimpleMath::Vector3 objectPosition, float deltaTime)
{
	// Alter camera rotation
	m_yaw += (objectPosition.x - centreX) * (m_speed / 50.f);
	m_pitch += (centreY - objectPosition.y) * (m_speed / 50.f);
}