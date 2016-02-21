#pragma once

#include "includes.h"

class m_Weapon
{
public:

	void CompensateSpread_Gmod( CUserCmd *pCmd );

	void CompensateSpread_TF2( CUserCmd *pCmd );

	void CompensateSpread_CSS( CUserCmd *pCmd );

	void CompensateSpread( CUserCmd *pCmd );

	void NoSpread( CUserCmd *pCmd );

	void NoPunchRecoil( QAngle &ang );

	void AutoFire( CUserCmd *pCmd );

	void AutoReload( CUserCmd *pCmd );


	void AutoStab( CUserCmd *pCmd );

};

extern m_Weapon *Weapon;