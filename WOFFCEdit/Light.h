#pragma once
#include "Tools.h"
///#include "Light.h"
///#include <directxmath.h>
using namespace DirectX;

class Light/// : public Light
{
public:
	Light(XMFLOAT4 diffuse, XMFLOAT4 ambient, XMFLOAT3 position, XMFLOAT3 direction, float constA, float linA, float quadA, LIGHT_TYPE type, bool enable)
	{
		m_diffuseColour = diffuse;
		m_ambientColour = ambient;
		m_position = position;
		m_direction = direction;
		m_constantAttenuation = constA;
		m_linearAttenuation = linA;
		m_quadraticAttenuation = quadA;
		m_type = type;
		m_enabled = enable;
	}
	~Light() {}	
	
	// Getters
	XMFLOAT4 GetDiffuseColour() { return m_diffuseColour; }
	XMFLOAT4 GetAmbientColour() { return m_ambientColour; }
	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetDirection() { return m_direction; }
	float GetConstantAttenuation() { return m_constantAttenuation; }
	float GetLinearAttenuation() { return m_linearAttenuation; }
	float GetQuadraticAttenuation() { return m_quadraticAttenuation; }
	LIGHT_TYPE GetType() { return m_type; }
	bool GetEnabled() { return m_enabled; }

	// Setters
	void SetDiffuseColour(XMFLOAT4 diffuse) { m_diffuseColour = diffuse; }
	void SetAmbientColour(XMFLOAT4 ambient) { m_ambientColour = ambient; }
	void SetPosition(XMFLOAT3 position) { m_position = position; }
	void SetDirection(XMFLOAT3 direction) { m_direction = direction; }
	void SetConstantAttenuation(float attenuation) { m_constantAttenuation = attenuation; }
	void SetLinearAttenuation(float attenuation) { m_linearAttenuation = attenuation; }
	void SetQuadraticAttenuation(float attenuation) { m_quadraticAttenuation = attenuation; }
	void SetType(LIGHT_TYPE type) { m_type = type; }
	void SetEnabled(bool enable) { m_enabled = enable; }

protected:
	XMFLOAT4 m_ambientColour, m_diffuseColour;
	XMFLOAT3 m_position, m_direction;
	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;
	LIGHT_TYPE m_type;
	bool m_enabled;
};

