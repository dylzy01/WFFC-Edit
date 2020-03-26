#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

struct TERRAIN {
	int row, column, ID;
	bool intersect = false;
	DirectX::SimpleMath::Vector3 position;
};

enum class MODE {
	OBJECT,
	LANDSCAPE
};

enum class HEIGHT {
	INCREASE,
	DECREASE,
	FLATTEN
};

enum class MODIFY {
	SCALE,
	TRANSLATE,
	ROTATE
};