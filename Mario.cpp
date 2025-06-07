#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

// include Koopas.h
#include "Koopas.h"

// include Mushroom.h
#include "Mushroom.h"

// include SuperLeaf.h
#include "SuperLeaf.h"

// include BouncingCoin.h
#include "BouncingCoin.h"

// include Block.h
#include "Block.h"

// include Brick.h
#include "Brick.h"

// include ParaKoopas.h
#include "ParaKoopas.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// handle shell holding
	if (isHolding && heldKoopas == NULL) {
		isHolding = false;
	}

	// reset isKicking after 0.3s
	if (isKicking && GetTickCount64() - kick_start > 300) {
		isKicking = false;
	}

	// reset isTailAttacking after MARIO_TAIL_ATTACK_TIME
	if (isTailAttacking && GetTickCount64() - tail_attack_start > MARIO_TAIL_ATTACK_TIME) {
		isTailAttacking = false;
	}

	// Extended shell pickup range - scan for nearby shells if not already holding one
	if (!isHolding && state == MARIO_STATE_HOLD && coObjects != NULL) {
		for (size_t i = 0; i < coObjects->size(); i++) {
			CKoopas* koopa = dynamic_cast<CKoopas*>(coObjects->at(i));
			if (koopa && koopa->IsInShell() && !koopa->IsBeingHeld()) {
				// Check if shell is within extended pickup range
				if (IsShellWithinPickupRange(koopa)) {
					DebugOut(L"Extended pickup range: Found shell to pick up!\n");
					HoldKoopas(koopa);
					break; // Only pick up one shell at a time
				}
			}
		}
	}

	// Flying height tracking for Raccoon Mario camera system
	if (level == MARIO_LEVEL_RACCOON) {
		// Track the highest point reached during flight/jumping for camera tracking
		if (isFlying || (!isOnPlatform && vy < 0)) {
			// Update flyingMaxHeight to track the highest point reached
			if (y < flyingMaxHeight || flyingMaxHeight == 0.0f) {
				flyingMaxHeight = y;
			}
		}
		// Reset flying height when Mario returns to ground level naturally
		else if (isOnPlatform && y > flyingMaxHeight + 40.0f) {
			flyingMaxHeight = y;  // Reset to current ground level
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) {
			isOnPlatform = true;

			// Reset flying state when Mario lands on a platform
			isFlying = false;
		}
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CKoopas*>(e->obj)) // collision with koopas
		OnCollisionWithKoopas(e);
	//else if (dynamic_cast<CParaKoopas*>(e->obj)) // collision with para koopas
	//	OnCollisionWithParaKoopas(e);
	else if (dynamic_cast<CMushroom*>(e->obj)) // collision with mushroom
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CSuperLeaf*>(e->obj))
		OnCollisionWithSuperLeaf(e);
	else if (dynamic_cast<CBlock*>(e->obj)) // collision with block
		OnCollisionWithBlock(e);
	else if (dynamic_cast<CBrick*>(e->obj)) // collision with brick
		OnCollisionWithBrick(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (isTailAttacking && level == MARIO_LEVEL_RACCOON)
				{
					// Hit with tail attack
					goomba->SetState(GOOMBA_STATE_DIE);
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (isTailAttacking && level == MARIO_LEVEL_RACCOON)
	{
		brick->SetState(BRICK_STATE_BROKEN);
	}

	if ((e->ny > 0 && level == MARIO_LEVEL_RACCOON) || (e->ny > 0 && level == MARIO_LEVEL_BIG)) {
		brick->SetState(BRICK_STATE_BROKEN);
	}
}

void CMario::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	CBlock* block = dynamic_cast<CBlock*>(e->obj);
	if (!block) return;

	// Mario jump from below
	if (e->ny > 0)
	{
		if (block->GetState() == BLOCK_STATE_QUESTION)
		{
			block->SetState(BLOCK_STATE_EMPTY);
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (block->GetType() == 0 && scene) // coin
			{
				CBouncingCoin* bouncing_coin = new CBouncingCoin(block->GetX(), block->GetY() - 16);
				scene->AddObject(bouncing_coin);
				coin++;
			}
			else if (block->GetType() == 1 && scene) // mushroom
			{
				CMushroom* mushroom = new CMushroom(block->GetX(), block->GetY() - 32);
				scene->AddObject(mushroom);
			}
			else if (block->GetType() == 2 && scene) // superleaf
			{
				CSuperLeaf* superleaf = new CSuperLeaf(block->GetX(), block->GetY() - 16);
				scene->AddObject(superleaf);
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);

	// Jump on top >> convert Koopa to shell or interact with shell
	if (e->ny < 0)
	{
		if (dynamic_cast<CParaKoopas*>(e->obj))
		{
			if (koopa->GetState() == PARA_KOOPAS_STATE_FLY)
			{
				koopa->SetState(KOOPAS_STATE_WALKING);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				return;
			}
		}

		if (koopa->GetState() == KOOPAS_STATE_WALKING)
		{
			// Convert to shell
			koopa->SetState(KOOPAS_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPAS_STATE_DIE) // Shell not moving
		{
			// Kick the shell
			koopa->SetNx(this->nx); // Set direction based on Mario's direction
			koopa->SetState(KOOPAS_STATE_SHELL_MOVING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPAS_STATE_SHELL_MOVING) // Shell is moving
		{
			// Stop the shell
			koopa->SetState(KOOPAS_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopa->GetState() == KOOPAS_STATE_REVIVING) // In the process of revival
		{
			// Stop the revival and convert back to shell
			koopa->SetState(KOOPAS_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // Side collision with Koopa or shell
	{
		if (koopa->GetState() == KOOPAS_STATE_WALKING) // Hit by walking Koopa
		{
			if (untouchable == 0)
			{
				if (isTailAttacking && level == MARIO_LEVEL_RACCOON)
				{
					// Hit with tail attack - convert to shell
					koopa->SetState(KOOPAS_STATE_DIE);
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
		else if (koopa->GetState() == KOOPAS_STATE_DIE) // Stationary shell
		{
			// Check if running button is held
			if (isHolding)
			{
				// Pickup and hold the shell
				HoldKoopas(koopa);
			}
			else
			{
				// Kick the shell
				koopa->SetNx(this->nx); // Set direction based on Mario's direction
				koopa->SetState(KOOPAS_STATE_SHELL_MOVING);
			}

			if (isTailAttacking && level == MARIO_LEVEL_RACCOON)
			{
				// Hit with tail attack - kick the shell
				koopa->SetNx(this->nx);
				koopa->SetState(KOOPAS_STATE_SHELL_MOVING);
			}
			else if (state == MARIO_STATE_HOLD)
			{
				// User is pressing the hold key and collided with shell - pick it up
				DebugOut(L"Mario picking up Koopa shell on collision\n");
				HoldKoopas(koopa);
			}
		}
		else if (koopa->GetState() == KOOPAS_STATE_SHELL_MOVING) // Moving shell
		{
			if (untouchable == 0)
			{
				if (isTailAttacking && level == MARIO_LEVEL_RACCOON)
				{
					// Hit with tail attack - stop the shell
					koopa->SetState(KOOPAS_STATE_DIE);
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
		else if (koopa->GetState() == KOOPAS_STATE_REVIVING) // Reviving shell
		{
			// Same as stationary shell
			//if (isHolding)
			if (state == MARIO_STATE_HOLD)
			{
				// Pickup and hold the shell - this interrupts revival
				koopa->SetState(KOOPAS_STATE_DIE); // Reset to shell state first
				HoldKoopas(koopa);
			}
			else
			{
				// Kick the shell
				koopa->SetNx(this->nx); // Set direction based on Mario's direction
				koopa->SetState(KOOPAS_STATE_SHELL_MOVING);
			}
		}
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{

	if (level < MARIO_LEVEL_BIG) {
		SetLevel(MARIO_LEVEL_BIG);
		StartUntouchable();
	}
	e->obj->Delete();
}

void CMario::OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e)
{
	if (level < MARIO_LEVEL_RACCOON) {
		SetLevel(MARIO_LEVEL_RACCOON);
		StartUntouchable();
	}
	e->obj->Delete();
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

//
// Get animation ID for raccoon Mario
//
int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	if (isTailAttacking)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_TAIL_ATTACK_RIGHT : ID_ANI_MARIO_RACCOON_TAIL_ATTACK_LEFT;
	}
	else if (!isOnPlatform)
	{
		if (isHolding)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_HOLD_RIGHT_JUMP : ID_ANI_MARIO_RACCOON_HOLD_LEFT_JUMP;
		}
		//
		else if (isWagging)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_TAIL_WAGGING_RIGHT : ID_ANI_MARIO_RACCOON_TAIL_WAGGING_LEFT;
		}
		else if (isFlying)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_FLY_TAIL_WAGGING_RIGHT : ID_ANI_MARIO_RACCOON_FLY_TAIL_WAGGING_LEFT;
		}
		//
		else if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
	}
	else if (isSitting)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_SIT_RIGHT : ID_ANI_MARIO_RACCOON_SIT_LEFT;
	}
	else if (vx == 0)
	{
		if (isHolding)
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_HOLD_RIGHT_IDLE : ID_ANI_MARIO_RACCOON_HOLD_LEFT_IDLE;
		else if (isKicking)
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_KICK_RIGHT : ID_ANI_MARIO_RACCOON_KICK_LEFT;
		else
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (isHolding)
			aniId = ID_ANI_MARIO_RACCOON_HOLD_RIGHT_WALKING;
		else if (isKicking)
			aniId = ID_ANI_MARIO_RACCOON_KICK_RIGHT;
		else if (ax < 0)
			aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
		else if (ax == MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
		else if (ax == MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (isHolding)
			aniId = ID_ANI_MARIO_RACCOON_HOLD_LEFT_WALKING;
		else if (isKicking)
			aniId = ID_ANI_MARIO_RACCOON_KICK_LEFT;
		else if (ax > 0)
			aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
		else if (ax == -MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
		else if (ax == -MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
	return aniId;
}

//void CMario::Render()
//{
//	CAnimations* animations = CAnimations::GetInstance();
//	int aniId = -1;
//
//	if (state == MARIO_STATE_DIE)
//		aniId = ID_ANI_MARIO_DIE;
//	
//	// raccoon
//	else if (level == MARIO_LEVEL_RACCOON)
//		aniId = GetAniIdRaccoon();
//
//	else if (level == MARIO_LEVEL_BIG)
//		aniId = GetAniIdBig();
//	else if (level == MARIO_LEVEL_SMALL)
//		aniId = GetAniIdSmall();
//
//	animations->Get(aniId)->Render(x, y);
//
//	//RenderBoundingBox();
//	
//	DebugOutTitle(L"Coins: %d", coin);
//}

void CMario::Render()
{
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon();

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();    // Visualize shell pickup range when F key is pressed (state == MARIO_STATE_HOLD)
	if (state == MARIO_STATE_HOLD && !isHolding) {
		// Draw visual indicators for shells in pickup range
		LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT>& objects = scene->GetObjects();
		for (size_t i = 0; i < objects.size(); i++) {
			CKoopas* koopa = dynamic_cast<CKoopas*>(objects.at(i));
			if (koopa && koopa->IsInShell() && !koopa->IsBeingHeld()) {
				if (IsShellWithinPickupRange(koopa)) {
					// Draw a visual indicator above the shell
					float koopa_x = koopa->GetX();
					float koopa_y = koopa->GetY();

					// Draw a simple indicator above the shell using bounding box texture
					float cx, cy;
					CGame::GetInstance()->GetCamPos(cx, cy);

					// Draw a highlighted square above the shell
					CGame::GetInstance()->Draw(
						koopa_x - cx,
						koopa_y - 20 - cy,
						CTextures::GetInstance()->Get(ID_TEX_BBOX),
						nullptr,
						0.8f,  // Higher alpha to make it more visible
						16, 8  // Small rectangle above the shell
					);
				}
			}
		}
	}
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;

			// Set flying height tracking for Raccoon Mario
			if (level == MARIO_LEVEL_RACCOON) {
				flyingMaxHeight = y;  // Record starting height for camera tracking
			}
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
			{
				// Flying for raccoon Mario
				isFlying = true;
				vy = -MARIO_JUMP_RUN_SPEED_Y;
				flyingMaxHeight = y;  // Record starting height for camera tracking
			}
			else
			{
				// Wagging tail to slow fall
				isWagging = true;
				vy = -MARIO_JUMP_SPEED_Y / 3;
				flyingMaxHeight = y;  // Record starting height for camera tracking
			}
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;

		// Don't reset flying state immediately - let Update() method handle it
		// based on Mario's position relative to his max flying height
		isWagging = false;

		break;

	// state kick
	case MARIO_STATE_KICK:
		isKicking = true;
		kick_start = GetTickCount64();
		break;

	// state hold
	case MARIO_STATE_HOLD:
		//isHolding = true;
		
		// Don't set isHolding=true here, as this would prevent picking up shells
		// isHolding is set to true only after successfully picking up a shell in HoldKoopas()
		DebugOut(L"Mario HOLD state activated\n");

		// Check for nearby shells if Mario doesn't already hold one
		if (!isHolding) {
			// We'll scan for nearby shells in the Update method
			// This flag will be used there
		}

		break;

	case MARIO_STATE_RELEASE_HOLD:
		if (isHolding && heldKoopas != NULL) {
			ReleaseKoopas();

			// kick after release
			SetState(MARIO_STATE_KICK);
		}
		isHolding = false;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	// case tail attack
	case MARIO_STATE_TAIL_ATTACK:
		if (level == MARIO_LEVEL_RACCOON) {
			isTailAttacking = true;
			tail_attack_start = GetTickCount64();
		}
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	// raccoon mario
	if (level == MARIO_LEVEL_RACCOON)
	{
		if (isTailAttacking)
		{
			// Wider bbox when tail attacking
			if (nx > 0) {
				left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
				top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
				right = left + MARIO_RACCOON_TAIL_ATTACK_WIDTH;
				bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
			}
			else {
				right = x + MARIO_RACCOON_BBOX_WIDTH / 2;
				top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
				left = right - MARIO_RACCOON_TAIL_ATTACK_WIDTH;
				bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
			}
		}
		else
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}

	else if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	/*if (this->level == MARIO_LEVEL_SMALL)*/
	if (this->level == MARIO_LEVEL_SMALL && l != MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void CMario::HoldKoopas(CKoopas* koopa)
{
	if (isHolding || !koopa)
		return;

	if (koopa->IsInShell())
	{
		// Debug output to confirm method is called
		DebugOut(L"Mario is picking up Koopa shell at (%.2f, %.2f)\n", koopa->GetX(), koopa->GetY());

		isHolding = true;
		heldKoopas = koopa;
		heldKoopas->SetBeingHeld(true);
		heldKoopas->SetHoldingMario(this);
		heldKoopas->SetState(KOOPAS_STATE_BEING_HELD);

		// Play a sound effect here if you have one for picking up shells
		// CAudio::GetInstance()->Play("pickup_shell");
	}
	else
	{
		DebugOut(L"Cannot pick up Koopa that isn't in shell state\n");
	}
}

void CMario::ReleaseKoopas()
{
	if (!isHolding || !heldKoopas)
		return;

	// Debug output for shell release
	DebugOut(L"Mario releasing shell, direction: %d\n", nx);

	// Release the shell - this will launch it in the direction Mario is facing
	isHolding = false;

	// Pass Mario's nx to the shell's Release method which will set proper velocity
	heldKoopas->Release();
	heldKoopas = NULL;
}

bool CMario::IsShellWithinPickupRange(CKoopas* koopa)
{
	if (!koopa || !koopa->IsInShell()) return false;

	float koopa_x = koopa->GetX();
	float koopa_y = koopa->GetY();
	float mario_x = this->x;
	float mario_y = this->y;

	// Calculate distance between Mario and Koopa
	float dx = abs(mario_x - koopa_x);
	float dy = abs(mario_y - koopa_y);

	// Get Mario's bounding box width based on level
	float mario_width = 0;
	if (level == MARIO_LEVEL_SMALL)
		mario_width = MARIO_SMALL_BBOX_WIDTH;
	else if (level == MARIO_LEVEL_BIG)
		mario_width = MARIO_BIG_BBOX_WIDTH;
	else if (level == MARIO_LEVEL_RACCOON)
		mario_width = MARIO_RACCOON_BBOX_WIDTH;

	// Get Koopa's width
	float koopa_width = KOOPAS_BBOX_WIDTH;

	// Calculate horizontal distance considering half width of each object
	// plus the extended pickup range
	float pickup_distance = (mario_width + koopa_width) / 2 + MARIO_SHELL_PICKUP_RANGE;

	// Check if Koopa is within horizontal pickup range and vertically close
	// The vertical threshold is more forgiving - we use 24 pixels (approximately the height of big Mario)
	if (dx <= pickup_distance && dy <= 24) {
		DebugOut(L"Shell is within extended pickup range! Distance: %.2f (limit: %.2f)\n",
			dx, pickup_distance);
		return true;
	}

	return false;
}