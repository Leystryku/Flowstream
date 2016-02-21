#pragma once

#include "antiaim.h"

m_AntiAim *AntiAim = new m_AntiAim;

void m_AntiAim::DoAntiAim( CUserCmd *pCmd, bool* bSendPacket )
{

	if ( pCmd->viewangles.x == 0 && pCmd->viewangles.y == 0 && pCmd->viewangles.z == 0 )
		return;

	if ( ( pCmd->buttons & IN_ATTACK ) )
		return;

	QAngle oldMove = pCmd->move;
	QAngle oldView = pCmd->viewangles;

	Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
	QAngle v;

	v.Init(pCmd->viewangles.y, pCmd->viewangles.x, pCmd->viewangles.z);
	AngleVectors( pCmd->viewangles, &viewforward, &viewright, &viewup );

	/*
	static int AntiAimAngs = 3;
	
	if ( AntiAimAngs == 0 )
		pCmd->viewangles = QAngle(181, -180, -180);

	if ( AntiAimAngs == 1 )
		pCmd->viewangles.x = 541;

	if ( AntiAimAngs == 1 && g_AppID != 4000 )
	{
		pCmd->viewangles.y = pCmd->viewangles.y -125;
	}

	if ( AntiAimAngs == 2  )
		pCmd->viewangles = QAngle(-181, -181, 180);

	if ( AntiAimAngs == 3 )
		pCmd->viewangles.y = pCmd->viewangles.y - 100;

	if ( AntiAimAngs == 4 )
		pCmd->viewangles = QAngle(-181, -181, -181);
	
	if ( AntiAimAngs == 5 )
		pCmd->viewangles.x = pCmd->viewangles.x - 100;
	
	if ( AntiAimAngs == 6 )
		pCmd->viewangles.y = pCmd->viewangles.x;

	if ( AntiAimAngs == 7 )
		pCmd->viewangles.x = pCmd->viewangles.y;
	
	if ( AntiAimAngs == 8 )
		pCmd->viewangles = QAngle(-180,-181,180);
	

	if ( AntiAimAngs > 7 )
		AntiAimAngs = 0;

	AntiAimAngs += 1;
	*/

	static bool AntiAimA = false;

	if ( AntiAimA )
	{
		static bool swag = false;
		static bool swagx = false;

		if ( swag )
			pCmd->viewangles.y = 590;
		else
			pCmd->viewangles.y = -2000;


		if ( swagx )
			pCmd->viewangles.x = 480;
		else
			pCmd->viewangles.x = -2000 * g_pGlobals->interval_per_tick;

		pCmd->viewangles.z = -13337;

		swag = !swag;
		swagx = !swagx;
	}else{
		pCmd->viewangles.x = pCmd->viewangles.x * g_pGlobals->interval_per_tick;
		pCmd->viewangles.y = pCmd->viewangles.y * g_pGlobals->interval_per_tick;
	}

	AntiAimA = !AntiAimA;

	Bots->FixMove( pCmd, oldView );


}