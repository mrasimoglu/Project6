#pragma once

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Camera
{
private:
	Vector2 pos;
	Vector2 view;

public:
	Vector2 GetOffset();
	Vector2 GetView();
	Vector2 GetPosition();
	void SetPosition(Vector2);
	void SetView(Vector2);

	Camera(Vector2, Vector2);
	~Camera();
};

