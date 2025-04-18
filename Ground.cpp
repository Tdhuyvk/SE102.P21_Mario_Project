#include "Ground.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
#include "AssetIDs.h"

int GetBoxSpriteId(int section, int pos)
{
    // section: 0 - head, 1 - body
    // pos: 0 - left, 1 - mid, 2 - right
    if (section == 0) // head
    {
        if (pos == 0) return ID_SPRITE_GROUND_TOP_LEFT;
        else if (pos == 1) return ID_SPRITE_GROUND_TOP_CEN;
        else if (pos == 2) return ID_SPRITE_GROUND_TOP_RIGHT;
    }
    else if (section == 1) // body
    {
        if (pos == 0) return ID_SPRITE_GROUND_MID_LEFT;
        else if (pos == 1) return ID_SPRITE_GROUND_MID_CEN;
        else if (pos == 2) return ID_SPRITE_GROUND_MID_RIGHT;
    }

    return 0;
}

void CGround::Render()
{
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    float width = r - l;
    float height = b - t;
    float centerX = l + width / 2 + 8;
    float centerY = t + height / 2 + 8;

    CSprites* sprites = CSprites::GetInstance();

    for (int row = 0; row < rows; row++)
    {
        int section = (row == 0) ? 0 : 1; // first head, else body

        for (int col = 0; col < columns; col++)
        {
            int pos;
            if (columns == 1)
                pos = 1;
            else if (col == 0)
                pos = 0;
            else if (col == columns - 1)
                pos = 2;
            else
                pos = 1;

            int spriteId = GetBoxSpriteId(section, pos);
            if (spriteId != 0)
            {
                float boxLeft = centerX - (width / 2.0f);
                float boxTop = centerY - (height / 2.0f);

                float drawX = boxLeft + col * cellWidth;
                float drawY = boxTop + row * cellHeight;
                sprites->Get(spriteId)->Draw(drawX, drawY);
            }
        }
    }

    RenderBoundingBox();
}

void CGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - (columns * cellWidth) / 2;
    t = y - cellHeight / 2;
    r = l + columns * cellWidth;
    b = t + rows * cellHeight;
}

void CGround::RenderBoundingBox()
{
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)(r - l);
    rect.bottom = (int)(b - t);

    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    float xx = l + rect.right / 2;
    float yy = t + rect.bottom / 2;

    CGame::GetInstance()->Draw(xx - cx, yy - cy,
        CTextures::GetInstance()->Get(ID_TEX_BBOX),
        nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}