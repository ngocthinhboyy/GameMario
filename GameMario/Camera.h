#pragma once
class Camera
{
	float cam_x;
	float cam_y;
	static Camera* __instance;
public:
	Camera();
	void SetCamPos(float x, float y);
	void GetCamPos(float& x, float& y);

	static Camera* GetInstance();
	~Camera();
};

typedef Camera* LPCAMERA;

