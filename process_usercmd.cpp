#pragma once

#include "process_usercmd.h"

void ProcessUserCommand_PropKill( CUserCmd *pCmd )
{

	if ( !g_pLocalWeapon )
		return;

	if ( !strcmp(NetVar__GetClassname(g_pLocalWeapon), "weapon_physgun") )
	{
		static int hold = 0, punt = 0;

		if ( (pCmd->buttons & IN_ATTACK) )
		{
			float latency = g_pNetChan->GetLatency(0) + g_pNetChan->GetLatency(FLOW_INCOMING);

			hold = (int)((1.f / g_pGlobals->interval_per_tick) * (latency + .05f));
			punt = (int)((1.f / g_pGlobals->interval_per_tick) * (latency + .2f));
		}
		else
		{
			if (hold > 0)
			{
				pCmd->buttons |= IN_ATTACK;
				hold--;
			}

			if (punt > 0)
			{
				*pCmd->mousewheel() = 0x7F;
				punt--;
			}
		}
	}
}

void ProcessUserCommand_ViewESP( CUserCmd *cmd )
{
	QAngle Angles = cmd->viewangles;

	Vector Eyes, Forward;
	

	Eyes = NetVar__GetEyePosition( g_pLocalEntity );

	if ( Eyes.x == 0 && Eyes.y == 0 && Eyes.z == 0 )
		return;

	Vector vDirection;

	AngleVectors( Angles, &vDirection );

	vDirection = vDirection * 8192 + Eyes;

	if ( vDirection.x == 0 && vDirection.y == 0 && vDirection.z == 0 )
		return;
	
	trace_t tr;
	CTraceFilterHitAll filter;

	Ray_t pRay;
	pRay.Init( Eyes, vDirection );

	g_pEngineTrace->TraceRay( pRay, 0x46004003, &filter, &tr);

	if ( !tr.m_pEnt || tr.m_pEnt->entindex() == 0 )
	{
		g_viewClassName = NULL;
	}else{

		Vector org = NetVar__GetOrigin( tr.m_pEnt );

		if ( org.x == 0 && org.y == 0 && org.z == 0 )
		{
			g_viewClassName = NULL;
		}else{

			ClientClass *trClientClass = tr.m_pEnt->GetClientClass();
			
			if ( !trClientClass )
			{
				g_viewClassName = NULL;
				return;
			}

			if ( g_AppID == 4000 )
				g_viewClassName = NetVar__GetClassname( tr.m_pEnt );
			else
				g_viewClassName = trClientClass->m_pNetworkName;

			if ( !g_viewClassName )
				g_viewClassName = NULL;
		}
	}
}

bool doingskip = false;
void ProcessUserCommand( CUserCmd *pCmd, DWORD CreateMove_EBP )
{

	DWORD *returnAddress = (DWORD*)( *(char**)CreateMove_EBP + 0x4 );
	bool* bSendPacket = ( bool* )( *( bool** )CreateMove_EBP - 0x1 );

	g_pLocalWeapon = NetVar__C_GetActiveWeapon( g_pLocalEntity );

	if ( g_pLocalWeapon && !g_pLocalWeapon->IsDormant() )
		g_flNextPrimaryAttack = NetVar__GetNextPrimaryAttack( g_pLocalWeapon );
	else
		g_flNextPrimaryAttack = 0.0f;

	QAngle oldView = pCmd->viewangles;

	if ( menu->M_SafeMode )
	{
	
		if ( doingskip )
		{
			pCmd->viewangles.x = 0;
			pCmd->viewangles.y = 0;
			pCmd->viewangles.z = 0;
			doingskip = false;
			Bots->FixMove(pCmd, oldView);
		}else{
			doingskip = true;
		}

		
	}else{
		doingskip = true;
	}

	if ( !utils->IsAlive( g_pLocalEntity ) || menu->M_FreezeHack )
	{
		if ( menu->M_ESP_ViewESP )
			ProcessUserCommand_ViewESP( pCmd );

		if ( menu->M_FreezeHack )
			pCmd->tick_count = 0xFFFFF;

		return;
	}

 	if ( menu->M_FlashlightSpam )
	{
		pCmd->impulse = 100;
	}

	if ( menu->S_PredictionType == 1 || menu->S_PredictionType == 3 )
		Bots->RunCommand( pCmd );

	g_pGlobals->curtime = ( NetVar__GetTickBase( g_pLocalEntity ) ) * g_pGlobals->interval_per_tick;

	if ( menu->M_Bhop )
		BunnyHop->DoJump( pCmd, !menu->M_SafeMode );

	if ( menu->M_AimEnable && doingskip )
		Bots->Aimbot( pCmd, bSendPacket );

	if ( menu->M_TriggerBot && doingskip )
		Bots->Trigger( pCmd );

	if ( menu->M_AutoPistol )
		Weapon->AutoFire( pCmd );

	if ( menu->M_AutoStab )
		Weapon->AutoStab( pCmd );

	if ( menu->M_AutoReload )
		Weapon->AutoReload( pCmd );

	if ( menu->M_NoSpread )
		Weapon->NoSpread( pCmd );

	if ( menu->M_CompensateSpread && doingskip )
		Weapon->CompensateSpread( pCmd );

	if ( menu->M_NoPunchRecoil && doingskip )
		Weapon->NoPunchRecoil( pCmd->viewangles );

	if ( menu->M_AntiAim && doingskip )
		AntiAim->DoAntiAim( pCmd, bSendPacket );

	if ( menu->M_ESP_ViewESP )
		ProcessUserCommand_ViewESP( pCmd );

	if ( g_AppID == 4000 && menu->M_PropKill )
		ProcessUserCommand_PropKill( pCmd );

	if ( !doingskip )
	{

		if ( pCmd->buttons & IN_ATTACK )
			pCmd->buttons &= ~IN_ATTACK;
	}


	if ( menu->M_FakeLag )
	{
		static bool bLagSwitch = false;
		static int iLagCounter = 0;
		iLagCounter++;

		if( iLagCounter == 2 && bLagSwitch )
		{
			bLagSwitch = false;
			iLagCounter = 0;
		}

		if( iLagCounter > menu->S_FakeLag )
		{
			bLagSwitch = !bLagSwitch;
			iLagCounter = 0;
		}

		if( !bLagSwitch )
			*bSendPacket = false;
		else
			*bSendPacket = true;

		if ( (pCmd->buttons & IN_ATTACK) )
			*bSendPacket = true;

	}

	if ( menu->M_FakeCrouch && pCmd->buttons & IN_DUCK )
	{
		static bool bDoCrouch = false;
		static int iCrouchCount = 0;
			
		if ( iCrouchCount == 3)
		{
			iCrouchCount = 0;
			bDoCrouch = !bDoCrouch;
		}else{
			iCrouchCount++;
		}
		
		if ( bDoCrouch )
		{
			pCmd->buttons |= IN_DUCK;
			*bSendPacket = true;
		}else{
			pCmd->buttons &= ~IN_DUCK;
			*bSendPacket = false;
		}

		if ( (pCmd->buttons & IN_ATTACK) )
			*bSendPacket = true;
	}

	if ( menu->M_SpeedHack == 1 )
	{

		static int CL_Speed = 0;

		if ( CL_Speed > 0 )	
		{
			CL_Speed -= 1;
			
			if ( menu->M_FakeLag )
				*bSendPacket = false;

			*returnAddress -= 5;
		}else{
			CL_Speed = menu->S_SpeedHack;

			if ( !g_iSpeedCmds )
				g_iSpeedCmds = menu->S_SpeedHack;

			if ( menu->M_FakeLag )
				*bSendPacket = true;
		}

	}

}