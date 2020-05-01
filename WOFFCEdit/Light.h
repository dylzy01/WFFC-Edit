#pragma once
#include "Tools.h"
#include "SceneObject.h"
///#include "Light.h"
///#include <directxmath.h>
using namespace DirectX;

class Light/// : public DisplayObject
{
public:
	Light(XMFLOAT4 diffuse, XMFLOAT4 ambient, XMFLOAT3 position, XMFLOAT3 direction, float constA, float linA, float quadA, LIGHT_TYPE type, bool enable)
	{
		m_diffuse = diffuse;
		m_ambient = ambient;
		m_position = position;
		m_direction = direction;
		m_constA = constA;
		m_linA = linA;
		m_quadA = quadA;
		m_type = type;
		m_enabled = enable;
	}
	~Light() {}	
	
	// Getters
	XMFLOAT4 GetDiffuse() { return m_diffuse; }
	XMFLOAT4 GetAmbient() { return m_ambient; }
	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetDirection() { return m_direction; }
	float GetConstantAttenuation() { return m_constA; }
	float GetLinearAttenuation() { return m_linA; }
	float GetQuadraticAttenuation() { return m_quadA; }
	LIGHT_TYPE GetLightType() { return m_type; }
	bool GetEnabled() { return m_enabled; }

	// Setters
	void SetDiffuse(XMFLOAT4 diffuse) { m_diffuse = diffuse; }
	void SetAmbient(XMFLOAT4 ambient) { m_ambient = ambient; }
	void SetPosition(XMFLOAT3 position) { m_position = position; }	
	void SetDirection(XMFLOAT3 direction) { m_direction = direction; }
	void SetConstantAttenuation(float attenuation) { m_constA = attenuation; }
	void SetLinearAttenuation(float attenuation) { m_linA = attenuation; }
	void SetQuadraticAttenuation(float attenuation) { m_quadA = attenuation; }
	void SetLightType(LIGHT_TYPE type) { m_type = type; }
	void SetEnabled(bool enable) { m_enabled = enable; }

	void SetDiffuseR(float r) { m_diffuse.x = r; }
	void SetDiffuseG(float g) { m_diffuse.y = g; }
	void SetDiffuseB(float b) { m_diffuse.z = b; }
	void SetAmbientR(float r) { m_ambient.x = r; }
	void SetAmbientG(float g) { m_ambient.y = g; }
	void SetAmbientB(float b) { m_ambient.z = b; }

	void SetPositionX(float x) { m_position.x = x; }
	void SetPositionY(float y) { m_position.y = y; }
	void SetPositionZ(float z) { m_position.z = z; };
	void SetDirectionX(float x) { m_direction.x = x; }
	void SetDirectionY(float y) { m_direction.y = y; }
	void SetDirectionZ(float z) { m_direction.z = z; }

protected:
	XMFLOAT4 m_ambient, m_diffuse;
	XMFLOAT3 m_position, m_direction;
	float m_constA;
	float m_linA;
	float m_quadA;
	LIGHT_TYPE m_type;
	bool m_enabled;
};

