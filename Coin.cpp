#include "Coin.h"
#include "Textures.h"
#include "Game.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x + COIN_BBOX_WIDTH / 2, y + COIN_BBOX_HEIGHT / 2);

	RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	/*l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;*/

	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}

void CCoin::RenderBoundingBox()
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