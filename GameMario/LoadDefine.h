#pragma once

#define MAX_CHARACTER_IN_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_RESOURCES 1
#define GAME_FILE_SECTION_SETTINGS 2
#define GAME_FILE_SECTION_SCENES 3
#define GAME_FILE_SECTION_BOARDGAME_ITEMS 4
#define GAME_FILE_SECTION_POSITION_IN_BOARDGAME 5


#define SHARED_RESOURCE_SECTION_UNKNOWN -1
#define SHARED_RESOURCE_SECTION_TEXTURES 1
#define SHARED_RESOURCE_SECTION_SPRITES 2
#define SHARED_RESOURCE_SECTION_ANIMATIONS 3
#define SHARED_RESOURCE_SECTION_ANIMATION_SETS	4
#define SHARED_RESOURCE_SECTION_OBJECTS	5
#define MAX_SHARED_RESOURCE_LINE 1024

#define BOARDGAME_SECTION_UNKNOWN -1
#define BOARDGAME_SECTION_INBOARD 1
#define BOARDGAME_SECTION_FONT 2
#define MAX_BOARDGAME_LINE 1024


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_OBJECTS_NOT_IN_GRID	8
#define SCENE_SECTION_OBJECTS_IN_GRID	9
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_REMAINING_TIME 10
#define MAX_SCENE_LINE 1024

#define OBJECT_TYPE_MARIO	0
#define	OBJECT_TYPE_COLLISION_MAP	2
#define OBJECT_TYPE_GATE	8
#define OBJECT_TYPE_GOOMBA	3
#define OBJECT_TYPE_KOOPA	4
#define OBJECT_TYPE_QUESTION_BRICK	5
#define OBJECT_TYPE_ESPECIAL_BRICK	9
#define OBJECT_TYPE_RANDOM_GIFT	10
#define OBJECT_TYPE_FLOWER	6
#define OBJECT_TYPE_COIN	7
