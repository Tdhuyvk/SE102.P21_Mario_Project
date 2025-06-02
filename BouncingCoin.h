#pragma once
#include "Coin.h"


#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

// inheritance from coin

#define BOUNCING_COIN_BOUNCE_FORCE 0.15f
#define BOUNCING_COIN_BOUNCE_RECURE 0.00045f

class CBouncingCoin : public CCoin {
	float def_y;
	float vy;

public:
	CBouncingCoin(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	int IsCollidable() { return 0; }
	int IsBlocking() { return 0; }
};
