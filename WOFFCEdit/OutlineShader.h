#pragma once
#include "pch.h"
#include "Tools.h"
#include <d3dcompiler.h>
#include "Shader.h"

class OutlineShader : public Shader
{
public:
	OutlineShader() {}
	~OutlineShader() {}

	// Load vertex & pixel shaders
	static bool Initialise(ID3D11Device * device);

	// Setup shader 
	static bool SetShaderParameters(ID3D11DeviceContext * context, ID3D11ShaderResourceView* texture1, DirectX::XMFLOAT2 screen);

	// Handler
	static void Enable(ID3D11DeviceContext * context);

	// Setters
	static void SetOutlineWidth(float width) { m_edgeWidth = width; }
	static void SetOutlineIntensity(float intensity) { m_edgeIntensity = intensity; }
	static void SetThresholds(float normal, float depth) { m_thresholdNormal = normal; m_thresholdDepth = depth; }
	static void SetSensitivity(float normal, float depth) { m_sensitivityNormal = normal; m_sensitityDepth = depth; }

	static void SetWorld(DirectX::SimpleMath::Matrix * world) { m_world = world; }
	static void SetView(DirectX::SimpleMath::Matrix * view) { m_view = view; }
	static void SetProjection(DirectX::SimpleMath::Matrix * projection) { m_projection = projection; }

private:
	struct ScreenBufferType
	{
		float screenWidth;
		float screenHeight;
		DirectX::XMFLOAT2 padding;
	};

	struct OutlineBufferType
	{
		float edgeWidth;
		float edgeIntensity;
		float thresholdNormal;
		float thresholdDepth;
		float sensitivityNormal;
		float sensitivityDepth;
		DirectX::XMFLOAT2 padding;
	};

	static ID3D11Buffer * m_bufferScreen, * m_bufferOutline;
	static ID3D11SamplerState *	m_sampleState;
	static Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_shaderVertex;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_shaderPixel;
	static ID3D11InputLayout *							m_inputLayout;
	static ID3D11Buffer *								m_bufferMatrix;
	static ID3D11SamplerState *							m_samplerState;
	static DirectX::SimpleMath::Matrix *				m_world;
	static DirectX::SimpleMath::Matrix *				m_view;
	static DirectX::SimpleMath::Matrix *				m_projection;

	// Outline variables
	static float m_edgeWidth, m_edgeIntensity;
	static float m_thresholdNormal, m_thresholdDepth;
	static float m_sensitivityNormal, m_sensitityDepth;
};

