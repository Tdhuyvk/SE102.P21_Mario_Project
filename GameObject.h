#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

// define active states
#define OBJECT_STATE_INACTIVE 0
#define OBJECT_STATE_ACTIVE   1

// define trigger state
#define TRIGGER_IGNORE -1
#define TRIGGER_READY   0
#define TRIGGER_ACTIVE  1


class CGameObject
{
protected:

	float x; 
	float y;

	float vx;
	float vy;

	int nx;	 

	int state;

	bool isDeleted;

	float def_x, def_y;       // default position
	int trigger;              // active state (TRIGGER_IGNORE, TRIGGER_READY, TRIGGER_ACTIVE)
	bool isActived;           // actived

public: 

	// 
    void SetDefaultPosition(float x, float y) { def_x = x; def_y = y; }
    float GetDefaultX() { return def_x; }
    float GetDefaultY() { return def_y; }
    
    bool IsActivated() const { return isActived; }
    void SetActivationState(bool state) { isActived = state; }
    
    int GetTriggerState() const { return trigger; }
    void SetTriggerState(int state) { trigger = state; }
    
    // check reset if needed
    virtual bool NeedReset(float fallY) { return false; }
    
    // reset to default position
    virtual void Reset() 
    { 
        x = def_x; 
        y = def_y; 
        isActived = false; 
        trigger = TRIGGER_READY; 
    }

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	float GetX() { return this->x; }
	float GetY() { return this->y; }
	int GetNx() { return this->nx; }
	void SetNx(int nx) { this->nx = nx; }

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox();

	CGameObject();
	/*CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }*/

	CGameObject(float x, float y);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// Does this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};
