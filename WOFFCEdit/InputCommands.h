#pragma once

struct InputCommands
{
	bool W;
	bool S;
	bool D;
	bool A;
	bool E;
	bool Q;
	bool rotRight;
	bool rotLeft;
	bool mouseLeft;
	bool mouseRight, pickOnce, storeOnce;
	bool escape;
	bool ONE;
	bool TWO;
	bool SCALE, TRANSLATE, ROTATE;
	bool X, Y, Z;
	bool INCREASE, DECREASE, FLATTEN;
	bool ZERO;
	DirectX::SimpleMath::Vector2 mousePos;
	DirectX::SimpleMath::Vector2 mousePosPrevious;
};
