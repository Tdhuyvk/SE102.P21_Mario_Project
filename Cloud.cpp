#include "Cloud.h"
#include "Textures.h"
#include "Game.h"
#include "Sprite.h"
#include "Sprites.h"
#include "AssetIDs.h"

// Helper to fetch sprite ID based on section (row) and position (col)
// section: 0 = top, 1 = bottom
// pos: 0 = left cap, 1 = middle segment, else = right cap

static int GetCloudSpriteId(int section, int pos) {
    if (section == 0) {
        if (pos == 0) return ID_SPRITE_CLOUD_CUTE_TOP_LEFT;
        else if (pos == 1) return ID_SPRITE_CLOUD_CUTE_TOP_MID;
        else               return ID_SPRITE_CLOUD_CUTE_TOP_RIGHT;
    }
    else {
        if (pos == 0) return ID_SPRITE_CLOUD_CUTE_BOTTOM_LEFT;
        else if (pos == 1) return ID_SPRITE_CLOUD_CUTE_BOTTOM_MID;
        else               return ID_SPRITE_CLOUD_CUTE_BOTTOM_RIGHT;
    }
}


void CCloud::Render() {

    float offsetX = CLOUD_SPRITE_WIDTH / 2;
    float offsetY = CLOUD_SPRITE_HEIGHT / 2;

    CSprites* sprites = CSprites::GetInstance();

    int width = length + 2;

    for (int row = 0; row < 2; ++row) {
        int section = row;  // 0 = top, 1 = bottom
        for (int col = 0; col < width; ++col) {
            int pos = (width == 1 ? 1
                : col == 0 ? 0
                : col == width - 1 ? 2
                : 1);

            if (int spriteId = GetCloudSpriteId(section, pos)) {
                sprites->Get(spriteId)
                    ->Draw(
                        x + col * CLOUD_SPRITE_WIDTH + offsetX,
                        y + row * CLOUD_SPRITE_HEIGHT + offsetY
                    );
            }
        }
    }

    //RenderBoundingBox();
}

void CCloud::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x;
    t = y;
    r = x + (length + 2) * CLOUD_SPRITE_WIDTH;
    b = y + 2 * CLOUD_SPRITE_HEIGHT;
}

void CCloud::RenderBoundingBox() {
    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    // draw bounding box at center

    float left = l - cx;
    float top = t - cy;
    float width = r - l;
    float height = b - t;

    CGame::GetInstance()->Draw(
        left + width / 2,
        top + height / 2,
        CTextures::GetInstance()->Get(ID_TEX_BBOX),
        nullptr,
        BBOX_ALPHA,
        (int)width,
        (int)height
    );
}
