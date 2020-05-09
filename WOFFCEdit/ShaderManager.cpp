#include "ShaderManager.h"

// Initialise all shaders
void ShaderManager::Initialise(ID3D11Device * device)
{
	// Initialise texture shader
	TextureShader::Initialise(device);

	// Initialise blend shader
	BlendShader::Initialise(device);

	// Initialise toon shader
	ToonShader::Initialise(device);

	// Initialise toon blend shader
	ToonBlendShader::Initialise(device);
}

// Setup specific shader parameters
void ShaderManager::Shader(bool isNormal, bool blend, ID3D11DeviceContext * context, std::vector<DisplayObject> lights, ID3D11ShaderResourceView * texture1, ID3D11ShaderResourceView * texture2, DirectX::XMFLOAT2 screen)
{	
	// Setup temp light
	std::vector<DisplayObject> temp;
	temp.push_back(GetLight());
	
	// If should blend
	if (blend)
	{
		// Handle texture shader
		TextureShader::Enable(context);

		// If light vector is empty
		if (lights.size() == 0)
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(isNormal, context, texture1, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(isNormal, context, texture1, lights);
		}
	}

	// Else, if should not blend
	else
	{
		// Handle blend shader
		BlendShader::Enable(context);

		// If light vector is empty
		if (lights.size() == 0)
		{
			// Setup texture parameters
			BlendShader::SetShaderParameters(context, texture1, texture2, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup texture parameters
			BlendShader::SetShaderParameters(context, texture1, texture2, lights);
		}
	}
}

// Set shader world matrices
void ShaderManager::SetWorld(DirectX::SimpleMath::Matrix * world)
{
	TextureShader::SetWorld(world);
	BlendShader::SetWorld(world);
	ToonShader::SetWorld(world);
	ToonBlendShader::SetWorld(world);
}

// Set shader view matrices
void ShaderManager::SetView(DirectX::SimpleMath::Matrix * view)
{
	TextureShader::SetView(view);
	BlendShader::SetView(view);
	ToonShader::SetView(view);
	ToonBlendShader::SetView(view);
}

// Set shader projection matrices
void ShaderManager::SetProjection(DirectX::SimpleMath::Matrix * projection)
{
	TextureShader::SetProjection(projection);
	BlendShader::SetProjection(projection);
	ToonShader::SetProjection(projection);
	ToonBlendShader::SetProjection(projection);
}

// Setup temp, disabled light
DisplayObject ShaderManager::GetLight()
{
	// Setup details
	DirectX::XMFLOAT3 diffuse = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 ambient = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 position = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 direction = { 0.f, 0.f, 0.f };
	float constA = 0.f, linA = 0.f, quadA = 0.f;
	LIGHT_TYPE type = LIGHT_TYPE::NA;
	bool enable = false;

	// Return temp light
	return DisplayObject(diffuse, ambient, position, direction, constA, linA, quadA, type, enable);
}