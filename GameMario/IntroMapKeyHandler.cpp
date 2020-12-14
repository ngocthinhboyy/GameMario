#include "IntroMapKeyHandler.h"
#include "game.h"
#include "IntroMap.h"
#include "Arrow.h"

void IntroMapKeyHandler::KeyState(BYTE* states)
{
}

void IntroMapKeyHandler::OnKeyDown(int KeyCode)
{
	IntroMap* introMap = dynamic_cast<IntroMap*> (Game::GetInstance()->GetCurrentScene());
	LPGAMEOBJECT arrow = NULL;
	for (auto object : introMap->objects)
	{
		if (dynamic_cast<Arrow*>(object))
			arrow = object;
	}
	switch (KeyCode)
	{
		case DIK_UP: {
			if (arrow != NULL) {
				if (arrow->y != arrow->startPositionY)
					arrow->y = arrow->startPositionY;
				else
					arrow->y = arrow->startPositionY + 48;
			}
			break;
		}
		case DIK_DOWN: {
			if (arrow != NULL) {
				if (arrow->y != arrow->startPositionY)
					arrow->y = arrow->startPositionY;
				else
					arrow->y = arrow->startPositionY + 48;
			}
			break;
		}
		case DIK_S: {
			Game::GetInstance()->SwitchScene(1);
			break;
		}
	default:
		break;
	}
}

void IntroMapKeyHandler::OnKeyUp(int KeyCode)
{
}
