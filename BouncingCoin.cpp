#include "BouncingCoin.h"


CBouncingCoin::CBouncingCoin(float x, float y) : CCoin(x, y) {
    def_y = y;
    vy = -BOUNCING_COIN_BOUNCE_FORCE;
}

void CBouncingCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//
    y += vy * dt;

    if (y < def_y) {
        vy += BOUNCING_COIN_BOUNCE_RECURE * dt;
    }
    else {
        y = def_y;
        isDeleted = true;
    }
}

void CBouncingCoin::Render() {
    CCoin::Render();
}