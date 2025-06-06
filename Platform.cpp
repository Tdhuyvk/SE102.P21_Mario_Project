#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CPlatform::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;


	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float centerX = (l + r) / 2.0f;
	float centerY = (t + b) / 2.0f;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	/*float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);*/
	CGame::GetInstance()->Draw(
		centerX - cx,
		centerY - cy,
		bbox,
		nullptr,
		BBOX_ALPHA,
		rect.right,
		rect.bottom
	);
}

void CPlatform::Render()
{
	if (this->length <= 0) return; 

	float startX = x + this->cellWidth / 2.0f;
	float startY = y + this->cellHeight / 2.0f;

	//float xx = x; 
	CSprites * s = CSprites::GetInstance();

	float xx = startX;

	s->Get(this->spriteIdBegin)->Draw(xx, startY);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, startY);
		xx += this->cellWidth;
	}
	if (length>1)
		s->Get(this->spriteIdEnd)->Draw(xx, startY);

	RenderBoundingBox();
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->length * cellWidth;
	b = y + cellHeight;
}

int CPlatform::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	else return 0;
}