#include "SuperLeaf.h"
#include "Textures.h"
#include "Game.h"
#include "AssetIDs.h"

CSuperLeaf::CSuperLeaf(float x, float y) :CGameObject(x, y)
{
    vx = 0;
    vy = LEAF_INIT_UP_SPEED;
    ay = LEAF_ACCEL_Y;
    dir = 1;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (vy <= 0)
        vy += LEAF_ACCEL_Y * dt;

    if (vy > 0) {
        vy = LEAF_FALL_SPEED * dt;

        if (dir == -1) {
            if (vx >= -LEAF_MAX_HORIZONTAL_SPEED)
                vx -= 0.0001f * dt;
            else
                dir = 1;
        }
        else if (dir == 1) {
            if (vx <= LEAF_MAX_HORIZONTAL_SPEED)
                vx += 0.0001f * dt;
            else
                dir = -1;
        }
    }

    x += vx * dt;
    y += vy * dt;
}

void CSuperLeaf::Render()
{
    if (dir == 1)
        CAnimations::GetInstance()->Get(ID_ANI_SUPER_LEAF_RIGHT)->Render(x, y);
    else
        CAnimations::GetInstance()->Get(ID_ANI_SUPER_LEAF_LEFT)->Render(x, y);

    //RenderBoundingBox();
}

void CSuperLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + SUPER_LEAF_BBOX_WIDTH;
    b = y + SUPER_LEAF_BBOX_HEIGHT;
}

void CSuperLeaf::RenderBoundingBox()
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