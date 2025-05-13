#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "Animation.h"

#define ID_ANI_BLOCK_QUESTION 12000
#define ID_ANI_BLOCK_EMPTY    12001

#define BLOCK_BBOX_WIDTH  16
#define BLOCK_BBOX_HEIGHT 16

class CBlock : public CGameObject {
private:
    bool used;  // false = ?, true = empty
public:
    CBlock(float x, float y) : CGameObject(x, y) { used = false; }
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) { }
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void OnCollisionWith(LPCOLLISIONEVENT e);
};