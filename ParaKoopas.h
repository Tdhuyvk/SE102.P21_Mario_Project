#pragma once
#include "Koopas.h"

#define ID_ANI_KARA_KOOPAS_GREEN_WALKING_LEFT    6202
#define ID_ANI_KARA_KOOPAS_GREEN_WALKING_RIGHT   6203

#define PARA_KOOPAS_TYPE_GREEN 3

#define PARA_KOOPAS_FLY_SPEED		0.1f

#define PARA_KOOPAS_STATE_FLY		600
#define PARA_KOOPAS_STATE_NO_WING_WALKING	700

#define PARA_KOOPAS_BBOX_HEIGHT 28

class CParaKoopas : public CKoopas
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	CParaKoopas(float x, float y, BOOLEAN type);
	virtual void SetState(int state);
};