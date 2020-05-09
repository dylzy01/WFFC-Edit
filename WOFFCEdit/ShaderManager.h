#pragma once
#include "pch.h"
#include "Tools.h"
#include "DisplayObject.h"

#include "TextureShader.h"
#include "BlendShader.h"

class ShaderManager
{
public:
	ShaderManager() {}
	~ShaderManager() {}

	// Initialise all shaders
	static void Initialise(ID3D11Device * device);

	// Setup specific shader parameters
	static void Shader(SHADER_TYPE type, ID3D11DeviceContext * context, std::vector<DisplayObject> lights, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2 = NULL, DirectX::XMFLOAT2 screen = { NULL, NULL });

	// Setters
	static void SetWorld(DirectX::SimpleMath::Matrix * world);
	static void SetView(DirectX::SimpleMath::Matrix * view);
	static void SetProjection(DirectX::SimpleMath::Matrix * projection);

private:
	// Setup temp, disabled light
	static DisplayObject GetLight();
};

