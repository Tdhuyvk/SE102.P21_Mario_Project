#include "Block.h"
#include "Sprites.h"
#include "Game.h"
#include "Coin.h"
#include "PlayScene.h"
#include "Mario.h"

void CBlock::Render() {
    CAnimations* animations = CAnimations::GetInstance();
    if (!used)
        animations->Get(ID_ANI_BLOCK_QUESTION)->Render(x + BLOCK_BBOX_WIDTH / 2, y + BLOCK_BBOX_HEIGHT / 2);
    else
        animations->Get(ID_ANI_BLOCK_EMPTY)->Render(x + BLOCK_BBOX_WIDTH / 2, y + BLOCK_BBOX_HEIGHT / 2);
    RenderBoundingBox();
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x;
    t = y;
    r = x + BLOCK_BBOX_WIDTH;
    b = y + BLOCK_BBOX_HEIGHT;
}

void CBlock::RenderBoundingBox()
{
    // get bounding box
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    // RECT (r-l) x (b-t)
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)(r - l);
    rect.bottom = (int)(b - t);

    // center bounding box
    float xx = (l + r) / 2;
    float yy = (t + b) / 2;

    // get camera
    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    // draw bounding box with (centerX, centerY)
    CGame::GetInstance()->Draw(xx - cx, yy - cy,
        CTextures::GetInstance()->Get(ID_TEX_BBOX),
        nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}

void CBlock::Update(DWORD dt, vector<LPGAMEOBJECT>*coObjects) {
    CGameObject::Update(dt);
    if (coObjects) {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }

}

void CBlock::OnCollisionWith(LPCOLLISIONEVENT e) {
    DebugOut(L"[BLOCK] Collision! ny = %.2f\n", e->ny);

    if (used || e->ny <= 0) return;

    CMario* mario = dynamic_cast<CMario*>(e->obj);
    if (mario) {
        used = true;
        DebugOut(L"[BLOCK] Activated by Mario!\n");

        CCoin* coin = new CCoin(x, y - 16);
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene) scene->AddObject(coin);
    }
}