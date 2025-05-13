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
    // RenderBoundingBox();
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x;
    t = y;
    r = x + BLOCK_BBOX_WIDTH;
    b = y + BLOCK_BBOX_HEIGHT;
}

void CBlock::OnCollisionWith(LPCOLLISIONEVENT e) {
    CMario* mario = dynamic_cast<CMario*>(e->obj);
    if (mario && e->ny > 0 && !used) {
        used = true;
        CCoin* coin = new CCoin(x, y - COIN_BBOX_HEIGHT);
        // add coin to object list of PlayScene
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene) {
            scene->AddObject(coin);
        }
        mario->SetState(MARIO_STATE_IDLE);
    }
}