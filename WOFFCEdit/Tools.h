#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

#include <random>

enum class EDITOR {
	NA,
	OBJECT_SPAWN,
	OBJECT_FUNCTION,
	TERRAIN_PAINT,
	SCULPT_FREELY,
	SCULPT_SINGLE,
	LIGHTS
};

enum class OBJECT_TYPE {
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

	// Misc,
	CUBE,
	CYLINDER,
	CONE,

	// Other	
	WATER,
	LIGHT,
	NA
};

enum class OBJECT_FUNCTION {
	SELECT,
	SCALE,
	TRANSLATE,
	ROTATE,

	NA
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
	DIRECTIONAL = 0,
	POINT = 1,
	NA = 2
};

enum class PICK_TYPE {
	ANY,
	OBJECT,
	LIGHT
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