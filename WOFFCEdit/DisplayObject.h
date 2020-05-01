#pragma once
#include "pch.h"
#include "Tools.h"

class DisplayObject
{
public:
	DisplayObject();
	DisplayObject(DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction, float constA, float linA, float quadA, LIGHT_TYPE type, bool enable)
	{
		m_light_diffuse_r = diffuse.x;
		m_light_diffuse_g = diffuse.y;
		m_light_diffuse_b = diffuse.z;
		m_ambient = ambient;
		m_position = position;
		m_orientation = direction;
		m_light_constant = constA;
		m_light_linear = linA;
		m_light_quadratic = quadA;
		m_light_type = (int)type;
		m_enabled = enable;
	}
	~DisplayObject();

	std::shared_ptr<DirectX::Model>						m_model;							//main Mesh
	ID3D11ShaderResourceView *							m_texture_diffuse;					//diffuse texture
	ID3D11ShaderResourceView *							m_texture_highlight;				//highlight texture

	bool												m_isWater;
	OBJECT_TYPE											m_objectType;
	int													m_ID;
	DirectX::SimpleMath::Vector3						m_position;
	DirectX::SimpleMath::Vector3						m_orientation;
	DirectX::SimpleMath::Vector3						m_scale;
	bool												m_render;
	bool												m_wireframe;

	int													m_light_type;
	float												m_light_diffuse_r,	m_light_diffuse_g,	m_light_diffuse_b;
	float												m_light_specular_r, m_light_specular_g, m_light_specular_b;
	float												m_light_spot_cutoff;
	float												m_light_constant;
	float												m_light_linear;
	float												m_light_quadratic;

	// Light extension //////////////////////////////////////////////////////////////////

	// Getters
	DirectX::XMFLOAT4 GetDiffuse() { return DirectX::XMFLOAT4{ m_light_diffuse_r, m_light_diffuse_g, m_light_diffuse_b, 1.f }; }
	DirectX::XMFLOAT4 GetAmbient() { return m_ambient; }
	DirectX::XMFLOAT3 GetPosition() { return m_position; }
	DirectX::XMFLOAT3 GetDirection() { return m_orientation; }
	float GetConstantAttenuation() { return m_light_constant; }
	float GetLinearAttenuation() { return m_light_linear; }
	float GetQuadraticAttenuation() { return m_light_quadratic; }
	LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_light_type; }
	bool GetEnabled() { return m_enabled; }

	// Setters
	void SetDiffuse(DirectX::XMFLOAT4 diffuse) {
		m_light_diffuse_r = diffuse.x;
		m_light_diffuse_g = diffuse.y;
		m_light_diffuse_b = diffuse.z;
	}
	void SetAmbient(DirectX::XMFLOAT4 ambient) { m_ambient = ambient; }
	void SetPosition(DirectX::XMFLOAT3 position) { m_position = position; }
	void SetDirection(DirectX::XMFLOAT3 direction) { m_orientation = direction; }
	void SetConstantAttenuation(float attenuation) { m_light_constant = attenuation; }
	void SetLinearAttenuation(float attenuation) { m_light_linear = attenuation; }
	void SetQuadraticAttenuation(float attenuation) { m_light_quadratic = attenuation; }
	void SetLightType(LIGHT_TYPE type) { m_light_type = (int)type; }
	void SetEnabled(bool enable) { m_enabled = enable; }

	void SetDiffuseR(float r) { m_light_diffuse_r = r; }
	void SetDiffuseG(float g) { m_light_diffuse_g = g; }
	void SetDiffuseB(float b) { m_light_diffuse_b = b; }
	void SetAmbientR(float r) { m_ambient.x = r; }
	void SetAmbientG(float g) { m_ambient.y = g; }
	void SetAmbientB(float b) { m_ambient.z = b; }

	void SetPositionX(float x) { m_position.x = x; }
	void SetPositionY(float y) { m_position.y = y; }
	void SetPositionZ(float z) { m_position.z = z; };
	void SetDirectionX(float x) { m_orientation.x = x; }
	void SetDirectionY(float y) { m_orientation.y = y; }
	void SetDirectionZ(float z) { m_orientation.z = z; }

protected:
	DirectX::XMFLOAT4 m_ambient;
	bool m_enabled;
};