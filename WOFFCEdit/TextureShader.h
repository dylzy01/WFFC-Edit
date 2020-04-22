//// Class and related shaders sourced from RasterTek.com //
//
//#pragma once
//#include "pch.h"
//#include "Tools.h"
//#include <d3dcompiler.h>
//
//class TextureShader
//{
//private:
//	struct MatrixBufferType
//	{
//		DirectX::SimpleMath::Matrix world, view, projection;
//	};
//
//public:
//	TextureShader();
//	TextureShader(const TextureShader&) {}
//	~TextureShader() {}
//
//	static bool Initialise(ID3D11Device*, HWND);
//	static void Shutdown();
//	static bool Render(ID3D11DeviceContext*, int, DirectX::SimpleMath::Matrix, DirectX::SimpleMath::Matrix, DirectX::SimpleMath::Matrix, ID3D11ShaderResourceView**);
//
//private:
//	static bool InitialiseShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
//	static void ShutdownShader();
//	static void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
//
//	static bool SetShaderParameters(ID3D11DeviceContext*, DirectX::SimpleMath::Matrix, DirectX::SimpleMath::Matrix, DirectX::SimpleMath::Matrix, ID3D11ShaderResourceView**);
//	static void RenderShader(ID3D11DeviceContext*, int);
//
//private:
//	static ID3D11VertexShader* m_vertexShader;
//	static ID3D11PixelShader* m_pixelShader;
//	static ID3D11InputLayout* m_layout;
//	static ID3D11Buffer* m_matrixBuffer;
//	static ID3D11SamplerState* m_sampleState;
//};
//
