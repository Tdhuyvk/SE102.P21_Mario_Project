#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define	MUSHROOM_WIDTH 16
#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

// define physic
#define MUSHROOM_GRAVITY 0.004f
#define MUSHROOM_WALKING_SPEED 0.1f

class CMushroom : public CGameObject {
protected:
	float ax;
	float ay;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroom(float x, float y);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
};