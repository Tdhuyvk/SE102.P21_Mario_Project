#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"
#include "Brick.h"
#include "Block.h"
#include "Platform.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f

// define shell
#define	KOOPAS_SHELL_SPEED 0.2f
#define KOOPAS_SHELL_DELECT_SPEED 0.3f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_BBOX_SHELL_HEIGHT 16

#define KOOPAS_DIE_TIMEOUT 500
#define KOOPAS_SHELL_TIMEOUT 5000
#define KOOPAS_REVIVE_TIMEOUT 1000
#define KOOPAS_BEING_HELD_TIMEOUT 10000

// define Koopas state
#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_SHELL_MOVING 300
#define KOOPAS_STATE_REVIVING 400
#define KOOPAS_STATE_BEING_HELD 500

// define Koopas type
#define KOOPAS_TYPE_GREEN 1
#define KOOPAS_TYPE_RED 2

// Animation IDs
// Koopas
#define ID_ANI_KOOPAS_WALKING 6000
#define ID_ANI_KOOPAS_DIE 6001

#define ID_ANI_KOOPAS_WALKING_LEFT 6002
#define ID_ANI_KOOPAS_WALKING_RIGHT 6003
#define ID_ANI_KOOPAS_SHELL 6004
#define ID_ANI_KOOPAS_SHELL_MOVING 6005
#define ID_ANI_KOOPAS_REVIVING 6006

// red Koopa
#define ID_ANI_RED_KOOPAS_WALKING 6100
#define ID_ANI_RED_KOOPAS_WALKING_LEFT 6102
#define ID_ANI_RED_KOOPAS_WALKING_RIGHT 6103
#define ID_ANI_RED_KOOPAS_SHELL 6104
#define ID_ANI_RED_KOOPAS_SHELL_MOVING 6105
#define ID_ANI_RED_KOOPAS_REVIVING 6106

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG shell_start;
	ULONGLONG revive_start;
	ULONGLONG hold_start;

	int type;
	bool isBeingHeld;
	CMario* holdingMario;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != KOOPAS_STATE_BEING_HELD; } // check state
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	// collision with others
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	
	void HandleRedKoopaTurnaround();

public:
	CKoopas(float x, float y, int type = KOOPAS_TYPE_GREEN);
	virtual void SetState(int state);

	bool IsInShell() { return (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_SHELL_MOVING); }
	bool IsBeingHeld() { return isBeingHeld; }
	void SetBeingHeld(bool held) { isBeingHeld = held; }
	void SetHoldingMario(CMario* mario) { holdingMario = mario; }
	void Release();
	int getKoopasType() { return type; }
	float GetVy() { return vy; }
};