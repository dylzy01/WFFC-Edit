#pragma once
#include "pch.h"
#include "Tools.h"
#include <d3dcompiler.h>
#include "Shader.h"
#include "Light.h"

class WaterShader : public Shader
{
public:
	WaterShader() {}
	~WaterShader() {}

	// Load vertex & pixel shaders
	static bool Initialise(ID3D11Device * device);

	// Setup shader 
	static bool SetShaderParameters(ID3D11DeviceContext * context, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, ID3D11ShaderResourceView* textureNormal,
		XMFLOAT3 camPosition, XMFLOAT2 repeatNormal, float translation, float scale, XMFLOAT4 tint);

	// Handler
	static void Enable(ID3D11DeviceContext * context);

	// Setters
	static void SetWorld(DirectX::SimpleMath::Matrix * world) { m_world = world; }
	static void SetView(DirectX::SimpleMath::Matrix * view) { m_view = view; }
	static void SetProjection(DirectX::SimpleMath::Matrix * projection) { m_projection = projection; }

private:
	// Water buffer for pixel shader
	struct WaterBufferTypePS
	{
		DirectX::XMFLOAT4 tint;
		float translation;
		float scale;
		DirectX::XMFLOAT2 padding;
	};

	// Water buffer for vertex shader
	struct WaterBufferTypeVS
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 normal;
		DirectX::XMFLOAT3 padding;
	};

	static ID3D11Buffer *								m_bufferMatrix;
	static ID3D11Buffer *								m_bufferWaterVS;
	static ID3D11Buffer *								m_bufferWaterPS;
	static ID3D11SamplerState *							m_samplerState;
	static Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_shaderVertex;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_shaderPixel;
	static ID3D11InputLayout *							m_inputLayout;

	static DirectX::SimpleMath::Matrix *				m_world;
	static DirectX::SimpleMath::Matrix *				m_view;
	static DirectX::SimpleMath::Matrix *				m_projection;
	static DirectX::SimpleMath::Matrix *				m_reflection;
};