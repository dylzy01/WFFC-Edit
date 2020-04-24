#include "ShaderManager.h"

// Initialise all shaders
void ShaderManager::Initialise(ID3D11Device * device)
{
	// Initialise texture shader
	TextureShader::Initialise(device);

	// Initialise blend shader
	BlendShader::Initialise(device);
}

// Setup specific shader parameters
void ShaderManager::Shader(SHADER_TYPE type, ID3D11DeviceContext * context, ID3D11ShaderResourceView * texture1, ID3D11ShaderResourceView * texture2)
{
	// Switch between shader type
	switch (type)
	{
	case SHADER_TYPE::TEXTURE:
	{
		// Handle texture shader
		TextureShader::Enable(context);
		
		// Setup texture parameters
		TextureShader::SetShaderParameters(context, texture1);
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
