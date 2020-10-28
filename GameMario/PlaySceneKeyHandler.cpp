#include "PlaySceneKeyHandler.h"
#include "Mario.h"
#include "PlayScene.h"
void PlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	Mario* mario = Mario::GetInstance();
	switch (KeyCode)
	{
	case DIK_R:
		mario->Reset();
		break;
	default:
		mario->GetPlayerState()->OnKeyDown(KeyCode);
		break;
	}
}

void PlaySceneKeyHandler::KeyState(BYTE* states)
{
	Mario* mario = Mario::GetInstance();
	mario->GetPlayerState()->KeyState(states);

}
void PlaySceneKeyHandler::OnKeyUp(int KeyCode) {
	Mario* mario = Mario::GetInstance();
	mario->GetPlayerState()->OnKeyUp(KeyCode);
}