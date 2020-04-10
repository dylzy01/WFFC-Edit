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
	bool mouseRight;
	bool mouseWheel;
	bool mouseDrag;
	bool pickOnce, storeOnce, toggle;
	bool escape;
	bool SHIFT;
	bool CTRL;
	DirectX::SimpleMath::Vector2 mousePos;
	DirectX::SimpleMath::Vector2 mousePosPrevious;
};
