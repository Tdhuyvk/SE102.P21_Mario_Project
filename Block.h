#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_Q_BLOCK 11000 // Question (?) block
#define ID_ANI_EMTY_BLOCK 11001

// State of block
#define BLOCK_STATE_IDLE 100
#define BLOCK_STATE_COLLIDED_BELOW 200
#define BLOCK_STATE_EMPTY 300

#define	BLOCK_WIDTH 16
#define BLOCK_BBOX_WIDTH 16
#define BLOCK_BBOX_HEIGHT 16

// Item
#define CONTAIN_COIN 0
#define CONTAIN_1UP_MUSHROOM 1
#define CONTAIN_SUPER_LEAF 2

class CBlock : public CGameObject {
protected:
	float init_x, init_y; // initial position
	int contain_type; // item type
	CGameObject* item;

public:
	CBlock(float x, float y, int contain_type, CGameObject* item) : CGameObject(x, y) {
		// set state
		this->state = BLOCK_STATE_IDLE;
		this->init_x = x;
		this->init_y = y;
		this->contain_type = contain_type;
		this->item = item;
	}
	CBlock(float x, float y) : CGameObject(x, y) {
		this->item = nullptr;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	//void GetState(int state);
};