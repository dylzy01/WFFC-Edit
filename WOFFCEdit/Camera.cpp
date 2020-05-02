#include "Camera.h"

//void Camera::Initialize()
//{
//	/*m_rotateFactor = 3.f;
//	m_speed = 0.3f;
//
//	m_position.x = 0.f;
//	m_position.y = 3.7f;
//	m_position.z = -3.5f;
//
//	m_orientation.x = 0.f;
//	m_orientation.y = 0.f;
//	m_orientation.z = 0.f;
//
//	m_lookAt.x = 0.f;
//	m_lookAt.y = 0.f;
//	m_lookAt.z = 0.f;
//
//	m_direction.x = 0.f;
//	m_direction.y = 0.f;
//	m_direction.z = 0.f;
//
//	m_right.x = 0.f;
//	m_right.y = 0.f;
//	m_right.z = 0.f;
//
//	m_up.x = 0.f;
//	m_up.y = 0.f;
//	m_up.z = 0.f;*/
//}

Camera::Camera()
{
	// Initialise vectors
	m_position = DirectX::SimpleMath::Vector3(0.f, 3.7f, -3.5f);
	m_forward = DirectX::SimpleMath::Vector3(0.f, 0.f, 1.f);
	m_right = DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f);
	m_up = DirectX::SimpleMath::Vector3(0.f, 1.f, 0.f);
	m_lookAt = DirectX::SimpleMath::Vector3(0.f, 0.f, -5.f);

	// Initialise rotation
	m_yaw = m_pitch = m_roll = 0.f;
	///m_speed = 0.3f;
	///m_speed = 1.f;
	m_speed = 2.f;
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

	// Forward
	m_forward.x = sinY * cosP;
	m_forward.y = sinP;
	m_forward.z = cosP * -cosY;

	// If not focussing at an object
	if (!m_focussing)
	{
		// Look At
		m_lookAt.x = m_position.x + m_forward.x;
		m_lookAt.y = m_position.y + m_forward.y;
		m_lookAt.z = m_position.z + m_forward.z;
	}

	// Else, if focussing on an object
	else
	{
		// Look At
		m_lookAt.x = m_focusObject.GetPosition().x;/// + m_forward.x;
		m_lookAt.y = m_focusObject.GetPosition().y;/// + m_forward.y;
		m_lookAt.z = m_focusObject.GetPosition().z;/// + m_forward.z;

		// Setup forward to be distance between object and camera position
		m_forward = DirectX::SimpleMath::Vector3(m_focusObject.GetPosition() - m_position);
		m_forward.Normalize();
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

	// Move up
	if (m_input->E) {
		MoveUp(deltaTime);
		Update();
	}

	// Move down
	if (m_input->Q) {
		MoveDown(deltaTime);
		Update();
	}

	// Left
	if (m_input->A) {
		MoveLeft(deltaTime);
		Update();
	}

	// Right
	if (m_input->D) {
		MoveRight(deltaTime);
		Update();
	}

	// Forward
	if (m_input->W) {
		MoveForward(deltaTime);
		Update();
	}

	// Backward
	if (m_input->S) {
		MoveBackward(deltaTime);
		Update();
	}

	// Rotation (if not focussing on an object)
	if (!m_focussing) {
		if (m_input->mouseWheel) {
			TrackMouse(centre_x, centre_y, cursorPos, deltaTime);
			Update();
		}
	}
}

void Camera::MoveUp(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera up
	temp = m_speed * m_up;
	m_position.operator+=(temp);
}

void Camera::MoveDown(float deltaTime)
{
	DirectX::SimpleMath::Vector3 temp;

	// Move camera down
	temp = -m_speed * m_up;
	m_position.operator+=(temp);
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
	m_yaw += (cursorPos.x - centreX) * (m_speed / 50.f);
	m_pitch += (centreY - cursorPos.y) * (m_speed / 50.f);
}

void Camera::TrackObject(float centreX, float centreY, DirectX::SimpleMath::Vector3 objectPosition, float deltaTime)
{
	// Alter camera rotation
	m_yaw += (objectPosition.x - centreX) * (m_speed / 50.f);
	m_pitch += (centreY - objectPosition.y) * (m_speed / 50.f);
}
