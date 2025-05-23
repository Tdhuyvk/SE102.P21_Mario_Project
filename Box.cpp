#include "Box.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
#include "AssetIDs.h"

// helper collect sprite id, (section), (position) each row
// section: 0 - head, 1 - body, 2 - foot
// pos: 0 - left, 1 - mid, 2 - right
int GetBoxSpriteId(BoxColor color, int section, int pos)
{
    switch (color)
    {
    case BOX_WHITE:
        if (section == 0) // Head
        {
            if (pos == 0) return ID_SPRITE_BOX_WHITE_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_WHITE_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BOX_WHITE_HEAD_RIGHT;
        }
        else if (section == 1) // Body
        {
            if (pos == 0) return ID_SPRITE_BOX_WHITE_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_WHITE_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BOX_WHITE_BODY_RIGHT;
        }
        else if (section == 2) // Foot
        {
            if (pos == 0) return ID_SPRITE_BOX_WHITE_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_WHITE_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BOX_WHITE_FOOT_RIGHT;
        }
        break;

    case BOX_GREEN:
        if (section == 0)
        {
            if (pos == 0) return ID_SPRITE_BOX_GREEN_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_GREEN_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BOX_GREEN_HEAD_RIGHT;
        }
        else if (section == 1)
        {
            if (pos == 0) return ID_SPRITE_BOX_GREEN_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_GREEN_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BOX_GREEN_BODY_RIGHT;
        }
        else if (section == 2)
        {
            if (pos == 0) return ID_SPRITE_BOX_GREEN_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_GREEN_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BOX_GREEN_FOOT_RIGHT;
        }
        break;

    case BOX_PINK:
        if (section == 0)
        {
            if (pos == 0) return ID_SPRITE_BOX_PINK_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_PINK_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BOX_PINK_HEAD_RIGHT;
        }
        else if (section == 1)
        {
            if (pos == 0) return ID_SPRITE_BOX_PINK_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_PINK_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BOX_PINK_BODY_RIGHT;
        }
        else if (section == 2)
        {
            if (pos == 0) return ID_SPRITE_BOX_PINK_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_PINK_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BOX_PINK_FOOT_RIGHT;
        }
        break;

    case BOX_BLUE:
        if (section == 0)
        {
            if (pos == 0) return ID_SPRITE_BOX_BLUE_HEAD_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_BLUE_HEAD_MID;
            else if (pos == 2) return ID_SPRITE_BOX_BLUE_HEAD_RIGHT;
        }
        else if (section == 1)
        {
            if (pos == 0) return ID_SPRITE_BOX_BLUE_BODY_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_BLUE_BODY_MID;
            else if (pos == 2) return ID_SPRITE_BOX_BLUE_BODY_RIGHT;
        }
        else if (section == 2)
        {
            if (pos == 0) return ID_SPRITE_BOX_BLUE_FOOT_LEFT;
            else if (pos == 1) return ID_SPRITE_BOX_BLUE_FOOT_MID;
            else if (pos == 2) return ID_SPRITE_BOX_BLUE_FOOT_RIGHT;
        }
        break;
    }
    return 0;
}

void CBox::Render()
{

    float l = x + cellWidth / 2;
    float t = y + cellHeight / 2;

    CSprites* sprites = CSprites::GetInstance();

    for (int row = 0; row < rows; row++)
    {
        int section;
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
            if (columns == 1)
                pos = 1;
            else if (col == 0)
                pos = 0;
            else if (col == columns - 1)
                pos = 2;
            else
                pos = 1;

            int spriteId = GetBoxSpriteId(color, section, pos);
            if (spriteId != 0)
            {
                sprites->Get(spriteId)->Draw(
                    l + col * cellWidth,
                    t + row * cellHeight
                );
            }
        }
    }

    //RenderBoundingBox();
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + columns * cellWidth;
    b = y + rows * cellHeight;
}

void CBox::RenderBoundingBox()
{
    // get bounding box
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    // RECT (r-l) x (b-t)
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)(r - l);
    rect.bottom = (int)(b - t);

    // center bounding box
    float xx = (l + r) / 2;
    float yy = (t + b) / 2;

    // get camera
    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    // draw bounding box with (centerX, centerY)
    CGame::GetInstance()->Draw(xx - cx, yy - cy,
        CTextures::GetInstance()->Get(ID_TEX_BBOX),
        nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}

int CBox::IsDirectionColliable(float nx, float ny)
{
    if (nx == 0 && ny == -1) return 1;
    else return 0;
}
