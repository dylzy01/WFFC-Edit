#pragma once
#include "pch.h"
#include "Tools.h"
#include <d3dcompiler.h>

class Shader
{
public:
	Shader() {}
	~Shader() {}	

protected:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};	
};

