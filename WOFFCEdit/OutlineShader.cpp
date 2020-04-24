#include "OutlineShader.h"

Microsoft::WRL::ComPtr<ID3D11VertexShader>	OutlineShader::m_shaderVertex;
Microsoft::WRL::ComPtr<ID3D11PixelShader>	OutlineShader::m_shaderPixel;
ID3D11InputLayout *							OutlineShader::m_inputLayout;
ID3D11Buffer *								OutlineShader::m_bufferMatrix;
ID3D11SamplerState *						OutlineShader::m_samplerState;
DirectX::SimpleMath::Matrix *				OutlineShader::m_world;
DirectX::SimpleMath::Matrix *				OutlineShader::m_view;
DirectX::SimpleMath::Matrix *				OutlineShader::m_projection;
ID3D11SamplerState *						OutlineShader::m_sampleState;
ID3D11Buffer *								OutlineShader::m_bufferScreen;
ID3D11Buffer *								OutlineShader::m_bufferOutline;
float										OutlineShader::m_edgeWidth;
float										OutlineShader::m_edgeIntensity;
float										OutlineShader::m_thresholdNormal;
float										OutlineShader::m_thresholdDepth;
float										OutlineShader::m_sensitivityNormal;
float										OutlineShader::m_sensitityDepth;

bool OutlineShader::Initialise(ID3D11Device * device)
{
	D3D11_BUFFER_DESC	matrixBufferDesc;
	D3D11_SAMPLER_DESC	samplerDesc;
	D3D11_BUFFER_DESC	screenBufferDesc, outlineBufferDesc;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load shader : Vertex
	auto vertexShaderBuffer = DX::ReadData(L"outline_vs.cso");
	HRESULT result = device->CreateVertexShader(vertexShaderBuffer.data(), vertexShaderBuffer.size(), NULL, &m_shaderVertex);
	if (result != S_OK) { return false; }

	// Create vertex input layout description
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },		
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Count elements in layout
	unsigned int elements;
	elements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create vertex input layout
	device->CreateInputLayout(polygonLayout, elements, vertexShaderBuffer.data(), vertexShaderBuffer.size(), &m_inputLayout);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load shader : Pixel
	auto pixelShaderBuffer = DX::ReadData(L"outline_ps.cso");
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
	// Setup description of screen buffer
	screenBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	screenBufferDesc.ByteWidth = sizeof(ScreenBufferType);
	screenBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	screenBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screenBufferDesc.MiscFlags = 0;
	screenBufferDesc.StructureByteStride = 0;

	// Create constant buffer pointer for access to vertex shader constant buffer
	device->CreateBuffer(&screenBufferDesc, NULL, &m_bufferScreen);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setup description of outline buffer
	outlineBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	outlineBufferDesc.ByteWidth = sizeof(ScreenBufferType);
	outlineBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	outlineBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	outlineBufferDesc.MiscFlags = 0;
	outlineBufferDesc.StructureByteStride = 0;

	// Create constant buffer pointer for access to vertex shader constant buffer
	device->CreateBuffer(&outlineBufferDesc, NULL, &m_bufferOutline);

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
	device->CreateSamplerState(&samplerDesc, &m_sampleState);

	return true;
}

bool OutlineShader::SetShaderParameters(ID3D11DeviceContext * context, ID3D11ShaderResourceView * texture1, DirectX::XMFLOAT2 screen)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	ScreenBufferType* screenPtr;
	OutlineBufferType* outlinePtr;
	DirectX::SimpleMath::Matrix  tworld, tview, tproj;

	// Transpose matrices for shader preperation
	tworld = m_world->Transpose();
	tview = m_view->Transpose();
	tproj = m_projection->Transpose();

	// Send matrix data
	context->Map(m_bufferMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	context->Unmap(m_bufferMatrix, 0);
	context->VSSetConstantBuffers(0, 1, &m_bufferMatrix);

	// Send screen data
	context->Map(m_bufferScreen, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	screenPtr = (ScreenBufferType*)mappedResource.pData;
	screenPtr->screenHeight = screen.y;
	screenPtr->screenWidth = screen.x;
	screenPtr->padding = DirectX::XMFLOAT2(0.0f, 0.0f);
	context->Unmap(m_bufferScreen, 0);
	context->PSSetConstantBuffers(0, 1, &m_bufferScreen);

	// Send outline data
	context->Map(m_bufferOutline, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	outlinePtr = (OutlineBufferType*)mappedResource.pData;
	outlinePtr->edgeWidth = m_edgeWidth;
	outlinePtr->edgeIntensity = m_edgeIntensity;
	outlinePtr->thresholdNormal = m_thresholdNormal;
	outlinePtr->thresholdDepth = m_thresholdDepth;
	outlinePtr->sensitivityNormal = m_sensitivityNormal;
	outlinePtr->sensitivityDepth = m_sensitityDepth;
	outlinePtr->padding = DirectX::XMFLOAT2(0.0f, 0.0f);
	context->Unmap(m_bufferOutline, 0);
	context->PSSetConstantBuffers(0, 1, &m_bufferOutline);

	// Pass textures to pixel shader
	context->PSSetShaderResources(0, 1, &texture1);	

	return true;
}

void OutlineShader::Enable(ID3D11DeviceContext * context)
{
	// Set input layout for shader to match geometry
	context->IASetInputLayout(m_inputLayout);

	// Turn on shaders
	context->VSSetShader(m_shaderVertex.Get(), 0, 0);
	context->PSSetShader(m_shaderPixel.Get(), 0, 0);

	// Set sampler states in pixel shader
	context->PSSetSamplers(0, 1, &m_samplerState);
	context->PSSetSamplers(1, 1, &m_sampleState);
}
