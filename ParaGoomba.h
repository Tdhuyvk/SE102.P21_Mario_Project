#pragma once
#include "Goomba.h"

#define PARAGOOMBA_SMALL_JUMP_SPEED	0.3f
#define PARAGOOMBA_FLY_SPEED		0.4f

#define PARAGOOMBA_BBOX_BUFFER_HEIGHT (20 - GOOMBA_BBOX_HEIGHT)

#define ID_ANI_PARAGOOMBA_WALKING_CLOSE_WING	5500
#define ID_ANI_PARAGOOMBA_SMALL_JUMP			5501
#define ID_ANI_PARAGOOMBA_FLY					5502
#define ID_ANI_PARAGOOMBA_NO_WING_WALK			5500
#define ID_ANI_PARAGOOMBA_DIE					5600

#define PARAGOOMBA_SMALL_JUMP_MAX	3

#define PARAGOOMBA_WALK_TIMEOUT	2000

#define PARAGOOMBA_STATE_SMALL_JUMP        400
#define PARAGOOMBA_STATE_FLY               500
#define PARAGOOMBA_STATE_NO_WING_WALKING   600

class CParaGoomba : public CGoomba
{
	BOOLEAN isJump;
	int numOfJumps;
	ULONGLONG walk_start;

	BOOLEAN isFlying;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	CParaGoomba(float x, float y);
	virtual void SetState(int state);
};
