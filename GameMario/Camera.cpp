#include "Camera.h"

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

void Camera::UpdateCamPos(DWORD dt)
{
	Game* game = Game::GetInstance();
	PlayScene* scene = dynamic_cast<PlayScene*> (Game::GetInstance()->GetCurrentScene());
	if (scene->GetIsAutoMovingCamera()) {
		this->cam_y = CAM_Y_DRAW_MAP;
		if (isAutoMovingCamera) {
			if (this->cam_x + game->GetScreenWidth() >= 6160) {
				this->cam_x = 6160 - game->GetScreenWidth();
			}
			else {
				this->cam_x = this->cam_x + 0.09f * dt;
			}
		}
		else {
			float cx, cy;
			Mario* player = Mario::GetInstance();
			player->GetPosition(cx, cy);
			if (cx - (game->GetScreenWidth() / 2) <= 6192.0f) {
				cx = 6192.0f;
			}
			else
			{
				if (cx + game->GetScreenWidth() / 2 < CAM_X_END_SCENE) {
					cx -= game->GetScreenWidth() / 2;
				}
				else {
					cx = CAM_X_END_SCENE - game->GetScreenWidth();
				}
			}
			this->cam_x = cx;
		}
	}
	else {
		//DebugOut(L"CAMMM X %f\n", CAM_X_END_SCENE);
		float cx, cy;
		Mario* player = Mario::GetInstance();
		player->GetPosition(cx, cy);
		if (cx < (game->GetScreenWidth() / 2)) {
			cx = CAM_X_MIN;
		}
		else
		{
			if (cx + game->GetScreenWidth() / 2 < CAM_X_END_SCENE) {
				cx -= game->GetScreenWidth() / 2;
			}
			else {
				cx = CAM_X_END_SCENE - game->GetScreenWidth();
			}
		}
		this->cam_x = cx;
		if (player->GetLevel() == MARIO_LEVEL_RACCOON && player->GetIsFlying()) {
			canChangeCamY = true;
			if (cy - game->GetScreenHeight() / 2 < CAM_Y_MAX) {
				if ((cy - game->GetScreenHeight() / 2) > CAM_Y_MIN) {
					cy -= game->GetScreenHeight() / 2;
				}
				else
					cy = CAM_Y_MIN;
				this->cam_y = cy;
			}
			else {
				cy = CAM_Y_DRAW_MAP;
				this->cam_y = cy;
			}
		}
		else if (canChangeCamY) {
			if (cy - game->GetScreenHeight() / 2 < CAM_Y_MAX) {
				if ((cy - game->GetScreenHeight() / 2) > CAM_Y_MIN) {
					cy -= game->GetScreenHeight() / 2;
				}
				else
					cy = CAM_Y_MIN;
				this->cam_y = cy;
			}
			else {
				cy = CAM_Y_DRAW_MAP;
				canChangeCamY = false;
				this->cam_y = cy;
			}
		}
		else {
			if (cx < (game->GetScreenWidth() / 2)) {
				cy = CAM_Y_DRAW_MAP;
				this->cam_y = cy;
			}
			if (cam_y == CAM_Y_DRAW_MAP) {
				cy = CAM_Y_DRAW_MAP;
				this->cam_y = cy;
			}
			canChangeCamY = false;
		}
	}

}

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

Camera::~Camera()
{
}
