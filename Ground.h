#pragma once

#include "Platform.h"

// inheritance of CPlatform
class CGround : public CPlatform
{

public:
	CGround(float x, float y,
    float cell_width, float cell_height, int length,
        int sprite_id_begin, int sprite_id_middle, int sprite_id_end)
		:CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end) {}

	void Update(DWORD dt) {}
	int IsDirectionColliable(float nx, float ny) { return 1; }
};

typedef CGround* LPGROUND;