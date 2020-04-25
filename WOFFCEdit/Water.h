#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
///#include "include\DXF.h"

class Water ///: public PlaneMesh
{
public:
	Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int size);
	~Water() {}

	// Getters
	DirectX::XMFLOAT4 GetTint() { return m_tint; }
	DirectX::XMFLOAT2 GetNormal() { return m_normal; }
	float GetScale() { return m_scale; }

	// Setters
	void SetTint(DirectX::XMFLOAT4 tint) { m_tint = tint; }
	void SetNormal(DirectX::XMFLOAT2 normal) { m_normal = normal; }
	void SetScale(float scale) { m_scale = scale; }

private:
	DirectX::XMFLOAT4 m_tint;
	DirectX::XMFLOAT2 m_normal;
	float m_scale;
};

