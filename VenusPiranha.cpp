#include "VenusPiranha.h"
#include "PlayScene.h"
#include "FireBullet.h"
#include "debug.h"
#include "Game.h"

void CVenusPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGame* game = CGame::GetInstance();
    CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

    // active based off cam
    if (!game->IsCamEnter(def_x, def_y)) {
        if (!isActived)
        {
            trigger = TRIGGER_READY;
        }
    }
    else if (trigger == TRIGGER_READY)
    {
        trigger = TRIGGER_ACTIVE;
    }

    if (!isActived && trigger == TRIGGER_ACTIVE)
    {
        isActived = true;
        trigger = TRIGGER_IGNORE;
    }

    // skip update if not active
    if (!isActived || state == VENUS_STATE_DIE) return;

    // Venus/Piranha logic
    y += vy * dt;

    StateHandler();

    // up/down, fire, sleep
    if (state == VENUS_STATE_MOVING_DOWN) {
        if (sleep_start == -1)
            sleep_start = GetTickCount64();
        if (GetTickCount64() - sleep_start > VENUS_SLEEP_TIME) {
            sleep_start = -1;
            SetState(VENUS_STATE_MOVING_UP);
        }
    }
    else if (state == VENUS_STATE_AIMING) {
        if (wait_start == -1)
            wait_start = GetTickCount64();
        if (GetTickCount64() - wait_start > VENUS_WAIT_TIME) {
            wait_start = -1;
            SetState(VENUS_STATE_FIRE);
        }
    }
    else if (state == VENUS_STATE_FIRE) {
        // Venus Red/Green fire
        if (type == VENUS_TYPE_RED || type == VENUS_TYPE_GREEN)
        {
            CMario* player = (CMario*)scene->GetPlayer();
            float px, py;
            player->GetPosition(px, py);

            bool face_left = px <= this->x;
            bool face_up = py <= this->y;

            float vx = (face_left) ? -1 : 1;
            float vy = (face_up) ? -1 : 1;

            if ((face_left && px >= this->x - VENUS_FIRE_RANGE) ||
                (!face_left && px <= this->x + VENUS_FIRE_RANGE))
            {
                vx *= VENUS_FIRE_NEAR;
                vy *= VENUS_FIRE_NEAR;
            }
            else
            {
                vx *= VENUS_FIRE_NEAR;
                vy *= VENUS_FIRE_FAR;
            }
            CFireBullet* fire = new CFireBullet(x, y, vx, vy);
            scene->SpawnObject(fire);
        }
        SetState(VENUS_STATE_MOVING_DOWN);
    }
}

void CVenusPiranha::Reset()
{
    this->vx = 0;
    this->vy = 0;
    wait_start = -1;
    sleep_start = -1;
    isActived = false;
    trigger = TRIGGER_READY;
    x = def_x;
    y = def_y;
}

void CVenusPiranha::StateHandler()
{
    if (y <= grow_y && state == VENUS_STATE_MOVING_UP)
    {
        y = grow_y;
        vy = 0;
        SetState(VENUS_STATE_AIMING);
    }
    else if (y >= hidden_y && state == VENUS_STATE_MOVING_DOWN)
    {
        y = hidden_y;
        vy = 0;
        SetState(VENUS_STATE_MOVING_DOWN); // reset at sleep
    }
}

void CVenusPiranha::Render()
{
    // skip render if not active
    if (!isActived) return;

    int aniID = -1;

    if (type == VENUS_TYPE_RED || type == VENUS_TYPE_GREEN)
    {
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        CMario* player = (CMario*)scene->GetPlayer();
        float px, py;
        player->GetPosition(px, py);

        bool face_left = px <= this->x;
        bool face_up = py <= this->y;

        if (type == VENUS_TYPE_RED)
        {
            if (face_left)
                aniID = (face_up) ? ID_ANI_VENUS_RED_LEFT_UP : ID_ANI_VENUS_RED_LEFT_DOWN;
            else
                aniID = (face_up) ? ID_ANI_VENUS_RED_RIGHT_UP : ID_ANI_VENUS_RED_RIGHT_DOWN;
        }
        else if (type == VENUS_TYPE_GREEN)
        {
            if (face_left)
                aniID = (face_up) ? ID_ANI_VENUS_GREEN_LEFT_UP : ID_ANI_VENUS_GREEN_LEFT_DOWN;
            else
                aniID = (face_up) ? ID_ANI_VENUS_GREEN_RIGHT_UP : ID_ANI_VENUS_GREEN_RIGHT_DOWN;
        }
    }
    else if (type == PIRANHA_TYPE)
    {
        aniID = ID_ANI_PIRANHA_MOVING;
    }

    if (aniID != -1)
        CAnimations::GetInstance()->Get(aniID)->Render(x, y);

    //RenderBoundingBox();
}

void CVenusPiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    if (type == VENUS_TYPE_RED)
    {
        l = x - VENUS_RED_BBOX_WIDTH / 2;
        t = y - VENUS_RED_BBOX_HEIGHT / 2;
        r = l + VENUS_RED_BBOX_WIDTH;
        b = t + VENUS_RED_BBOX_HEIGHT;
    }
    else if (type == VENUS_TYPE_GREEN)
    {
        l = x - VENUS_GREEN_BBOX_WIDTH / 2;
        t = y - VENUS_GREEN_BBOX_HEIGHT / 2;
        r = l + VENUS_GREEN_BBOX_WIDTH;
        b = t + VENUS_GREEN_BBOX_HEIGHT;
    }
    else // PIRANHA_TYPE
    {
        l = x - PIRANHA_BBOX_WIDTH / 2;
        t = y - PIRANHA_BBOX_HEIGHT / 2;
        r = l + PIRANHA_BBOX_WIDTH;
        b = t + PIRANHA_BBOX_HEIGHT;
    }
}

void CVenusPiranha::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case VENUS_STATE_MOVING_UP:
        vy = -VENUS_MOVE_SPEED;
        break;
    case VENUS_STATE_MOVING_DOWN:
        vy = VENUS_MOVE_SPEED;
        sleep_start = -1;
        break;
    case VENUS_STATE_AIMING:
        vy = 0;
        wait_start = -1;
        break;
    case VENUS_STATE_FIRE:
        vy = 0;
        break;
    case VENUS_STATE_DIE:
        isDeleted = true;
        break;
    }
}