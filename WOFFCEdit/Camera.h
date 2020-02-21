#pragma once
#include <vector>

#include "pch.h"
#include "InputCommands.h"

#define PI 3.1415

class Camera
{
public:
	Camera();
	~Camera() {}

	void Update();
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	DirectX::SimpleMath::Vector3 GetLookAt() { return m_lookAt; }
	DirectX::SimpleMath::Vector3 GetUp() { return m_up; }

	void HandleInput(InputCommands* input, float deltaTime, float centre_x, float centre_y, POINT cursorPos);
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void TrackMouse(float centre_x, float centre_y, POINT cursorPos, float deltaTime);

protected:
	// Input
	InputCommands* m_input;

	// Rotation
	float m_yaw, m_pitch, m_roll, m_speed;

	// Vectors
	DirectX::SimpleMath::Vector3 m_up, m_lookAt, m_right, m_forward, m_position;
};

