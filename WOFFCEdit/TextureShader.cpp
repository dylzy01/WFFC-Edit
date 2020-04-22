//#include "TextureShader.h"
//
//ID3D11VertexShader* TextureShader::m_vertexShader;
//ID3D11PixelShader* TextureShader::m_pixelShader;
//ID3D11InputLayout* TextureShader::m_layout;
//ID3D11Buffer* TextureShader::m_matrixBuffer;
//ID3D11SamplerState* TextureShader::m_sampleState;
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
//	///result = InitialiseShader(device, hwnd, L"multitexture.vs", L"multitexture.ps");
//	result = InitialiseShader(device, hwnd, L"texture_vs.hlsl", L"texture_ps.hlsl");
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
