#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

// include Koopa
#include "Koopas.h"

//include CKoopas class
class CKoopas;

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

// define hold
#define MARIO_STATE_HOLD			700
#define MARIO_STATE_RELEASE_HOLD	701

// define kick
#define MARIO_STATE_KICK			800

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

// define big hold/kick ani
#define ID_ANI_MARIO_KICK_RIGHT 1010
#define ID_ANI_MARIO_KICK_LEFT 1011

#define ID_ANI_MARIO_HOLD_LEFT_IDLE 1020
#define ID_ANI_MARIO_HOLD_LEFT_WALKING 1021
#define ID_ANI_MARIO_HOLD_LEFT_JUMP 1030
#define ID_ANI_MARIO_HOLD_RIGHT_IDLE 1025
#define ID_ANI_MARIO_HOLD_RIGHT_WALKING 1026
#define ID_ANI_MARIO_HOLD_RIGHT_JUMP 1031

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

// define small hold/kick ani
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1610
#define ID_ANI_MARIO_SMALL_KICK_LEFT 1611

#define ID_ANI_MARIO_SMALL_HOLD_LEFT_IDLE 1620
#define ID_ANI_MARIO_SMALL_HOLD_LEFT_WALKING 1621
#define ID_ANI_MARIO_SMALL_HOLD_LEFT_JUMP 1630
#define ID_ANI_MARIO_SMALL_HOLD_RIGHT_IDLE 1625
#define ID_ANI_MARIO_SMALL_HOLD_RIGHT_WALKING 1626
#define ID_ANI_MARIO_SMALL_HOLD_RIGHT_JUMP 1631

// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 1700
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 1701
#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 1800
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 1801
#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 1900
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 1901
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2000
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 2001
#define ID_ANI_MARIO_RACCOON_JUMP_FALLING_RIGHT 2010
#define ID_ANI_MARIO_RACCOON_JUMP_FALLING_LEFT 2011
#define ID_ANI_MARIO_RACCOON_TAIL_WAGGING_RIGHT 2020
#define ID_ANI_MARIO_RACCOON_TAIL_WAGGING_LEFT 2021
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 2100
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 2101
#define ID_ANI_MARIO_RACCOON_FLY_TAIL_WAGGING_RIGHT 2110
#define ID_ANI_MARIO_RACCOON_FLY_TAIL_WAGGING_LEFT 2111
#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2200
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2201
#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2300
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 2301
#define ID_ANI_MARIO_RACCOON_KICK_RIGHT 2310
#define ID_ANI_MARIO_RACCOON_KICK_LEFT 2311
#define ID_ANI_MARIO_RACCOON_HOLD_LEFT_IDLE 2320
#define ID_ANI_MARIO_RACCOON_HOLD_LEFT_WALKING 2321
#define ID_ANI_MARIO_RACCOON_HOLD_LEFT_JUMP 2330
#define ID_ANI_MARIO_RACCOON_HOLD_RIGHT_IDLE 2325
#define ID_ANI_MARIO_RACCOON_HOLD_RIGHT_WALKING 2326
#define ID_ANI_MARIO_RACCOON_HOLD_RIGHT_JUMP 2331
#define ID_ANI_MARIO_RACCOON_TAIL_ATTACK_LEFT 2401
#define ID_ANI_MARIO_RACCOON_TAIL_ATTACK_RIGHT 2402
//#define ID_ANI_MARIO_RACCOON_PIPE	2500

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_LEVEL_RACCOON	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

// define raccoon
#define MARIO_RACCOON_BBOX_WIDTH  20
#define MARIO_RACCOON_BBOX_HEIGHT 26


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 

	// shell hold
	BOOLEAN isHolding;
	CKoopas* heldKoopas;

	// kick
	BOOLEAN isKicking;
	ULONGLONG kick_start;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);

	// collision with Koopas
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);

	// collision with Mushroom
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);

	// collision with Leaf
	void OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e);

	// collision with Block
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();

	// get raccoon
	int GetAniIdRaccoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;

		// hold shell
		isHolding = false;
		heldKoopas = NULL;

		// kick
		isKicking = false;
		kick_start = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void HoldKoopas(CKoopas* koopas);
	void ReleaseKoopas();
	bool IsHolding() { return isHolding; }
};