#include "TextureShader.h"

//ID3D11VertexShader*									TextureShader::m_vertexShader;
//ID3D11PixelShader*									TextureShader::m_pixelShader;
//ID3D11InputLayout*									TextureShader::m_layout;
//ID3D11Buffer*										TextureShader::m_matrixBuffer;
//ID3D11SamplerState*									TextureShader::m_sampleState;
//DirectX::SimpleMath::Matrix *						TextureShader::m_world;
//DirectX::SimpleMath::Matrix *						TextureShader::m_view;
//DirectX::SimpleMath::Matrix *						TextureShader::m_projection;
//
//TextureShader::TextureShader()
//{
//	m_vertexShader = 0;
//	m_pixelShader = 0;
//	m_layout = 0;
//	m_matrixBuffer = 0;
//	m_sampleState = 0;
//}
//
//bool TextureShader::Initialise(ID3D11Device * device, HWND hwnd)
//{
//	bool result;
//	
//	// Initialise vertex and pixel shaders
//	result = InitialiseShader(device, hwnd, L"texture_vs.cso", L"texture_ps.cso");
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void TextureShader::Shutdown()
//{
//	// Shutdown vertex/pixel shaders & all related objects
//	ShutdownShader();
//
//	return;
//}
//
//bool TextureShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, DirectX::SimpleMath::Matrix worldMatrix, 
//	DirectX::SimpleMath::Matrix viewMatrix, DirectX::SimpleMath::Matrix projectionMatrix, ID3D11ShaderResourceView ** textureArray)
//{
//	bool result;
//
//	// Set the shader parameters that it will use for rendering.
//	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray);
//	if (!result)
//	{
//		return false;
//	}
//
//	// Now render the prepared buffers with the shader.
//	RenderShader(deviceContext, indexCount);
//
//	return true;
//}
//
//bool TextureShader::InitialiseShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
//{
//	HRESULT result;
//	ID3D10Blob* errorMessage;
//	ID3D10Blob* vertexShaderBuffer;
//	ID3D10Blob* pixelShaderBuffer;
//	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
//	unsigned int numElements;
//	D3D11_BUFFER_DESC matrixBufferDesc;
//	D3D11_SAMPLER_DESC samplerDesc;
//
//	// Reset pointers
//	errorMessage = vertexShaderBuffer = pixelShaderBuffer = 0;
//
//	// Compile vertex shader
//	result = D3DCompileFromFile(vsFilename, NULL, NULL, "MultiTextureVertexShader", "vs_5_0", 
//		D3D10_SHADER_ENABLE_STRICTNESS,	0, &vertexShaderBuffer, &errorMessage);
//
//	if (FAILED(result))
//	{
//		// If the shader failed to compile, the error message should contain something
//		if (errorMessage) { OutputShaderErrorMessage(errorMessage, hwnd, vsFilename); }
//		
//		// Else, if error message is empty then shader couldn't be found
//		else { MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK); }
//
//		return false;
//	}
//
//	// Compile pixel shader
//	result = D3DCompileFromFile(psFilename, NULL, NULL, "MultiTexturePixelShader", "ps_5_0",
//		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
//
//	if (FAILED(result))
//	{
//		// If the shader failed to compile, the error message should contain something
//		if (errorMessage) { OutputShaderErrorMessage(errorMessage, hwnd, psFilename); }
//
//		// Else, if error message is empty then shader couldn't be found
//		else { MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK); }
//
//		return false;
//	}
//
//	// Create vertex shader from buffer
//	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
//	if (FAILED(result)) { return false; }
//
//	// Create pixel shader from buffer
//	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
//	if (FAILED(result)) { return false; }
//
//	// Create vertex input layout description
//	// Setup needs to match the VertexType structure in shader
//	polygonLayout[0].SemanticName = "POSITION";
//	polygonLayout[0].SemanticIndex = 0;
//	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	polygonLayout[0].InputSlot = 0;
//	polygonLayout[0].AlignedByteOffset = 0;
//	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	polygonLayout[0].InstanceDataStepRate = 0;
//
//	polygonLayout[1].SemanticName = "TEXCOORD";
//	polygonLayout[1].SemanticIndex = 0;
//	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	polygonLayout[1].InputSlot = 0;
//	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
//	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	polygonLayout[1].InstanceDataStepRate = 0;
//
//	// Count elements in layout
//	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
//
//	// Create vertex input layout
//	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
//	if (FAILED(result)) { return false; }
//
//	// Release vertex and pixel shader buffers as no longer needed
//	vertexShaderBuffer->Release();	vertexShaderBuffer = 0;
//	pixelShaderBuffer->Release();	pixelShaderBuffer = 0;
//
//	// Setup description of matrix dynamic constant buffer in the vertex shader
//	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
//	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	matrixBufferDesc.MiscFlags = 0;
//	matrixBufferDesc.StructureByteStride = 0;
//
//	// Create matrix constant buffer pointer to access vertex shader constant buffer from this class
//	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
//	if (FAILED(result)) { return false; }
//
//	// Create texture sampler state description
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.MipLODBias = 0.0f;
//	samplerDesc.MaxAnisotropy = 1;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	samplerDesc.BorderColor[0] = 0;
//	samplerDesc.BorderColor[1] = 0;
//	samplerDesc.BorderColor[2] = 0;
//	samplerDesc.BorderColor[3] = 0;
//	samplerDesc.MinLOD = 0;
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	// Create texture sampler state
//	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
//	if (FAILED(result)) { return false; }
//
//	return true;
//}
//
//void TextureShader::ShutdownShader()
//{
//	// Release the sampler state.
//	if (m_sampleState)
//	{
//		m_sampleState->Release();
//		m_sampleState = 0;
//	}
//
//	// Release the matrix constant buffer.
//	if (m_matrixBuffer)
//	{
//		m_matrixBuffer->Release();
//		m_matrixBuffer = 0;
//	}
//
//	// Release the layout.
//	if (m_layout)
//	{
//		m_layout->Release();
//		m_layout = 0;
//	}
//
//	// Release the pixel shader.
//	if (m_pixelShader)
//	{
//		m_pixelShader->Release();
//		m_pixelShader = 0;
//	}
//
//	// Release the vertex shader.
//	if (m_vertexShader)
//	{
//		m_vertexShader->Release();
//		m_vertexShader = 0;
//	}
//
//	return;
//}
//
//void TextureShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
//{
//	char* compileErrors;
//	unsigned long bufferSize, i;
//	std::ofstream fout;
//
//
//	// Get a pointer to the error message text buffer.
//	compileErrors = (char*)(errorMessage->GetBufferPointer());
//
//	// Get the length of the message.
//	bufferSize = errorMessage->GetBufferSize();
//
//	// Open a file to write the error message to.
//	fout.open("shader-error.txt");
//
//	// Write out the error message.
//	for (i = 0; i < bufferSize; i++)
//	{
//		fout << compileErrors[i];
//	}
//
//	// Close the file.
//	fout.close();
//
//	// Release the error message.
//	errorMessage->Release();
//	errorMessage = 0;
//
//	// Pop a message up on the screen to notify the user to check the text file for compile errors.
//	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
//
//	return;
//}
//
//bool TextureShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, DirectX::SimpleMath::Matrix worldMatrix, 
//	DirectX::SimpleMath::Matrix viewMatrix, DirectX::SimpleMath::Matrix projectionMatrix, ID3D11ShaderResourceView ** textureArray)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	MatrixBufferType* dataPtr;
//	unsigned int bufferNumber;
//
//	// Transpose matrices to prepare for shader
//	///DirectX::SimpleMath::Matrix::Transpose(&worldMatrix, &worldMatrix);
//	DirectX::XMMatrixTranspose(worldMatrix);
//	DirectX::XMMatrixTranspose(viewMatrix);
//	DirectX::XMMatrixTranspose(projectionMatrix);
//
//	// Lock matrix constant buffer for writing
//	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result)) { return false; }
//
//	// Store a pointer to constant buffer data
//	dataPtr = (MatrixBufferType*)mappedResource.pData;
//
//	// Copy matrices into constant buffer
//	dataPtr->world = worldMatrix;
//	dataPtr->view = viewMatrix;
//	dataPtr->projection = projectionMatrix;
//
//	// Unlock matrix constant buffer
//	deviceContext->Unmap(m_matrixBuffer, 0);
//
//	// Set position of matrix constant buffer in vertex shader
//	bufferNumber = 0;
//
//	// Set matrix constant buffer in vertex shader, with updated values
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
//
//	// Set shader texture array resource in pixel shader
//	deviceContext->PSSetShaderResources(0, 2, textureArray);
//
//	return true;
//}
//
//void TextureShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
//{
//	// Set vertex input layout
//	deviceContext->IASetInputLayout(m_layout);
//	
//	// Set vertex and pixel shaders to render current geometry
//	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
//	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
//
//	// Set sampler state in pixel shader
//	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
//	
//	// Render geometry
//	deviceContext->DrawIndexed(indexCount, 0, 0);
//
//	return;
//}

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
bool TextureShader::Initialise(ID3D11Device * device, WCHAR * vsFilename, WCHAR * psFilename)
{
	D3D11_BUFFER_DESC	matrixBufferDesc;
	D3D11_SAMPLER_DESC	samplerDesc;
	D3D11_BUFFER_DESC	lightBufferDesc;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load shader : Vertex
	auto vertexShaderBuffer = DX::ReadData(vsFilename);
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
	auto pixelShaderBuffer = DX::ReadData(psFilename);
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
bool TextureShader::SetShaderParameters(ID3D11DeviceContext * context, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2)
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
	context->PSSetShaderResources(0, 1, &texture1);
	context->PSSetShaderResources(1, 1, &texture2);

	// Set shader texture array resource in pixel shader
	///context->PSSetShaderResources(0, 2, textureArray);

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