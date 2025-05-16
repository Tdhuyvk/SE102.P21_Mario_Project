#include "Koopas.h"
#include "debug.h"
#include "Mario.h"
#include "Goomba.h"
#include "Brick.h"
#include "Block.h"
#include "Platform.h"
#include "PlayScene.h"
#include "Game.h"

CKoopas::CKoopas(float x, float y, int type) :CGameObject(x, y), type(type)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	// define shell
	shell_start = -1;
	revive_start = -1;
	hold_start = -1;

	this->type = type;
	isBeingHeld = false;
	holdingMario = NULL;

	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*if (state == KOOPAS_STATE_DIE)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}*/

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_SHELL_MOVING || state == KOOPAS_STATE_BEING_HELD)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_SHELL_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	else if (state == KOOPAS_STATE_REVIVING)
	{
		// between shell and normal height (in revival)
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - (KOOPAS_BBOX_SHELL_HEIGHT + (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2) / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_SHELL_HEIGHT + (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2;
	}
	else // walking state
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	/*if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}*/

	// Skip if object is being held (Mario handles movement)
	if (state == KOOPAS_STATE_BEING_HELD) return;

	// Handle specific collision types
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	// Default collision handling
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		// Only change direction if walking or if it hit something while shell moving
		if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_SHELL_MOVING || state == KOOPAS_STATE_REVIVING)
		{
			// For shell moving, the direction change implies bouncing off obstacles
			if (state == KOOPAS_STATE_SHELL_MOVING)
			{
				vx = -vx;
			}
			// For walking or reviving state, just turn around
			else
			{
				vx = -vx;
				SetNx(-GetNx());  // Update the nx value
			}
		}
	}
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// If shell is moving, kill the Goomba
	if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		// Kill the Goomba
		goomba->SetState(GOOMBA_STATE_DIE);

		// No need to change shell direction - continue moving
		// This creates the effect of shells taking out multiple enemies in a row
	}
}

void CKoopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);

	// If shell is moving, hit the other Koopa
	if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		if (koopa->GetState() != KOOPAS_STATE_DIE && koopa->GetState() != KOOPAS_STATE_SHELL_MOVING)
		{
			koopa->SetState(KOOPAS_STATE_DIE);
		}
		else if (koopa->GetState() == KOOPAS_STATE_DIE) // If hitting a stationary shell
		{
			koopa->SetState(KOOPAS_STATE_SHELL_MOVING);
			// Set velocity direction based on collision
			if (e->nx != 0)
				koopa->SetSpeed(-vx, koopa->GetVy());
		}
	}
}

void CKoopas::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	// If shell is moving, break the brick
	if (state == KOOPAS_STATE_SHELL_MOVING && e->nx != 0)
	{
		// Break the brick when hit by moving shell
		brick->SetState(BRICK_STATE_BROKEN);

		// Optionally, you can add sound effect or visual feedback here

		// Bounce the shell slightly when breaking a brick
		if (vx > 0) SetNx(1);
		else SetNx(-1);
	}
}

void CKoopas::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	// Default behavior - handled by main collision method
}

void CKoopas::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	// For red koopas, check if they need to turn around at platform edges
	if (type == KOOPAS_TYPE_RED && state == KOOPAS_STATE_WALKING && e->ny < 0)
	{
		HandleRedKoopaTurnaround();
	}
}

void CKoopas::HandleRedKoopaTurnaround()
{
	// Only for red koopas in walking state
	if (type != KOOPAS_TYPE_RED || state != KOOPAS_STATE_WALKING)
		return;

	// Get current position
	float left, top, right, bottom;
	GetBoundingBox(left, top, right, bottom);

	// Calculate position to check (slightly ahead in movement direction)
	float ahead_x = (vx > 0) ? right + 4.0f : left - 4.0f;
	float check_y = bottom + 4.0f; // Slightly below the koopa's feet

	// Check if there's ground ahead
	// This is a simplified check. In a complete implementation, 
	// you'd want to use your collision system to check for a platform/ground
	bool has_ground_ahead = false;
	// Get all game objects and check if there's ground ahead
	vector<LPGAMEOBJECT>& objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetObjects();
	for (UINT i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT obj = objects[i];

		// Skip if not a platform or block type object
		if (!dynamic_cast<CPlatform*>(obj) && !dynamic_cast<CBlock*>(obj))
			continue;

		// Get the object's bounding box
		float oleft, otop, oright, obottom;
		obj->GetBoundingBox(oleft, otop, oright, obottom);

		// Check if the point ahead is above the platform
		if (ahead_x >= oleft && ahead_x <= oright && check_y >= otop && check_y <= obottom)
		{
			has_ground_ahead = true;
			break;
		}
	}
	// If no ground ahead, turn around
	if (!has_ground_ahead)
	{
		vx = -vx;
		SetNx(-GetNx());  // Update the nx value to reflect direction change
	}
}


void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPAS_STATE_DIE) && (GetTickCount64() - die_start > KOOPAS_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);*/

	// Being held by Mario
	if (state == KOOPAS_STATE_BEING_HELD)
	{
		if (holdingMario != NULL)
		{
			// Update position to follow Mario
			float marioX, marioY;
			int marioDirection;

			marioX = holdingMario->GetX();
			marioY = holdingMario->GetY();
			marioDirection = holdingMario->GetNx();

			// Position shell slightly in front of Mario in the direction he's facing
			x = marioX + marioDirection * 12;
			y = marioY - 2; // Slightly above Mario's center

			// Check if we've been held too long
			if (GetTickCount64() - hold_start > KOOPAS_BEING_HELD_TIMEOUT)
			{
				Release();
			}
		}
		else
		{
			// No Mario holding? Revert to shell
			SetState(KOOPAS_STATE_DIE);
		}
	}
	else
	{
		// Apply gravity if not being held
		vy += ay * dt;
		vx += ax * dt;

		// Handle shell timeout and revival
		if (state == KOOPAS_STATE_DIE)
		{
			// Only start reviving if the shell is stationary
			if (abs(vx) < 0.01f && GetTickCount64() - die_start > KOOPAS_SHELL_TIMEOUT)
			{
				// Start reviving
				SetState(KOOPAS_STATE_REVIVING);
				revive_start = GetTickCount64();
			}
		}
		else if (state == KOOPAS_STATE_REVIVING)
		{
			// During revival, the shell should stay in place
			vx = 0;

			if (GetTickCount64() - revive_start > KOOPAS_REVIVE_TIMEOUT)
			{
				// Complete revival back to walking
				SetState(KOOPAS_STATE_WALKING);
			}
		}

		// Red Koopas in walking state need special edge detection
		if (type == KOOPAS_TYPE_RED && state == KOOPAS_STATE_WALKING)
		{
			HandleRedKoopaTurnaround();
		}
	}

	CGameObject::Update(dt, coObjects);

	// Only process collisions if not being held
	if (state != KOOPAS_STATE_BEING_HELD)
		CCollision::GetInstance()->Process(this, dt, coObjects);
}


//void CKoopas::Render()
//{
//	/*int aniId = ID_ANI_KOOPAS_WALKING;*/
//	/*int aniId;
//	if (state == KOOPAS_STATE_DIE)
//	{
//		aniId = ID_ANI_KOOPAS_DIE;
//	}
//	else {
//		aniId = (vx > 0)
//			? ID_ANI_KOOPAS_WALKING_RIGHT
//			: ID_ANI_KOOPAS_WALKING_LEFT;
//	}
//
//	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
//
//	RenderBoundingBox();*/
//
//	int aniId;
//
//	// Select animation ID based on state and type
//	if (type == KOOPAS_TYPE_GREEN)
//	{
//		switch (state)
//		{
//		case KOOPAS_STATE_DIE:
//			aniId = ID_ANI_KOOPAS_SHELL;
//			break;
//		case KOOPAS_STATE_SHELL_MOVING:
//			aniId = ID_ANI_KOOPAS_SHELL_MOVING;
//			break;
//		case KOOPAS_STATE_REVIVING:
//			aniId = ID_ANI_KOOPAS_REVIVING;
//			break;
//		case KOOPAS_STATE_BEING_HELD:
//			aniId = ID_ANI_KOOPAS_SHELL;
//			break;
//		default: // Walking state
//			aniId = (vx > 0) ? ID_ANI_KOOPAS_WALKING_RIGHT : ID_ANI_KOOPAS_WALKING_LEFT;
//			break;
//		}
//	}
//	else // Red Koopas
//	{
//		switch (state)
//		{
//		case KOOPAS_STATE_DIE:
//			aniId = ID_ANI_RED_KOOPAS_SHELL;
//			break;
//		case KOOPAS_STATE_SHELL_MOVING:
//			aniId = ID_ANI_RED_KOOPAS_SHELL_MOVING;
//			break;
//		case KOOPAS_STATE_REVIVING:
//			aniId = ID_ANI_RED_KOOPAS_REVIVING;
//			break;
//		case KOOPAS_STATE_BEING_HELD:
//			aniId = ID_ANI_RED_KOOPAS_SHELL;
//			break;
//		default: // Walking state
//			aniId = (vx > 0) ? ID_ANI_RED_KOOPAS_WALKING_RIGHT : ID_ANI_RED_KOOPAS_WALKING_LEFT;
//			break;
//		}
//	}
//
//	CAnimations* animations = CAnimations::GetInstance();
//	if (animations->Get(aniId) != NULL)
//	{
//		animations->Get(aniId)->Render(x, y);
//	}
//	else
//	{
//		// Fallback animation if the specific one is not found
//		animations->Get(ID_ANI_KOOPAS_WALKING_LEFT)->Render(x, y);
//	}
//
//	RenderBoundingBox();
//}

void CKoopas::Render()
{
	int aniId = -1;

	if (type == KOOPAS_TYPE_GREEN)
	{
		switch (state)
		{
		case KOOPAS_STATE_DIE:
			aniId = ID_ANI_KOOPAS_GREEN_SHELL;          // 6104
			break;
		case KOOPAS_STATE_SHELL_MOVING:
			aniId = ID_ANI_KOOPAS_GREEN_SHELL_MOVING;   // 6105
			break;
		case KOOPAS_STATE_REVIVING:
			aniId = ID_ANI_KOOPAS_GREEN_REVIVING;        // 6106
			break;
		case KOOPAS_STATE_BEING_HELD:
			aniId = ID_ANI_KOOPAS_GREEN_SHELL;           // 6104
			break;
		default: // Walking state
			aniId = (nx > 0) ? ID_ANI_KOOPAS_GREEN_WALKING_RIGHT : ID_ANI_KOOPAS_GREEN_WALKING_LEFT; // 6103 / 6102
			break;
		}
	}
	//if (type == KOOPAS_TYPE_RED)
	//{
	//	switch (state)
	//	{
	//	case KOOPAS_STATE_DIE:
	//		aniId = ID_ANI_KOOPAS_RED_SHELL;             // 6004
	//		break;
	//	case KOOPAS_STATE_SHELL_MOVING:
	//		aniId = ID_ANI_KOOPAS_RED_SHELL_MOVING;      // 6005
	//		break;
	//	case KOOPAS_STATE_REVIVING:
	//		aniId = ID_ANI_KOOPAS_RED_REVIVING;         // 6006
	//		break;
	//	case KOOPAS_STATE_BEING_HELD:
	//		aniId = ID_ANI_KOOPAS_RED_SHELL;            // 6004
	//		break;
	//	default: // Walking state
	//		aniId = (vx > 0) ? ID_ANI_KOOPAS_RED_WALKING_RIGHT : ID_ANI_KOOPAS_RED_WALKING_LEFT; // 6003 / 6002
	//		break;
	//	}
	//}

	if (type == KOOPAS_TYPE_RED)
	{
		switch (state)
		{
		case KOOPAS_STATE_DIE:
			aniId = ID_ANI_KOOPAS_RED_SHELL;             // 6004
			break;
		case KOOPAS_STATE_SHELL_MOVING:
			aniId = ID_ANI_KOOPAS_RED_SHELL_MOVING;      // 6005
			break;
		case KOOPAS_STATE_REVIVING:
			aniId = ID_ANI_KOOPAS_RED_REVIVING;          // 6006
			break;
		case KOOPAS_STATE_BEING_HELD:
			aniId = ID_ANI_KOOPAS_RED_SHELL;            // 6004
			break;
		default: // Walking state
			aniId = (nx > 0)
				? ID_ANI_KOOPAS_RED_WALKING_RIGHT  // 6003
				: ID_ANI_KOOPAS_RED_WALKING_LEFT;  // 6002
			break;
		}
	}

	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION animation = animations->Get(aniId);

	// Fallback animation
	if (animation == nullptr)
	{
		animation = animations->Get(ID_ANI_KOOPAS_RED_WALKING_LEFT); //
	}

	animation->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	/*CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		die_start = GetTickCount64();
		y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = - KOOPAS_WALKING_SPEED;
		break;
	}*/

	// Save old state and position for adjustments
	int prevState = this->state;
	float oldY = y;

	// Log state changes for debugging
	DebugOut(L"Koopa state change: %d -> %d at position (%.2f, %.2f)\n", prevState, state, x, y);

	CGameObject::SetState(state);

	switch (state)
	{
	case KOOPAS_STATE_DIE:
		// If transitioning from walking/normal state to shell
		if (prevState == KOOPAS_STATE_WALKING || prevState == KOOPAS_STATE_REVIVING)
			y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2;

		vx = 0;
		vy = 0;
		ay = 0;
		die_start = GetTickCount64();
		shell_start = GetTickCount64();
		break;
	case KOOPAS_STATE_SHELL_MOVING:
		// Keep shell height the same as die state
		// If nx is 0, default to right direction
		if (GetNx() == 0)
			SetNx(1);
		vx = GetNx() * KOOPAS_SHELL_SPEED; // Use stored direction for movement
		ay = KOOPAS_GRAVITY;
		shell_start = -1; // Reset the shell timeout
		break;
	case KOOPAS_STATE_WALKING:
		// If coming from shell, adjust position
		if (prevState == KOOPAS_STATE_REVIVING)
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2;

		// Set initial direction for walking
		if (GetNx() == 0) SetNx(-1); // Default direction if not set
		vx = GetNx() * KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_REVIVING:
		// Start revival animation
		revive_start = GetTickCount64();
		break;

	case KOOPAS_STATE_BEING_HELD:
		isBeingHeld = true;
		hold_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ax = 0;
		ay = 0;
		break;
	}
}

void CKoopas::Release()
{
	if (state != KOOPAS_STATE_BEING_HELD)
		return;

	isBeingHeld = false;

	// Direction depends on Mario's facing direction
	if (holdingMario)
	{
		int direction = holdingMario->GetNx();
		SetNx(direction);

		// Set shell moving state which will apply the proper speed
		SetState(KOOPAS_STATE_SHELL_MOVING);

		// Make sure velocity is applied based on direction
		vx = GetNx() * KOOPAS_SHELL_SPEED;
	}
	else
	{
		// No Mario holding? Just drop as a stationary shell
		SetState(KOOPAS_STATE_DIE);
	}

	holdingMario = NULL;
}