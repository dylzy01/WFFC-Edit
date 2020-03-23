#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

struct CHUNK {
	int row, column, ID;
	bool intersect = false;
	DirectX::SimpleMath::Vector3 position;
};