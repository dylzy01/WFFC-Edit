#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

#include <random>

enum class MODEL_TYPE {
	NOT_WATER,
	WATER
};

enum class EDITOR {
	NA,
	OBJECT_SPAWN,
	OBJECT_FUNCTION,
	TERRAIN_PAINT,
	SCULPT_FREELY,
	SCULPT_SINGLE
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
	WATER,

	// Misc,
	LIGHT,
	CUBE,
	CYLINDER,
	CONE
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

enum class CONSTRAINT {
	NA,
	X,
	Y,
	Z,
	XY,
	XZ,
	YZ
};

enum class TERRAIN_PAINT {
	NA,
	GRASS,
	DIRT,
	SAND,
	STONE,
	SNOW,
	GRASS_DIRT,
	GRASS_SAND,
	GRASS_STONE,
	GRASS_SNOW,
	DIRT_SAND,
	DIRT_STONE,
	DIRT_SNOW,
	SAND_STONE,
	SAND_SNOW,
	STONE_SNOW
};

enum class TERRAIN_SCULPT {
	NA,
	INCREASE,
	FLATTEN,
	DECREASE,
	SELECT,
};

enum class SHADER_TYPE {
	TEXTURE,
	BLEND,
	TOON_SINGLE,
	TOON_BLEND,
	WATER
};

enum class LIGHT_TYPE {
	NA = 0,
	DIRECTIONAL = 1,
	POINT = 2,
	SPOT = 3
};

struct TERRAIN {
	int row, column, ID;
	bool intersect = false;
	DirectX::SimpleMath::Vector3 position;
	TERRAIN_PAINT paint;
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