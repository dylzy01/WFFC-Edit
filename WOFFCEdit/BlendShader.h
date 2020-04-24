#pragma once
#include "pch.h"
#include "Tools.h"
#include <d3dcompiler.h>

class BlendShader
{
public:
	BlendShader() {}
	~BlendShader() {}

	// Load vertex & pixel shaders
	static bool Initialise(ID3D11Device * device);

	// Setup shader 
	static bool SetShaderParameters(ID3D11DeviceContext * context, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2);

	// Handler
	static void Enable(ID3D11DeviceContext * context);

	// Setters
	static void SetWorld(DirectX::SimpleMath::Matrix * world) { m_world = world; }
	static void SetView(DirectX::SimpleMath::Matrix * view) { m_view = view; }
	static void SetProjection(DirectX::SimpleMath::Matrix * projection) { m_projection = projection; }

private:
	// Standard matrix buffer for all shaders
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	// Buffer for single light information
	struct LightBufferType
	{
		DirectX::SimpleMath::Vector4 ambient;
		DirectX::SimpleMath::Vector4 diffuse;
		DirectX::SimpleMath::Vector3 position;
		float padding;
	};

	// Shaders
	static Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_shaderVertex;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_shaderPixel;
	static ID3D11InputLayout*							m_inputLayout;
	static ID3D11Buffer*								m_bufferMatrix;
	static ID3D11SamplerState*							m_samplerState;
	static ID3D11Buffer*								m_bufferLight;

	// Control variables
	static DirectX::SimpleMath::Matrix * m_world;
	static DirectX::SimpleMath::Matrix * m_view;
	static DirectX::SimpleMath::Matrix * m_projection;
};
