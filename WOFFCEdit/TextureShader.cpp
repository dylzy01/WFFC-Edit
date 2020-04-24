#include "TextureShader.h"

Microsoft::WRL::ComPtr<ID3D11VertexShader>	TextureShader::m_shaderVertex;
Microsoft::WRL::ComPtr<ID3D11PixelShader>	TextureShader::m_shaderPixel;
ID3D11InputLayout *							TextureShader::m_inputLayout;
ID3D11Buffer *								TextureShader::m_bufferMatrix;
ID3D11SamplerState *						TextureShader::m_samplerState;
ID3D11Buffer *								TextureShader::m_bufferLight;
DirectX::SimpleMath::Matrix *				TextureShader::m_world;
DirectX::SimpleMath::Matrix *				TextureShader::m_view;
DirectX::SimpleMath::Matrix *				TextureShader::m_projection;

// Load vertex & pixel shaders
bool TextureShader::Initialise(ID3D11Device * device)
{
	D3D11_BUFFER_DESC	matrixBufferDesc;
	D3D11_SAMPLER_DESC	samplerDesc;
	D3D11_BUFFER_DESC	lightBufferDesc;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load shader : Vertex
	auto vertexShaderBuffer = DX::ReadData(L"texture_vs.cso");
	HRESULT result = device->CreateVertexShader(vertexShaderBuffer.data(), vertexShaderBuffer.size(), NULL, &m_shaderVertex);
	if (result != S_OK) { return false; }

	// Create vertex input layout description
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Count elements in layout
	unsigned int elements;
	elements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create vertex input layout
	device->CreateInputLayout(polygonLayout, elements, vertexShaderBuffer.data(), vertexShaderBuffer.size(), &m_inputLayout);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load shader : Pixel
	auto pixelShaderBuffer = DX::ReadData(L"texture_ps.cso");
	result = device->CreatePixelShader(pixelShaderBuffer.data(), pixelShaderBuffer.size(), NULL, &m_shaderPixel);
	if (result != S_OK) { return false; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setup description of dynamic matrix constant buffer within vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create constant buffer pointer for access to vertex shader constant buffer
	device->CreateBuffer(&matrixBufferDesc, NULL, &m_bufferMatrix);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setup description of light buffer
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create constant buffer pointer for access to vertex shader constant buffer
	device->CreateBuffer(&lightBufferDesc, NULL, &m_bufferLight);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create texture sampler state
	device->CreateSamplerState(&samplerDesc, &m_samplerState);

	return true;
}

// Setup shader 
bool TextureShader::SetShaderParameters(ID3D11DeviceContext * context, ID3D11ShaderResourceView* texture)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	DirectX::SimpleMath::Matrix  tworld, tview, tproj;

	// Transpose matrices for shader preperation
	tworld = m_world->Transpose();
	tview = m_view->Transpose();
	tproj = m_projection->Transpose();
	context->Map(m_bufferMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	context->Unmap(m_bufferMatrix, 0);
	context->VSSetConstantBuffers(0, 1, &m_bufferMatrix);

	context->Map(m_bufferLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	/*lightPtr->ambient = sceneLight1->getAmbientColour();
	lightPtr->diffuse = sceneLight1->getDiffuseColour();
	lightPtr->position = sceneLight1->getPosition();*/
	lightPtr->ambient = DirectX::SimpleMath::Vector4(0.3f, 0.3f, 0.3f, 1.f);
	lightPtr->diffuse = DirectX::SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);
	lightPtr->position = DirectX::SimpleMath::Vector3(2.f, 1.f, 1.f);
	lightPtr->padding = 0.0f;
	context->Unmap(m_bufferLight, 0);
	context->PSSetConstantBuffers(0, 1, &m_bufferLight);

	// Pass textures to pixel shader
	context->PSSetShaderResources(0, 1, &texture);

	return true;
}

// Handler
void TextureShader::Enable(ID3D11DeviceContext * context)
{
	// Set input layout for shader to match geometry
	context->IASetInputLayout(m_inputLayout);

	// Turn on shaders
	context->VSSetShader(m_shaderVertex.Get(), 0, 0);
	context->PSSetShader(m_shaderPixel.Get(), 0, 0);

	// Set sampler states in pixel shader
	context->PSSetSamplers(0, 1, &m_samplerState);
	context->PSSetSamplers(1, 1, &m_samplerState);
}