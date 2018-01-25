#include "Camera.h"

Vector2 Camera::GetOffset()
{
	return pos - (view / Vector2(2, 2));
}

Vector2 Camera::GetView()
{
	return view;
}

Vector2 Camera::GetPosition()
{
	return pos;
}

void Camera::SetPosition(Vector2 pos)
{
	this->pos = pos;
}

void Camera::SetView(Vector2 view)
{
	this->view = view;
}

Camera::Camera(Vector2 pos, Vector2 view)
{
	this->pos = pos;
	this->view = view;
}


Camera::~Camera()
{
}
