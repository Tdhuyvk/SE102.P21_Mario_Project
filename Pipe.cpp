#include "Pipe.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

#include "AssetIDs.h"

// Helper to fetch sprite ID based on section and side
// section: 0 = top, 1 = body, 2 = bottom
// side: 0 = left, 1 = right

static int GetPipeSpriteId(int section, int side) {
	switch (section) {
	case 0: // top
		return side == 0 ? ID_SPRITE_PIPE_TOP_LEFT : ID_SPRITE_PIPE_TOP_RIGHT;
	case 1: // body
		return side == 0 ? ID_SPRITE_PIPE_BODY_LEFT : ID_SPRITE_PIPE_BODY_RIGHT;
	case 2: // bottom
		return side == 0 ? ID_SPRITE_PIPE_BOTTOM_LEFT : ID_SPRITE_PIPE_BOTTOM_RIGHT;
	default:
		return 0;
	}
}

CPipe::CPipe(float x, float y, int height, int type) : CGameObject(x, y)
{
	this->height = height;
	this->type = type;
	// Hardcode cell size for now, should match your sprite size (pipe.txt shows 16x16)
	this->cellWidth = 32;   // 2 columns (each sprite 16px)
	this->cellHeight = 16;
}

void CPipe::RenderBoundingBox()
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)(r - l);
	rect.bottom = (int)(b - t);

	// center of bounding box
	float xx = (l + r) / 2;
	float yy = (t + b) / 2;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(
		xx - cx, yy - cy,
		CTextures::GetInstance()->Get(ID_TEX_BBOX),
		nullptr, BBOX_ALPHA,
		rect.right, rect.bottom
	);
}

void CPipe::Render()
{
	// Pipe draws from top to bottom
	// If type == 0: Top-down (top cap, then body cells)
	// If type == 1: Bottom-up (body cells, then bottom cap)

	CSprites* sprites = CSprites::GetInstance();

	float offsetX_left = 8;  // 16/2, center of left sprite
	float offsetX_right = 24; // 16 (left) + 8 (right center)
	float offsetY_start = this->cellHeight / 2;

	if (type == 0)
	{
		// Top cap
		for (int side = 0; side < 2; ++side) {
			int spriteId = GetPipeSpriteId(0, side);
			float xpos = (side == 0) ? (x + offsetX_left) : (x + offsetX_right);
			sprites->Get(spriteId)->Draw(xpos, y + offsetY_start);
		}

		// Body
		for (int i = 1; i < height; ++i) {
			for (int side = 0; side < 2; ++side) {
				int spriteId = GetPipeSpriteId(1, side);
				float xpos = (side == 0) ? (x + offsetX_left) : (x + offsetX_right);
				sprites->Get(spriteId)->Draw(xpos, y + offsetY_start + i * cellHeight);
			}
		}
	}
	else if (type == 1)
	{
		// Body
		for (int i = 0; i < height - 1; ++i) {
			for (int side = 0; side < 2; ++side) {
				int spriteId = GetPipeSpriteId(1, side);
				float xpos = (side == 0) ? (x + offsetX_left) : (x + offsetX_right);
				sprites->Get(spriteId)->Draw(xpos, y + offsetY_start + i * cellHeight);
			}
		}

		// Bottom cap
		for (int side = 0; side < 2; ++side) {
			int spriteId = GetPipeSpriteId(2, side);
			float xpos = (side == 0) ? (x + offsetX_left) : (x + offsetX_right);
			sprites->Get(spriteId)->Draw(xpos, y + offsetY_start + (height - 1) * cellHeight);
		}
	}

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + cellWidth;
	b = y + cellHeight * height;
}

int CPipe::IsDirectionColliable(float nx, float ny)
{
	return 1;
}