#pragma once
#include <vector>

#include "pch.h"
#include "InputCommands.h"

#define PI 3.1415

class Camera
{
public:
	Camera() {}
	~Camera() {}

	void Initialize();
	void Update();
	void HandleInput(InputCommands* input);

	// Getters
	float GetPitch()										{ return m_pitch; }
	float GetYaw()											{ return m_yaw; }
	float GetRoll()											{ return m_roll; }
	DirectX::SimpleMath::Vector3 GetPosition()				{ return m_position; }
	DirectX::SimpleMath::Vector3 GetForward()				{ return m_forward; }
	DirectX::SimpleMath::Vector3 GetUp()					{ return m_up; }
	DirectX::SimpleMath::Vector3 GetRight()					{ return m_right; }
	DirectX::SimpleMath::Vector3 GetRotation()				{ return m_rotation; }
	DirectX::SimpleMath::Vector3 GetLookAt()				{ return m_lookAt; }
	DirectX::SimpleMath::Vector3 GetDirection()				{ return m_direction; }
	DirectX::SimpleMath::Vector3 GetOrientation()			{ return m_orientation; }
	DirectX::SimpleMath::Matrix GetViewMatrix()				{ return m_viewMatrix; }

	// Setters
	void SetPitch(float x)									{ m_pitch = x; }
	void SetYaw(float y)									{ m_yaw = y; }
	void SetRoll(float z)									{ m_roll = z; }
	void SetRotateFactor(float factor)						{ m_rotateFactor = factor; }
	void SetPositionX(float x)								{ m_position.x = x; }
	void SetPositionY(float y)								{ m_position.y = y; }
	void SetPositionZ(float z)								{ m_position.z = z; }
	void SetPosition(DirectX::SimpleMath::Vector3 pos)		{ m_position = pos; }
	void ApplyPosition(DirectX::SimpleMath::Vector3 pos)	{ m_position += pos; }
	void SetOrientation(DirectX::SimpleMath::Vector3 o)		{ m_orientation = o; }

	// Movement
	void MoveForward()										{ m_position += m_forward; }
	void MoveBack()											{ m_position -= m_forward; }
	void MoveRight()										{ m_position += m_right; }
	void MoveLeft()											{ m_position -= m_right; }
	void MoveUp()											{ m_position += m_up; }
	void MoveDown()											{ m_position -= m_up; }
	void RotateX(float rate);
	void RotateY(float rate);
	void RotateZ(float rate);

protected:
	InputCommands* m_input;

	float m_pitch, m_yaw, m_roll, m_rotateFactor, m_speed;
	DirectX::SimpleMath::Vector3 m_position, m_forward, m_up, m_right, m_rotation, m_lookAt, m_direction;
	DirectX::SimpleMath::Matrix m_viewMatrix, m_orthoMatrix;

	DirectX::SimpleMath::Vector3 m_orientation;
};

