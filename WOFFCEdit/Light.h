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
	float GetConstantAttenuation() { return m_constantAttenuation; }
	float GetLinearAttenuation() { return m_linearAttenuation; }
	float GetQuadraticAttenuation() { return m_quadraticAttenuation; }
	LIGHT_TYPE GetType() { return m_type; }
	bool GetEnabled() { return m_enabled; }

	// Setters
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

