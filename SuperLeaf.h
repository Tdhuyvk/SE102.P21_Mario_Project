#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define	SUPER_LEAF_WIDTH 16
#define SUPER_LEAF_BBOX_WIDTH 16
#define SUPER_LEAF_BBOX_HEIGHT 16

#define ID_ANI_SUPER_LEAF_LEFT 14100
#define ID_ANI_SUPER_LEAF_RIGHT 14200


// define physic
#define LEAF_ACCEL_Y               0.0008f
#define LEAF_FALL_SPEED            0.0008f
#define LEAF_INIT_UP_SPEED         -0.2f
#define LEAF_MAX_HORIZONTAL_SPEED  0.05f

class CSuperLeaf : public CGameObject {
protected:
	float ax;
	float ay;

	float vx;
	float vy;
	int dir;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

public:
	CSuperLeaf(float x, float y);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
};