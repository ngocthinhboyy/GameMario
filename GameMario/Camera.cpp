#include "Camera.h"
#include <stdio.h>

Camera* Camera::__instance = NULL;
Camera::Camera()
{
	this->cam_x = 0.0f;
	this->cam_y = 0.0f;
}

void Camera::SetCamPos(float x, float y)
{
	this->cam_x = x;
	this->cam_y = y;
}

void Camera::GetCamPos(float& x, float& y)
{
	x = this->cam_x;
	y = this->cam_y;
}

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

Camera::~Camera()
{
}
