#include "Camera.h"

Camera::Camera()
{
	m_rotateFactor = 5.f;
	m_position = DirectX::SimpleMath::Vector3(0.0f, 51.0f, 0.0f);	m_lookAt = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);	m_up = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	m_forward = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
	m_rotation = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
}

void Camera::Update(float deltaTime)
{
	// Temp values for sin/cos
	float cosPitch, cosYaw, cosRoll;
	float sinPitch, sinYaw, sinRoll;
	float PI = 3.1415;

	// Calculate values when rotation changes (function called when rotation changes)
	cosPitch	= cosf(m_pitch*PI / 180);
	cosYaw		= cosf(m_yaw*PI / 180);
	cosRoll		= cosf(m_roll*PI / 180);
	sinPitch	= sinf(m_pitch*PI / 180);
	sinYaw		= sinf(m_yaw*PI / 180);
	sinRoll		= sinf(m_roll*PI / 180);

	// Calculate forward vector
	m_forward.x = sinYaw * cosPitch;
	m_forward.y = sinPitch;
	m_forward.z = cosPitch * -cosYaw;

	// Calculate look at vector
	m_lookAt = m_position + m_forward;

	// Calculate up vector
	m_up.x = (-cosYaw * sinRoll) - (sinYaw * sinPitch * cosRoll);
	m_up.y = cosPitch * cosRoll;
	m_up.z = (-sinYaw * sinRoll) - (sinPitch * cosRoll * -cosYaw);

	// Calculate right vector
	m_right = m_forward.Cross(m_up);
}

void Camera::RotateX(float rate)
{
	/*if (positive) { m_pitch += (m_rotateFactor * rate); }
	else { m_pitch -= (m_rotateFactor * rate); }*/

	m_pitch += m_rotateFactor * rate;
}

void Camera::RotateY(float rate)
{
	/*if (positive) { m_yaw += (m_rotateFactor * rate); }
	else { m_yaw -= (m_rotateFactor * rate); }*/

	m_yaw += m_rotateFactor * rate;
}

void Camera::RotateZ(float rate)
{
	/*if (positive) { m_roll += (m_rotateFactor * rate); }
	else { m_roll -= (m_rotateFactor * rate); }*/

	m_roll += m_rotateFactor * rate;
}
