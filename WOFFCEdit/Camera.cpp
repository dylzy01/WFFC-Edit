#include "Camera.h"

void Camera::Initialize()
{
	m_rotateFactor = 3.f;
	m_speed = 0.3f;

	m_position.x = 0.f;
	m_position.y = 3.7f;
	m_position.z = -3.5f;

	m_orientation.x = 0.f;
	m_orientation.y = 0.f;
	m_orientation.z = 0.f;

	m_lookAt.x = 0.f;
	m_lookAt.y = 0.f;
	m_lookAt.z = 0.f;

	m_direction.x = 0.f;
	m_direction.y = 0.f;
	m_direction.z = 0.f;

	m_right.x = 0.f;
	m_right.y = 0.f;
	m_right.z = 0.f;

	m_up.x = 0.f;
	m_up.y = 0.f;
	m_up.z = 0.f;
}

void Camera::Update()
{
	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_lookAt,
		DirectX::SimpleMath::Vector3::UnitY);
	
	// Temp values for sin/cos
	///float cosPitch, cosYaw, cosRoll;
	///float sinPitch, sinYaw, sinRoll;

	// Calculate values when rotation changes (function called when rotation changes)
	///cosPitch	= cosf(m_pitch*PI / 180);
	///cosYaw		= cosf(m_yaw*PI / 180);
	///cosRoll		= cosf(m_roll*PI / 180);
	///sinPitch	= sinf(m_pitch*PI / 180);
	///sinYaw		= sinf(m_yaw*PI / 180);
	///sinRoll		= sinf(m_roll*PI / 180);

	// Calculate forward vector
	///m_forward.x = sinYaw * cosPitch;
	///m_forward.y = sinPitch;
	///m_forward.z = cosPitch * -cosYaw;

	// Calculate look at vector
	///m_lookAt = m_position + m_forward;

	// Calculate up vector
	///m_up.x = (-cosYaw * sinRoll) - (sinYaw * sinPitch * cosRoll);
	///m_up.y = cosPitch * cosRoll;
	///m_up.z = (-sinYaw * sinRoll) - (sinPitch * cosRoll * -cosYaw);

	// Calculate right vector
	///m_right = m_forward.Cross(m_up);
}

void Camera::HandleInput(InputCommands * input)
{
	m_input = input;

	// Hold right mouse to use AD for rotation
	if (m_input->mouseRight)
	{
		if (m_input->D) { m_orientation.y -= m_rotateFactor; }
		if (m_input->A) { m_orientation.y += m_rotateFactor; }
	}

	// Create m_direction from Eular angles in m_orientation
	m_direction.x = sin((m_orientation.y) * PI / 180);
	m_direction.y = cos((m_orientation.z) * PI / 180);
	m_direction.Normalize();

	// Create right vector from m_direction
	m_direction.Cross(DirectX::SimpleMath::Vector3::UnitY, m_right);

	// Movement
	if (m_input->W) { m_position += m_lookAt * m_speed; }
	if (m_input->S) { m_position -= m_lookAt * m_speed; }
	if (!m_input->mouseRight)
	{
		if (m_input->D) { m_position += m_right * m_speed; }
		if (m_input->A) { m_position -= m_right * m_speed; }
	}
	if (m_input->E) { m_position += m_up * m_speed; }
	if (m_input->Q) { m_position -= m_up * m_speed; }

	// Update m_lookAt
	m_lookAt = m_position + m_direction;
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
