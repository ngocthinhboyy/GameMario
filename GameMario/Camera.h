#pragma once
class Camera
{
	float cam_x;
	float cam_y;
	static Camera* __instance;
	bool canChangeCamY = false;
public:
	Camera();
	void SetCamPos(float x, float y);
	void GetCamPos(float& x, float& y);

	void UpdateCamPos();

	static Camera* GetInstance();
	~Camera();
};

typedef Camera* LPCAMERA;

