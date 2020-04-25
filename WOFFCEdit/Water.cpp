#include "Water.h"


Water::Water(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int size)
{
	// Normal map tiling
	m_normal.x = size / 1000.f;
	m_normal.y = size / 70.f;

	// Scale value
	m_scale = 0.1f;

	// Tint
	m_tint = { 0.1f, 0.3f, 1.f, 1.f };

	initBuffers(device);
}
