#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

struct TERRAIN {
	int row, column, ID;
	bool intersect = false;
	DirectX::SimpleMath::Vector3 position;
};

enum class EDITOR {
	NA,
	OBJECT,
	SCULPT
};

enum class OBJECT_FUNCTION {
	NA,
	SELECT,
	SCALE,
	TRANSLATE,
	ROTATE
};

enum class OBJECT_CONSTRAINT {
	NA,
	XY,
	XZ,
	YZ,
	X,
	Y,
	Z,
	ALL
};

enum class SCULPT_FUNCTION {
	NA,
	SELECT,
	INCREASE,
	FLATTEN,
	DECREASE
};

enum class SCULPT_CONSTRAINT {
	NA,
	X, 
	Y,
	Z,
	ALL
};

enum class HEIGHT {
	INCREASE,
	DECREASE,
	FLATTEN
};