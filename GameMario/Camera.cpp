#include "Camera.h"
#include <stdio.h>
#include "Mario.h"
#include "game.h"
#include "PlayScene.h"
#include "debug.h"

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

void Camera::UpdateCamPos()
{
	float cx, cy;
	Game* game = Game::GetInstance();
	PlayScene* playScene = dynamic_cast<PlayScene*> (game->GetCurrentScene());
	Mario* player = playScene->GetPlayer();
	player->GetPosition(cx, cy);


	if (cx < (game->GetScreenWidth() / 2)) {
		cx = 0.0f;
		//cy = CAM_Y_DRAW_MAP;
	}
	else
	{
		if (cx + game->GetScreenWidth() / 2 < 8448) {
			cx -= game->GetScreenWidth() / 2;
			//cy = CAM_Y_DRAW_MAP;
		}
		else {
			cx = 8448 - game->GetScreenWidth();
			//cy = CAM_Y_DRAW_MAP;
		}
	}
	//750
	if (player->GetLevel() == MARIO_LEVEL_RACCOON && player->GetIsFlying()) {
		canChangeCamY = true;
		if (cy - game->GetScreenHeight()/2 < 720.0f) {
			if ((cy - game->GetScreenHeight() / 2) > 145) {
				cy -= game->GetScreenHeight() / 2;
			}
			else
				cy = 145;
		}
		else {
			cy = CAM_Y_DRAW_MAP;
		}
	}
	else if (canChangeCamY) {
		if ((cy - game->GetScreenHeight() / 2) < 720) {
			if ((cy - game->GetScreenHeight() / 2) > 145) {
				cy -= game->GetScreenHeight() / 2;
			}
			else
				cy = 145;
		}
		else {
			canChangeCamY = false;
		}
	}
	else {
		cy = CAM_Y_DRAW_MAP;
	}
	this->cam_x = cx;
	this->cam_y = cy;

}

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

Camera::~Camera()
{
}
