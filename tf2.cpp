#pragma once

#include "tf2.h"

typedef float	(__thiscall* TF2_GetSpreadFn)(void*);
TF2_GetSpreadFn TF2_GetBulletSpread;

float TF2__flGetWeaponSpread( C_BaseCombatWeapon* pWeapon )
{

	if ( !TF2_GetBulletSpread )
		TF2_GetBulletSpread = utils->GetVirtualFunc<TF2_GetSpreadFn>(pWeapon, 444);

	float hResult = TF2_GetBulletSpread(pWeapon);

	return hResult;
}