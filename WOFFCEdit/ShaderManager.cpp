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

	// Initialise outline shader
	OutlineShader::Initialise(device);

	// Initialise water shader
	WaterShader::Initialise(device);
}

// Setup specific shader parameters
void ShaderManager::Shader(SHADER_TYPE type, ID3D11DeviceContext * context, std::vector<Light*> light, ID3D11ShaderResourceView * texture1, ID3D11ShaderResourceView * texture2, DirectX::XMFLOAT2 screen)
{	
	// Setup temp light
	std::vector<Light*> temp;
	temp.push_back(GetLight());
	
	// Switch between shader type
	switch (type)
	{
	case SHADER_TYPE::TEXTURE:
	{
		// Handle texture shader
		TextureShader::Enable(context);
		
		// If light vector is empty
		if (light.size() == 0)
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(context, texture1, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(context, texture1, light);
		}		
	}
	break;
	case SHADER_TYPE::BLEND:
	{
		// Handle blend shader
		BlendShader::Enable(context);
		
		// Setup blend parameters
		BlendShader::SetShaderParameters(context, texture1, texture2);
	}
	break;
	case SHADER_TYPE::TOON_SINGLE:
	{
		// Handle toon shader
		ToonShader::Enable(context);

		// If light vector is empty
		if (light.size() == 0)
		{
			// Setup toon parameters
			ToonShader::SetShaderParameters(context, texture1, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup toon parameters
			ToonShader::SetShaderParameters(context, texture1, light);
		}
	}
	break;
	case SHADER_TYPE::TOON_BLEND:
	{
		// Handle toon shader
		ToonBlendShader::Enable(context);

		// If light vector is empty
		if (light.size() == 0)
		{
			// Setup toon blend parameters
			ToonBlendShader::SetShaderParameters(context, texture1, texture2, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup toon blend parameters
			ToonBlendShader::SetShaderParameters(context, texture1, texture2, light);
		}
	}
	break;
	case SHADER_TYPE::OUTLINE:
	{
		// Handle outline shader
		OutlineShader::Enable(context);

		// Setup outline parameters
		OutlineShader::SetShaderParameters(context, texture1, screen);
	}
	break;
	case SHADER_TYPE::WATER:
	{
		// Handle water shader
		WaterShader::Enable(context);

		// Setup water parameters
		///WaterShader::SetShaderParameters(context, texture1);
	}
	break;
	}
}

// Set shader world matrices
void ShaderManager::SetWorld(DirectX::SimpleMath::Matrix * world)
{
	TextureShader::SetWorld(world);
	BlendShader::SetWorld(world);
	ToonShader::SetWorld(world);
	ToonBlendShader::SetWorld(world);
	OutlineShader::SetWorld(world);
	WaterShader::SetWorld(world);
}

// Set shader view matrices
void ShaderManager::SetView(DirectX::SimpleMath::Matrix * view)
{
	TextureShader::SetView(view);
	BlendShader::SetView(view);
	ToonShader::SetView(view);
	ToonBlendShader::SetView(view);
	OutlineShader::SetView(view);
	WaterShader::SetView(view);
}

// Set shader projection matrices
void ShaderManager::SetProjection(DirectX::SimpleMath::Matrix * projection)
{
	TextureShader::SetProjection(projection);
	BlendShader::SetProjection(projection);
	ToonShader::SetProjection(projection);
	ToonBlendShader::SetProjection(projection);
	OutlineShader::SetProjection(projection);
	WaterShader::SetProjection(projection);
}

// Setup temp, disabled light
Light * ShaderManager::GetLight()
{
	// Setup details
	DirectX::XMFLOAT4 diffuse = { 0.f, 0.f, 0.f, 0.f };
	DirectX::XMFLOAT4 ambient = { 0.f, 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 position = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 direction = { 0.f, 0.f, 0.f };
	float constA = 0.f, linA = 0.f, quadA = 0.f;
	LIGHT_TYPE type = LIGHT_TYPE::NA;
	bool enable = false;

	// Setup temp light
	Light* light = new Light(diffuse, ambient, position, direction, constA, linA, quadA, type, enable);

	// Return temp light
	return light;
}
