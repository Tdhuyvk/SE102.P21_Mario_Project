#include "Mushroom.h"
#include "Textures.h"
#include "Game.h"
#include "AssetIDs.h"

CMushroom::CMushroom(float x, float y) :CGameObject(x, y)
{
    this->ax = 0;
    this->ay = MUSHROOM_GRAVITY;

    vy = 0.0f;
    nx = -1;
    vx = 0;
}

void CMushroom::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    
    if (dynamic_cast<CMushroom*>(e->obj)) return;

    if (e->ny != 0)
    {
        vy = 0;

        vx = MUSHROOM_WALKING_SPEED * nx;
    }
    else if (e->nx != 0)
    {
        nx = -nx;
        vx = MUSHROOM_WALKING_SPEED * nx;
    }
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    CSprites::GetInstance()->Get(ID_SPRITE_MUSHROOM)->Draw(x + MUSHROOM_WIDTH / 2, y + MUSHROOM_WIDTH / 2);

    RenderBoundingBox();
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + MUSHROOM_BBOX_WIDTH;
    b = y + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::RenderBoundingBox()
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