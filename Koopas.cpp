#include "Koopas.h"
#include "debug.h"
#include "Mario.h"
#include "Goomba.h"
#include "Brick.h"
#include "Block.h"
#include "Platform.h"
#include "PlayScene.h"
#include "Game.h"

#include "Coin.h"
#include "Mushroom.h"
#include "SuperLeaf.h"

#include "ParaKoopas.h"

CKoopas::CKoopas(float x, float y, int type) :CGameObject(x, y), type(type)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	shell_start = -1;
	revive_start = -1;
	hold_start = -1;

	last_edge_check_time = GetTickCount64();

	spawn_time = GetTickCount64();

	this->type = type;
	isBeingHeld = false;
	holdingMario = NULL;

	this->nx = -1; // Default to moving left

	// set default position
	SetDefaultPosition(x, y);
	isActived = false;
	trigger = TRIGGER_READY;

	isBlockByPlatform = (type == KOOPAS_TYPE_RED);
	if (isBlockByPlatform) {
		platformChecker = new CPlatformChecker(
			RelativedPositionOfChecker(),
			y,
			KOOPAS_BBOX_HEIGHT,
			KOOPAS_BBOX_WIDTH
		);
		platformChecker->SetVx(vx);
	}
	else platformChecker = nullptr;

	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_SHELL_MOVING || state == KOOPAS_STATE_BEING_HELD)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_SHELL_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	else if (state == KOOPAS_STATE_REVIVING)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - (KOOPAS_BBOX_SHELL_HEIGHT + (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2) / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_SHELL_HEIGHT + (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2;
	}
	else
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
	if (dynamic_cast<CMario*>(e->obj)) return;

	if (state == KOOPAS_STATE_BEING_HELD) return;

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

	//
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;

		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0)
	{
		if (state == KOOPAS_STATE_WALKING || state == KOOPAS_STATE_SHELL_MOVING || state == KOOPAS_STATE_REVIVING)
		{
			if (state == KOOPAS_STATE_SHELL_MOVING)
			{
				vx = -vx;
			}
			else
			{
				vx = -vx;
				SetNx(-GetNx());
			}
		}
	}
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
	}
}

void CKoopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);

	if (state == KOOPAS_STATE_SHELL_MOVING)
	{
		if (koopa->GetState() != KOOPAS_STATE_DIE && koopa->GetState() != KOOPAS_STATE_SHELL_MOVING)
		{
			koopa->SetState(KOOPAS_STATE_DIE);
		}
		else if (koopa->GetState() == KOOPAS_STATE_DIE)
		{
			koopa->SetState(KOOPAS_STATE_SHELL_MOVING);
			if (e->nx != 0)
				koopa->SetSpeed(-vx, koopa->GetVy());
		}
	}
}

void CKoopas::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (state == KOOPAS_STATE_SHELL_MOVING && e->nx != 0)
	{
		brick->SetState(BRICK_STATE_BROKEN);

		if (vx > 0) SetNx(1);
		else SetNx(-1);
	}
}

void CKoopas::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	// Default behavior

	CBlock* block = dynamic_cast<CBlock*>(e->obj);
	if (!block) return;

	// Koopas shell collision side
	if (e->nx != 0)
	{
		if (block->GetState() == BLOCK_STATE_QUESTION)
		{
			block->SetState(BLOCK_STATE_EMPTY);
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (block->GetType() == 0 && scene) // coin
			{
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

void CKoopas::OnCollisionWithPlatform(LPCOLLISIONEVENT e) {
	if (e->ny < 0) {
		vy = 0;
		isOnPlatform = true;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//isOnPlatform = false;

	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	// check reset if fall off screen
	if (scene->IsFallOff(y))
	{
		//Reset();
		if (dynamic_cast<CParaKoopas*>(this))
			SetState(PARA_KOOPAS_STATE_FLY);
		else
			SetState(KOOPAS_STATE_WALKING);
		return;
	}

	// active based of cam
	if (!game->IsCamEnter(def_x, def_y)) {
		if (!isActived)
		{
			trigger = TRIGGER_READY;
			// set moving direction based of cam
			if (game->IsRightSideOfCam(def_x))
				vx = -KOOPAS_WALKING_SPEED;
			else
				vx = KOOPAS_WALKING_SPEED;
		}
	}
	else if (trigger == TRIGGER_READY)
	{
		trigger = TRIGGER_ACTIVE;
	}

	// active enemy
	if (!isActived && trigger == TRIGGER_ACTIVE)
	{
		isActived = true;
		trigger = TRIGGER_IGNORE;
	}

	//
	if (state == KOOPAS_STATE_BEING_HELD || !isActived)
	{
		if (holdingMario != NULL)
		{
			float marioX, marioY;
			int marioDirection;

			marioX = holdingMario->GetX();
			marioY = holdingMario->GetY();
			marioDirection = holdingMario->GetNx();

			/*x = marioX + marioDirection * 12;
			y = marioY - 2;*/

			// Position the koopa shell relative to Mario based on Mario's level
			if (holdingMario->GetLevel() == MARIO_LEVEL_SMALL) {
				x = marioX + marioDirection * 10;
				y = marioY - 1;
			}
			else {
				x = marioX + marioDirection * 12;
				y = marioY - 2;
			}

			if (GetTickCount64() - hold_start > KOOPAS_BEING_HELD_TIMEOUT)
			{
				Release();
			}
		}
		else if (state == KOOPAS_STATE_BEING_HELD && holdingMario == NULL)
		{
			SetState(KOOPAS_STATE_DIE);
		}

		// Skip the rest of the update if not activated
		if (!isActived)
			return;
	}
	else
	{
		vy += ay * dt;
		vx += ax * dt;

		if (state == KOOPAS_STATE_DIE)
		{
			if (fabs(vx) < 0.01f && GetTickCount64() - die_start > KOOPAS_SHELL_TIMEOUT)
			{
				SetState(KOOPAS_STATE_REVIVING);
				revive_start = GetTickCount64();
			}
		}
		else if (state == KOOPAS_STATE_REVIVING)
		{
			vx = 0;
			if (GetTickCount64() - revive_start > KOOPAS_REVIVE_TIMEOUT)
			{
				if (GetNx() == 0) SetNx(-1);
				SetState(KOOPAS_STATE_WALKING);
			}
		}

		// EDGE CHECKER
		// checking platform and re direction for red koopas
		if (platformChecker) {
			platformChecker->SetPosition(RelativedPositionOfChecker(), y);
			platformChecker->SetVx(vx);
			platformChecker->Update(dt, coObjects);

			if (platformChecker->IsFallOffPlatform() &&
				isBlockByPlatform &&
				state == KOOPAS_STATE_WALKING)
			{
				vx = -vx;
				nx = -nx;
				platformChecker->SetVx(vx);
				platformChecker->SetPosition(RelativedPositionOfChecker(), y);
			}
		}
		// -----------------------------------

		CGameObject::Update(dt, coObjects);

		if (state != KOOPAS_STATE_BEING_HELD)
			CCollision::GetInstance()->Process(this, dt, coObjects);
	}

	/*vy += ay * dt;
	vx += ax * dt;

	if (!isBeingHeld)
		CCollision::GetInstance()->Process(this, dt, coObjects);*/

	
	if (type == PARA_KOOPAS_TYPE_GREEN && state == PARA_KOOPAS_STATE_FLY)
	{
		if (isOnPlatform)
		{
			vy = -PARA_KOOPAS_FLY_SPEED;
			if (vx == 0) vx = nx * KOOPAS_WALKING_SPEED;
		}
	}

	isOnPlatform = false;
}

void CKoopas::Render()
{

	if (!isActived) return;

	int aniId = -1;

	if (type == KOOPAS_TYPE_GREEN)
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

	if (type == KOOPAS_TYPE_RED)
	{
		switch (state)
		{
		case KOOPAS_STATE_DIE:
			aniId = ID_ANI_KOOPAS_RED_SHELL;
			break;
		case KOOPAS_STATE_SHELL_MOVING:
			aniId = ID_ANI_KOOPAS_RED_SHELL_MOVING;
			break;
		case KOOPAS_STATE_REVIVING:
			aniId = ID_ANI_KOOPAS_RED_REVIVING;
			break;
		case KOOPAS_STATE_BEING_HELD:
			aniId = ID_ANI_KOOPAS_RED_SHELL;
			break;
		default:
			aniId = (nx > 0)
				? ID_ANI_KOOPAS_RED_WALKING_RIGHT
				: ID_ANI_KOOPAS_RED_WALKING_LEFT;
			break;
		}
	}

	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION animation = animations->Get(aniId);

	if (animation == nullptr)
	{
		animation = animations->Get(ID_ANI_KOOPAS_RED_WALKING_LEFT);
	}

	animation->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	int prevState = this->state;
	float oldY = y;

	DebugOut(L"Koopa state change: %d -> %d at position (%.2f, %.2f)\n", prevState, state, x, y);

	CGameObject::SetState(state);

	switch (state)
	{
	case KOOPAS_STATE_DIE:
		if (prevState == KOOPAS_STATE_WALKING || prevState == KOOPAS_STATE_REVIVING)
			y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2;

		vx = 0;
		vy = 0;
		ay = 0;
		die_start = GetTickCount64();
		shell_start = GetTickCount64();
		break;
	case KOOPAS_STATE_SHELL_MOVING:
		if (GetNx() == 0)
			SetNx(1);
		vx = GetNx() * KOOPAS_SHELL_SPEED;
		ay = KOOPAS_GRAVITY;
		shell_start = -1;
		break;
	case KOOPAS_STATE_WALKING:
		if (prevState == KOOPAS_STATE_REVIVING)
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) / 2;

		if (GetNx() == 0) SetNx(-1);
		vx = GetNx() * KOOPAS_WALKING_SPEED;

		if (vx > 0) SetNx(1);
		else if (vx < 0) SetNx(-1);
		ay = KOOPAS_GRAVITY;

		spawn_time = GetTickCount64();

		break;

	case KOOPAS_STATE_REVIVING:
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

	case PARA_KOOPAS_STATE_FLY:
		
		if (type == PARA_KOOPAS_TYPE_GREEN)
		{
			vy = -PARA_KOOPAS_FLY_SPEED;
			vx = nx * KOOPAS_WALKING_SPEED;
			isBeingHeld = false;
		}
		break;
	}
}

void CKoopas::Release()
{
	if (state != KOOPAS_STATE_BEING_HELD)
		return;

	isBeingHeld = false;

	if (holdingMario)
	{
		int direction = holdingMario->GetNx();
		SetNx(direction);

		SetState(KOOPAS_STATE_SHELL_MOVING);

		vx = GetNx() * KOOPAS_SHELL_SPEED;
	}
	else
	{
		SetState(KOOPAS_STATE_DIE);
	}

	holdingMario = NULL;
}
