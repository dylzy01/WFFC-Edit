#include "ShaderManager.h"

// Initialise all shaders
void ShaderManager::Initialise(ID3D11Device * device)
{
	// Initialise toon shader
	TextureShader::Initialise(device);

	// Initialise toon blend shader
	BlendShader::Initialise(device);
}

// Setup specific shader parameters
void ShaderManager::Shader(SHADER_TYPE type, ID3D11DeviceContext * context, std::vector<DisplayObject> lights, ID3D11ShaderResourceView * texture1, ID3D11ShaderResourceView * texture2, DirectX::XMFLOAT2 screen)
{	
	// Setup temp light
	std::vector<DisplayObject> temp;
	temp.push_back(GetLight());

	// Switch between shader types
	switch (type)
	{
	case SHADER_TYPE::NORMAL_TEXTURE:
	{
		// Handle texture shader
		TextureShader::Enable(context);

		// If light vector is empty
		if (lights.size() == 0)
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(true, context, texture1, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(true, context, texture1, lights);
		}
	}
	break;
	case SHADER_TYPE::NORMAL_BLEND:
	{
		// Handle blend shader
		BlendShader::Enable(context);

		// If light vector is empty
		if (lights.size() == 0)
		{
			// Setup texture parameters
			BlendShader::SetShaderParameters(true, context, texture1, texture2, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup texture parameters
			BlendShader::SetShaderParameters(true, context, texture1, texture2, lights);
		}
	}
	break;
	case SHADER_TYPE::TOON_TEXTURE:
	{
		// Handle texture shader
		TextureShader::Enable(context);

		// If light vector is empty
		if (lights.size() == 0)
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(false, context, texture1, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup texture parameters
			TextureShader::SetShaderParameters(false, context, texture1, lights);
		}
	}
	break;
	case SHADER_TYPE::TOON_BLEND:
	{
		// Handle blend shader
		BlendShader::Enable(context);

		// If light vector is empty
		if (lights.size() == 0)
		{
			// Setup texture parameters
			BlendShader::SetShaderParameters(false, context, texture1, texture2, temp);
		}
		// Else, if light vector is valid
		else
		{
			// Setup texture parameters
			BlendShader::SetShaderParameters(false, context, texture1, texture2, lights);
		}
	}
	break;
	}
}

// Set shader world matrices
void ShaderManager::SetWorld(DirectX::SimpleMath::Matrix * world)
{
	TextureShader::SetWorld(world);
	BlendShader::SetWorld(world);
}

// Set shader view matrices
void ShaderManager::SetView(DirectX::SimpleMath::Matrix * view)
{
	TextureShader::SetView(view);
	BlendShader::SetView(view);
}

// Set shader projection matrices
void ShaderManager::SetProjection(DirectX::SimpleMath::Matrix * projection)
{
	TextureShader::SetProjection(projection);
	BlendShader::SetProjection(projection);
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