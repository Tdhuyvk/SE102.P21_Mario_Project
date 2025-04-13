#include "Block.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
#include "AssetIDs.h"

// helper collect sprite id, (section), (position) each row
// section: 0 - head, 1 - body, 2 - foot
// pos: 0 - left, 1 - mid, 2 - right
int GetBlockSpriteId(BlockColor color, int section, int pos)
{
    switch (color)
    {
    case BLOCK_WHITE:
        if (section == 0) // Head
        {
            if (pos == 0) return ID_SPRITE_BLOCK_WHITE_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_WHITE_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_WHITE_HEAD_RIGHT;
        }
        else if (section == 1) // Body
        {
            if (pos == 0) return ID_SPRITE_BLOCK_WHITE_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_WHITE_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_WHITE_BODY_RIGHT;
        }
        else if (section == 2) // Foot
        {
            if (pos == 0) return ID_SPRITE_BLOCK_WHITE_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_WHITE_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_WHITE_FOOT_RIGHT;
        }
        break;

    case BLOCK_GREEN:
        if (section == 0)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_GREEN_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_GREEN_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_GREEN_HEAD_RIGHT;
        }
        else if (section == 1)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_GREEN_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_GREEN_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_GREEN_BODY_RIGHT;
        }
        else if (section == 2)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_GREEN_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_GREEN_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_GREEN_FOOT_RIGHT;
        }
        break;

    case BLOCK_PINK:
        if (section == 0)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_PINK_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_PINK_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_PINK_HEAD_RIGHT;
        }
        else if (section == 1)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_PINK_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_PINK_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_PINK_BODY_RIGHT;
        }
        else if (section == 2)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_PINK_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_PINK_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_PINK_FOOT_RIGHT;
        }
        break;

    case BLOCK_BLUE:
        if (section == 0)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_BLUE_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_BLUE_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_BLUE_HEAD_RIGHT;
        }
        else if (section == 1)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_BLUE_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_BLUE_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_BLUE_BODY_RIGHT;
        }
        else if (section == 2)
        {
            if (pos == 0) return ID_SPRITE_BLOCK_BLUE_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BLOCK_BLUE_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BLOCK_BLUE_FOOT_RIGHT;
        }
        break;
    }
    return 0;
}

void CBlock::Render()
{
    float startX = x - (columns * cellWidth) / 2;
    float startY = y - cellHeight / 2;

    CSprites* sprites = CSprites::GetInstance();

    for (int row = 0; row < rows; row++)
    {
        int section;
        // section = 1 -> head, 
        // >=2 head, body, foot
        if (rows == 1)
            section = 0;
        else if (row == 0)
            section = 0;
        else if (row == rows - 1)
            section = 2;
        else
            section = 1;

        for (int col = 0; col < columns; col++)
        {
            int pos;
            // row:
            // 1 row -> use mid, else left, right, mid.
            if (columns == 1)
                pos = 1;
            else if (col == 0)
                pos = 0;
            else if (col == columns - 1)
                pos = 2;
            else
                pos = 1;

            int spriteId = GetBlockSpriteId(color, section, pos);
            if (spriteId != 0)
            {
                // draw current
                float drawX = startX + col * cellWidth;
                float drawY = startY + row * cellHeight;
                sprites->Get(spriteId)->Draw(drawX, drawY);
            }
        }
    }

    RenderBoundingBox();
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - (columns * cellWidth) / 2;
    t = y - (rows * cellHeight) / 2;
    r = l + columns * cellWidth;
    b = t + rows * cellHeight;
}

//void CBlock::RenderBoundingBox()
//{
//    D3DXVECTOR3 p(x, y, 0);
//    RECT rect;
//
//    LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
//
//    float l, t, r, b;
//    GetBoundingBox(l, t, r, b);
//    rect.left = 0;
//    rect.top = 0;
//    rect.right = (int)(r - l);
//    rect.bottom = (int)(b - t);
//
//    float cx, cy;
//    CGame::GetInstance()->GetCamPos(cx, cy);
//
//    float xx = l + rect.right / 2;
//
//    CGame::GetInstance()->Draw(xx - cx, t - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
//}

void CBlock::RenderBoundingBox()
{
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    // RECT based on bounding box size
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)(r - l);
    rect.bottom = (int)(b - t);

    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    // draw bounding box at (l, t) offset to camera
    CGame::GetInstance()->Draw(l - cx, t - cy,
        CTextures::GetInstance()->Get(ID_TEX_BBOX),
        nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}
