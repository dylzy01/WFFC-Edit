#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

struct TERRAIN {
	int row, column, ID;
	bool intersect = false;
	DirectX::SimpleMath::Vector3 position;
};

enum class MODEL_TYPE {
	PLACEHOLDER,
	GRASS,
	TREE,
	WATER
};

enum class EDITOR {
	NA,
	OBJECT_SPAWN,
	OBJECT_TRANSFORM,
	LANDSCAPE_PAINT,
	LANDSCAPE_SCULPT
};

enum class OBJECT_SPAWN {
	NA,
	GRASS,
	TREE,
	WATER
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

enum class LANDSCAPE_PAINT {
	NA,
	GRASS,
	DIRT,
	SAND,
	STONE,
	SNOW
};

enum class LANDSCAPE_SCULPT {
	NA,
	SELECT,
	INCREASE,
	FLATTEN,
	DECREASE
};

enum class LANDSCAPE_CONSTRAINT {
	NA,
	XY,
	XZ,
	YZ,
	X, 
	Y,
	Z,
	ALL
};