#include "ParaKoopas.h"

CParaKoopas::CParaKoopas(float x, float y, BOOLEAN block) : CKoopas(x, y, PARA_KOOPAS_TYPE_GREEN)
{
    SetState(PARA_KOOPAS_STATE_FLY);
}

void CParaKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CKoopas::Update(dt, coObjects);

    if (!isActived) return;

    if (state == PARA_KOOPAS_STATE_FLY)
    {
        if (isOnPlatform)
        {
            SetState(PARA_KOOPAS_STATE_FLY);
        }
    }
    isOnPlatform = false;
}

void CParaKoopas::Render()
{
    if (!isActived) return;

    int aniId = -1;
    if (state == PARA_KOOPAS_STATE_FLY)
    {
        aniId = (vx < 0) ? ID_ANI_KARA_KOOPAS_GREEN_WALKING_LEFT : ID_ANI_KARA_KOOPAS_GREEN_WALKING_RIGHT;
    }
    else
    {
        switch (state)
        {
        case KOOPAS_STATE_DIE:
            aniId = ID_ANI_KOOPAS_GREEN_SHELL;
            break;
        case KOOPAS_STATE_SHELL_MOVING:
            aniId = ID_ANI_KOOPAS_GREEN_SHELL_MOVING;
            break;
        case KOOPAS_STATE_REVIVING:
            aniId = ID_ANI_KOOPAS_GREEN_REVIVING;
            break;
        case KOOPAS_STATE_BEING_HELD:
            aniId = ID_ANI_KOOPAS_GREEN_SHELL;
            break;
        default:
            aniId = (nx > 0) ? ID_ANI_KOOPAS_GREEN_WALKING_RIGHT : ID_ANI_KOOPAS_GREEN_WALKING_LEFT;
            break;
        }
    }

    CAnimations* animations = CAnimations::GetInstance();
    LPANIMATION animation = animations->Get(aniId);

    if (animation == nullptr)
    {
        animation = animations->Get(ID_ANI_KOOPAS_GREEN_WALKING_LEFT);
    }

    animation->Render(x, y);
    RenderBoundingBox();
}

void CParaKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    int bboxHeight;
    if (state == PARA_KOOPAS_STATE_FLY || state == KOOPAS_STATE_WALKING)
        bboxHeight = KOOPAS_BBOX_HEIGHT;
    else
        bboxHeight = KOOPAS_BBOX_SHELL_HEIGHT;
    left = x - KOOPAS_BBOX_WIDTH / 2;
    top = y - bboxHeight / 2;
    right = left + KOOPAS_BBOX_WIDTH;
    bottom = top + bboxHeight;
}

void CParaKoopas::SetState(int state)
{
    switch (state)
    {
    case PARA_KOOPAS_STATE_FLY:
        vy = -PARA_KOOPAS_FLY_SPEED;
        vx = nx * KOOPAS_WALKING_SPEED;
        isOnPlatform = false;
        isBeingHeld = false;
        break;
    }
    CKoopas::SetState(state);
}