#pragma once
#include "GameObject.h"
#include "Mario.h"

#define VENUS_RED_BBOX_WIDTH 16
#define VENUS_RED_BBOX_HEIGHT 32

#define VENUS_GREEN_BBOX_WIDTH 16
#define VENUS_GREEN_BBOX_HEIGHT 24

#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_HEIGHT 24


#define VENUS_STATE_MOVING_UP 100
#define VENUS_STATE_MOVING_DOWN 200
#define VENUS_STATE_AIMING 300
#define VENUS_STATE_FIRE 400

//#define PIRANHA_MOVING 500

#define VENUS_STATE_DIE         500

#define VENUS_TYPE_RED 1
#define VENUS_TYPE_GREEN 2
#define PIRANHA_TYPE 3

#define ID_ANI_VENUS_RED_LEFT_DOWN		15111
#define ID_ANI_VENUS_RED_LEFT_UP		15112
#define ID_ANI_VENUS_RED_RIGHT_DOWN		15121
#define ID_ANI_VENUS_RED_RIGHT_UP		15122

#define ID_ANI_VENUS_GREEN_LEFT_DOWN	15211
#define ID_ANI_VENUS_GREEN_LEFT_UP		15212
#define ID_ANI_VENUS_GREEN_RIGHT_DOWN	15221
#define ID_ANI_VENUS_GREEN_RIGHT_UP		15222

#define ID_ANI_PIRANHA_MOVING			15300

// Venus/Piranha logic constants
#define VENUS_MOVE_SPEED      0.07f
#define VENUS_FIRE_NEAR       0.045f
#define VENUS_FIRE_FAR        0.02f
#define VENUS_FIRE_RANGE      72
#define VENUS_WAIT_TIME       1500
#define VENUS_SLEEP_TIME      3000

// Active/trigger system
#define TRIGGER_READY   0
#define TRIGGER_ACTIVE  1
#define TRIGGER_IGNORE  2

class CVenusPiranha : public CGameObject
{
protected:
    float hidden_y, grow_y;         // y
    ULONGLONG wait_start;           // wait at aim
    ULONGLONG sleep_start;          // sleep
    int type;                       // type: Venus red, Venus green, Piranha green

public:
    bool isActived;                 //

    CVenusPiranha(float x, float y, int type)
        : CGameObject(x, y)
    {
        this->type = type;
        this->state = VENUS_STATE_MOVING_DOWN; // start at hidden
        this->vx = 0;
        this->vy = 0;
        wait_start = -1;
        sleep_start = -1;
        isActived = false;

        // grow_y based on type
        if (type == VENUS_TYPE_RED) {
            hidden_y = y;
            grow_y = y - VENUS_RED_BBOX_HEIGHT;
        }
        else if (type == VENUS_TYPE_GREEN) {
            hidden_y = y;
            grow_y = y - VENUS_GREEN_BBOX_HEIGHT;
        }
        else { // PIRANHA_TYPE
            hidden_y = y;
            grow_y = y - PIRANHA_BBOX_HEIGHT;
        }
    }

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    virtual void Render() override;
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    virtual void SetState(int state) override;

    void StateHandler();
    void Reset();
    int IsBlocking() override { return 0; }
};