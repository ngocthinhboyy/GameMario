#pragma once
#define	MARIO_LEVEL_SMALL					1
#define	MARIO_LEVEL_BIG						2
#define	MARIO_LEVEL_RACCOON					3
#define	MARIO_LEVEL_FIRE					4

#define MARIO_ANI_BIG_IDLE_RIGHT			201
#define MARIO_ANI_BIG_IDLE_LEFT				202
#define MARIO_ANI_SMALL_IDLE_RIGHT			101
#define MARIO_ANI_SMALL_IDLE_LEFT			102
#define MARIO_ANI_RACCOON_IDLE_RIGHT		301
#define MARIO_ANI_RACCOON_IDLE_LEFT			302


#define MARIO_ANI_BIG_WALKING_RIGHT			203
#define MARIO_ANI_BIG_WALKING_LEFT			204
#define MARIO_ANI_SMALL_WALKING_RIGHT		103
#define MARIO_ANI_SMALL_WALKING_LEFT		104
#define MARIO_ANI_RACCOON_WALKING_RIGHT		303
#define MARIO_ANI_RACCOON_WALKING_LEFT		304

#define MARIO_ANI_BIG_JUMP_RIGHT			205
#define MARIO_ANI_BIG_JUMP_LEFT				206
#define MARIO_ANI_SMALL_JUMP_RIGHT			105
#define MARIO_ANI_SMALL_JUMP_LEFT			106
#define MARIO_ANI_RACCOON_JUMP_RIGHT		305
#define MARIO_ANI_RACCOON_JUMP_LEFT			306

#define MARIO_ANI_BIG_HIGH_JUMP_RIGHT		207
#define MARIO_ANI_BIG_HIGH_JUMP_LEFT		208
#define MARIO_ANI_SMALL_HIGH_JUMP_RIGHT		107
#define MARIO_ANI_SMALL_HIGH_JUMP_LEFT		108
#define MARIO_ANI_RACCOON_HIGH_JUMP_RIGHT	307
#define MARIO_ANI_RACCOON_HIGH_JUMP_LEFT	308

#define MARIO_ANI_SMALL_FALL				109
#define MARIO_ANI_BIG_FALL					209
#define MARIO_ANI_RACCOON_FALL				309

#define MARIO_ANI_BIG_CROUCH				210
#define MARIO_ANI_RACCOON_CROUCH			310

#define MARIO_ANI_BIG_RUN					211
#define MARIO_ANI_SMALL_RUN					111
#define MARIO_ANI_RACCOON_RUN				311

#define MARIO_ANI_BIG_SKID					212
#define MARIO_ANI_SMALL_SKID				110
#define MARIO_ANI_RACCOON_SKID				312

#define MARIO_ANI_SMALL_FLY					112
#define MARIO_ANI_BIG_FLY					213
#define MARIO_ANI_RACCOON_FLY				313

#define MARIO_ANI_RACCOON_SPIN				316

#define MARIO_ANI_FIRE_IDLE_RIGHT			401
#define MARIO_ANI_FIRE_IDLE_LEFT			402
#define MARIO_ANI_FIRE_WALKING_RIGHT		403
#define MARIO_ANI_FIRE_WALKING_LEFT			404
#define MARIO_ANI_FIRE_JUMP_RIGHT			405
#define MARIO_ANI_FIRE_JUMP_LEFT			406
#define MARIO_ANI_FIRE_HIGH_JUMP_RIGHT		407
#define MARIO_ANI_FIRE_HIGH_JUMP_LEFT		408
#define MARIO_ANI_FIRE_CROUCH				409
#define MARIO_ANI_FIRE_RUN					410
#define MARIO_ANI_FIRE_SKID					411
#define MARIO_ANI_FIRE_FLY					412
#define MARIO_ANI_FIRE_THROW_FIREBALL		413
#define MARIO_ANI_FIRE_HOLD					414
#define MARIO_ANI_FIRE_KICK					415

#define MARIO_ANI_RACCOON_HOLD				317
#define MARIO_ANI_RACCOON_KICK				318
#define MARIO_ANI_SMALL_HOLD				113
#define MARIO_ANI_SMALL_KICK				114
#define MARIO_ANI_BIG_HOLD					214
#define MARIO_ANI_BIG_KICK					215

#define MARIO_ANI_RACCOON_FALLING_SLOWLY	319


#define MARIO_SPEED_ACCELERATION			0.0002f
#define MARIO_SLOWLY_SPEED_ACCECLERATION	0.0004333f
#define MARIO_RUNNING_MAX_SPEED				0.55f

#define MARIO_WALKING_SPEED					0.2f 

#define MARIO_JUMP_SPEED_Y					0.5f


#define MARIO_JUMP_MAX_SPEED_Y				0.7f
#define MARIO_ACCELERATION_JUMP_Y		    0.0030666666f

#define MARIO_JUMP_DEFLECT_SPEED			0.2f
#define MARIO_JUMP_COLLISION_Y_WITH_ENEMY	0.4f
#define MARIO_GRAVITY						0.002f
#define MARIO_DIE_DEFLECT_SPEED				0.5f

#define MARIO_DEVIATION_CROUCHING_Y			27.0f

#define MARIO_ANI_DIE						8
#define MARIO_STATE_DIE						8


#define MARIO_BIG_BBOX_WIDTH				45
#define MARIO_BIG_BBOX_HEIGHT				81

#define MARIO_BIG_CROUCH_BBOX_WIDTH			45
#define MARIO_BIG_CROUCH_BBOX_HEIGHT		54 

#define MARIO_SMALL_BBOX_WIDTH				39
#define MARIO_SMALL_BBOX_HEIGHT				45


#define MARIO_RACCOON_BBOX_WIDTH  45
#define MARIO_RACCOON_BBOX_HEIGHT 81

#define MARIO_UNTOUCHABLE_TIME 5000

#define MARIO_BONUS_POINT_LEAF	1000
#define MARIO_BONUS_POINT_COIN	100



// effect
#define MARIO_EFFECT_HAVE_BONUS	1701