#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

#include <random>

struct TERRAIN {
	int row, column, ID;
	bool intersect = false;
	DirectX::SimpleMath::Vector3 position;
};

enum class MODEL_TYPE {
	NOT_WATER,
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
	/*RESIDENTIAL,
	NATURE,
	PROPS,*/

	// Residential
	HOUSE_ONE,
	HOUSE_TWO,
	CAVE,

	// Props
	BRIDGE,
	FENCE,
	BOAT,

	// Nature
	GRASS,
	TREE_ONE,
	TREE_TWO,
	WATER
};

enum class RESIDENTIAL_SPAWN {
	NA,
	HOUSE_ONE,
	HOUSE_TWO,
	CAVE
};

enum class NATURE_SPAWN {
	NA,
	GRASS,
	TREE_ONE,
	TREE_TWO,
	BUSH,
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

class Tools
{
public:
	int GetRandomNumber(int start, int end) {
		// Uniformly-distributed integer random number generator, that produces non-deterministic random numbers!
		std::random_device rd;

		//  Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits
		std::mt19937 mt(rd());

		// Uniform distribution
		std::uniform_int_distribution<> distribution(start, end);
		return distribution(mt);
	}
};