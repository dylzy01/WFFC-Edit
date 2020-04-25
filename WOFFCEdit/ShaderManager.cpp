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

	// Initialise outline shader
	OutlineShader::Initialise(device);
}

// Setup specific shader parameters
void ShaderManager::Shader(SHADER_TYPE type, ID3D11DeviceContext * context, std::vector<Light*> light, ID3D11ShaderResourceView * texture1, ID3D11ShaderResourceView * texture2, DirectX::XMFLOAT2 screen)
{	
	// Switch between shader type
	switch (type)
	{
	case SHADER_TYPE::TEXTURE:
	{
		// Handle texture shader
		TextureShader::Enable(context);
		
		// Setup texture parameters
		TextureShader::SetShaderParameters(context, texture1, light);
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
	case SHADER_TYPE::TOON:
	{
		// Handle toon shader
		ToonShader::Enable(context);

		// Setup blend parameters
		ToonShader::SetShaderParameters(context, texture1, texture2);
	}
	break;
	case SHADER_TYPE::OUTLINE:
	{
		// Handle toon shader
		OutlineShader::Enable(context);

		// Setup blend parameters
		OutlineShader::SetShaderParameters(context, texture1, screen);
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
	OutlineShader::SetWorld(world);
}

// Set shader view matrices
void ShaderManager::SetView(DirectX::SimpleMath::Matrix * view)
{
	TextureShader::SetView(view);
	BlendShader::SetView(view);
	ToonShader::SetView(view);
	OutlineShader::SetView(view);
}

// Set shader projection matrices
void ShaderManager::SetProjection(DirectX::SimpleMath::Matrix * projection)
{
	TextureShader::SetProjection(projection);
	BlendShader::SetProjection(projection);
	ToonShader::SetProjection(projection);
	OutlineShader::SetProjection(projection);
}
