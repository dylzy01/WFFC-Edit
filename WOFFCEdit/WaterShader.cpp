#include "WaterShader.h"


Microsoft::WRL::ComPtr<ID3D11VertexShader>	WaterShader::m_shaderVertex;
Microsoft::WRL::ComPtr<ID3D11PixelShader>	WaterShader::m_shaderPixel;
ID3D11InputLayout *							WaterShader::m_inputLayout;
ID3D11Buffer *								WaterShader::m_bufferMatrix;
ID3D11Buffer *								WaterShader::m_bufferWaterVS;
ID3D11Buffer *								WaterShader::m_bufferWaterPS;
ID3D11SamplerState *						WaterShader::m_samplerState;
DirectX::SimpleMath::Matrix *				WaterShader::m_world;
DirectX::SimpleMath::Matrix *				WaterShader::m_view;
DirectX::SimpleMath::Matrix *				WaterShader::m_projection;


bool WaterShader::Initialise(ID3D11Device * device)
{
	D3D11_BUFFER_DESC	matrixBufferDesc;
	D3D11_SAMPLER_DESC	samplerDesc;
	D3D11_BUFFER_DESC	waterBufferDescVS;
	D3D11_BUFFER_DESC	waterBufferDescPS;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load shader : Vertex
	auto vertexShaderBuffer = DX::ReadData(L"water_vs.cso");
	HRESULT result = device->CreateVertexShader(vertexShaderBuffer.data(), vertexShaderBuffer.size(), NULL, &m_shaderVertex);
	if (result != S_OK) { return false; }

	// Create vertex input layout description
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Count elements in layout
	unsigned int elements;
	elements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create vertex input layout
	device->CreateInputLayout(polygonLayout, elements, vertexShaderBuffer.data(), vertexShaderBuffer.size(), &m_inputLayout);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load shader : Pixel
	auto pixelShaderBuffer = DX::ReadData(L"water_ps.cso");
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
	// Setup description of water vertex buffer
	waterBufferDescVS.Usage = D3D11_USAGE_DYNAMIC;
	waterBufferDescVS.ByteWidth = sizeof(WaterBufferTypeVS);
	waterBufferDescVS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	waterBufferDescVS.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	waterBufferDescVS.MiscFlags = 0;
	waterBufferDescVS.StructureByteStride = 0;

	// Create constant buffer pointer for access to vertex shader constant buffer
	device->CreateBuffer(&waterBufferDescVS, NULL, &m_bufferWaterVS);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setup description of water pixel buffer
	waterBufferDescPS.Usage = D3D11_USAGE_DYNAMIC;
	waterBufferDescPS.ByteWidth = sizeof(WaterBufferTypePS);
	waterBufferDescPS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	waterBufferDescPS.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	waterBufferDescPS.MiscFlags = 0;
	waterBufferDescPS.StructureByteStride = 0;

	// Create constant buffer pointer for access to vertex shader constant buffer
	device->CreateBuffer(&waterBufferDescPS, NULL, &m_bufferWaterPS);

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

bool WaterShader::SetShaderParameters(ID3D11DeviceContext * context, ID3D11ShaderResourceView * texture1, ID3D11ShaderResourceView * texture2, ID3D11ShaderResourceView * textureNormal, XMFLOAT3 camPosition, XMFLOAT2 repeatNormal, float translation, float scale, XMFLOAT4 tint)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	WaterBufferTypeVS* waterPtrVS;
	WaterBufferTypePS* waterPtrPS;
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

	// Send water vertex data
	context->Map(m_bufferWaterVS, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	waterPtrVS = (WaterBufferTypeVS*)mappedResource.pData;
	waterPtrVS->position = camPosition;
	waterPtrVS->normal = repeatNormal;
	waterPtrVS->padding = { 0.f, 0.f, 0.f };
	context->Unmap(m_bufferWaterVS, 0);
	context->VSSetConstantBuffers(1, 1, &m_bufferWaterVS);

	// Send water pixel data
	context->Map(m_bufferWaterPS, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	waterPtrPS->tint = tint;
	waterPtrPS->translation = translation;
	waterPtrPS->scale = scale;
	waterPtrPS->padding = { 0.f, 0.f };
	context->Unmap(m_bufferWaterPS, 0);
	context->PSSetConstantBuffers(0, 1, &m_bufferWaterPS);

	// Pass textures to pixel shader
	context->PSSetShaderResources(0, 1, &texture1);
	context->PSSetShaderResources(1, 1, &texture2);
	context->PSSetShaderResources(2, 1, &textureNormal);

	return true;
}

void WaterShader::Enable(ID3D11DeviceContext * context)
{
	// Set input layout for shader to match geometry
	context->IASetInputLayout(m_inputLayout);

	// Turn on shaders
	context->VSSetShader(m_shaderVertex.Get(), 0, 0);
	context->PSSetShader(m_shaderPixel.Get(), 0, 0);

	// Set sampler states in pixel shader
	context->PSSetSamplers(0, 1, &m_samplerState);
}
