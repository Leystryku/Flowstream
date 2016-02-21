#pragma once

#include "weapon.h"

m_Weapon *Weapon = new m_Weapon;

void m_Weapon::CompensateSpread_Gmod( CUserCmd *pCmd )
{
	static C_BaseEntity* lastw = NULL;

	if ( lastw && lastw != g_pLocalWeapon )
	{
		lastw = g_pLocalWeapon;
		g_CurrentSpread = Vector(0,0,0);
		/*int bak_curtime = g_pGlobals->curtime;
		g_pGlobals->curtime = -999;
		g_pLocalEntity->ItemPostFrame();
		g_pGlobals->curtime = bak_curtime;*/
		return;
	}

	RandomSeed( MD5_PseudoRandom(pCmd->command_number) & 255 );
	float flRandomX = RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f);
	float flRandomY = RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f);

	Vector vForward, vRight, vUp;
	Vector vSpread = Vector(FloatNegate(g_CurrentSpread.x), FloatNegate(g_CurrentSpread.y), 0.f);

	utils->NormalizeAngles( pCmd->viewangles );
	AngleVectors(pCmd->viewangles, &vForward, &vRight, &vUp);

	Vector vNewAngles = vForward + (vRight * vSpread.x * flRandomX) + (vUp * vSpread.y * flRandomY);

	VectorAngles(vNewAngles, pCmd->viewangles);
	utils->NormalizeAngles(pCmd->viewangles);

}


typedef void	(__thiscall* CS_UpdateAccuracyPenaltyFn)(void*);
typedef float	(__thiscall* CS_GetSpreadFn)(void*);
void m_Weapon::CompensateSpread_CSS( CUserCmd *pCmd )
{

	static int
		m_fUpdateAccuracyPenalty	= 0x5CC + 8,
		m_fGetWeaponSpread			= 0x5CC + 0,
		m_fGetWeaponCone			= 0x5CC + 4;

	CS_UpdateAccuracyPenaltyFn UpdateAccuracyPenalty = ((CS_UpdateAccuracyPenaltyFn)(*(PDWORD)(*(PDWORD)g_pLocalWeapon + m_fUpdateAccuracyPenalty)));
	CS_GetSpreadFn GetWeaponSpread	= ((CS_GetSpreadFn)(*(PDWORD)(*(PDWORD)g_pLocalWeapon + m_fGetWeaponSpread)));
	CS_GetSpreadFn GetWeaponCone	= ((CS_GetSpreadFn)(*(PDWORD)(*(PDWORD)g_pLocalWeapon + m_fGetWeaponCone)));

	Vector vForward, vRight, vUp;

	// Credits: LuaStoned

	UpdateAccuracyPenalty( g_pLocalWeapon );

	float flSpread	= GetWeaponSpread( g_pLocalWeapon );
	float flCone	= GetWeaponCone( g_pLocalWeapon );

	int uSeed = MD5_PseudoRandom(pCmd->command_number) & 255;

	RandomSeed( uSeed + 1);

	float flRandom1 = RandomFloat(0.f, (float)M_PI * 2.f); // 6.283185f
	float flRandom2 = RandomFloat(0.f, flSpread);
	float flRandom3 = RandomFloat(0.f, (float)M_PI * 2.f); // 6.283185f
	float flRandom4 = RandomFloat(0.f, flCone);

	float flLeftX = cosf(flRandom1) * flRandom2;
	float flLeftY = sinf(flRandom1) * flRandom2;

	float flRightX = cosf(flRandom3) * flRandom4;
	float flRightY = sinf(flRandom3) * flRandom4;

	Vector vSpread = Vector(FloatNegate(flLeftX + flRightX), FloatNegate(flLeftY + flRightY), 0.f);
			
	utils->NormalizeAngles( pCmd->viewangles);
	AngleVectors( pCmd->viewangles, &vForward, &vRight, &vUp);

	Vector vNewAngles = vForward + (vRight * vSpread.x) + (vUp * vSpread.y);
	vNewAngles.NormalizeInPlace();

	VectorAngles(vNewAngles, pCmd->viewangles);
	utils->NormalizeAngles(pCmd->viewangles);

}

void m_Weapon::CompensateSpread_TF2( CUserCmd *pCmd )
{

	float flSpread = TF2__flGetWeaponSpread( (C_BaseCombatWeapon*)g_pLocalWeapon );//TF2__flGetSpreadFactor( );
	Msg("shit: %f\n", flSpread );
	
    Vector vForward, vRight, vUp, vSpread;
    QAngle qFinal;

	utils->NormalizeAngles( pCmd->viewangles );

    int uSeed = pCmd->random_seed & 255;
    RandomSeed( uSeed );

    float flRandomX = RandomFloat( -0.5, 0.5 ) + RandomFloat( -0.5, 0.5 );
    float flRandomY = RandomFloat( -0.5, 0.5 ) + RandomFloat( -0.5, 0.5 );

    vSpread[ 0 ] = FloatNegate(flRandomX * flSpread);
    vSpread[ 1 ] = FloatNegate(flRandomY * flSpread);
	vSpread[ 2 ] = 0;

	utils->NormalizeAngles( pCmd->viewangles );

    AngleVectors( pCmd->viewangles, &vForward, &vRight, &vUp );

    Vector vNewAngles = vForward + ( vRight * vSpread[ 0 ] ) + ( vUp * vSpread[ 1 ] );
    vNewAngles.NormalizeInPlace();

	VectorAngles(vNewAngles, pCmd->viewangles);
	utils->NormalizeAngles( pCmd->viewangles );

}

int idealSeed = 0;

void m_Weapon::NoSpread( CUserCmd *pCmd )
{

	int seed = 142; //gmod,tf2

	if ( g_AppID == 240 ) // css
		seed = 2087;

	int num = 0;
	
	if ( !idealSeed )
		idealSeed = MD5_PseudoRandom(seed) & 255;

	int niceSeed = idealSeed;

	if ( g_AppID == 440 ) // tf2
	{
		if ( g_pLocalWeapon && g_pLocalWeapon->entindex() != 0 )
		{
			ClientClass *cc = g_pLocalWeapon->GetClientClass();

			if ( cc && cc->m_pNetworkName )
			{
				const char *classname = cc->m_pNetworkName;

				if ( strstr(classname, "Minigun") )
				{
					niceSeed = MD5_PseudoRandom(188) & 255;
					seed = 188;
				}
			}
		}
	}

	for ( int i = g_HighSeedNum + 1; !num; i++ )
	{
		int uSeed = MD5_PseudoRandom(i);

		if ( !uSeed )
			continue;

		if ( (uSeed & 255) != niceSeed )
			continue;
		
		num = i;

	}

	if ( g_AppID == 4000 )
	{
		if ( g_pLocalWeapon && g_pLocalWeapon->entindex() != 0 )
		{
			const char *classname = NetVar__GetClassname(g_pLocalWeapon);

			if ( classname )
			{

				if ( strstr(classname, "fas2") )
				{
					num = 6719921;
				}
			}
		}
	}

	pCmd->command_number = num;
		
	g_HighSeedNum = num + 1;

}

void m_Weapon::CompensateSpread( CUserCmd *pCmd )
{

	if ( !g_pLocalWeapon )
		return;

	//if ( g_pLocalWeapon->IsDormant() )
	//	return;

	if ( !(pCmd->buttons & IN_ATTACK) )
		return;

	if ( g_AppID == 4000 )
		CompensateSpread_Gmod( pCmd );

	if ( g_AppID == 240 )
		CompensateSpread_CSS( pCmd );

	if ( g_AppID == 440 )
		CompensateSpread_TF2( pCmd );

}


void m_Weapon::NoPunchRecoil( QAngle &ang )
{
	if ( ang.x == 0 && ang.y == 0 && ang.z == 0 )
		return;

	float factor = 1.0f;

	if ( g_AppID == 240 )
		factor = 2.0f;

	QAngle vecPunchAng = *NetVar__GetViewPunch( g_pLocalEntity );

	float flModifier = VectorNormalize( vecPunchAng.Base() );
	flModifier -= (10.0f + flModifier * 0.5f) * g_pGlobals->interval_per_tick;

	QAngle finale = (vecPunchAng*flModifier);
	
	if ( g_AppID == 4000 )
		finale = vecPunchAng;

	finale = finale * factor;

	ang -= finale;

}

static bool altAutoPistol_switch = false;
int skip=15;

void m_Weapon::AutoFire( CUserCmd *pCmd )
{

	if ( menu->M_SafeMode )
	{
		skip = skip - 1;

		if ( 1 > skip )
		{
			if ( skip < -5 )
				skip = 15;

			return;
		}
	}

	if ( !Bots->HoldingValidWep() )
		return;

	if ( menu->M_AltAutoPistol )
	{

		
		if( pCmd->buttons & IN_ATTACK)
		{

			if ( altAutoPistol_switch )
				pCmd->buttons &= ~IN_ATTACK;
			else
				pCmd->buttons |= IN_ATTACK;

			altAutoPistol_switch =! altAutoPistol_switch;
		}

		return;
	}

	int m_iClip1 = NetVar__GetClip1( g_pLocalWeapon );

	if ( 1 > m_iClip1 )
		return;

	if ( ( pCmd->buttons & IN_ATTACK ) )
	{
		if ( !Bots->CanShoot() )
		{
			pCmd->buttons &= ~IN_ATTACK;
		}

	}

}

extern CTraceFilter filter;
extern Ray_t pRay;
extern trace_t tr;

void m_Weapon::AutoStab( CUserCmd *pCmd )
{

	if ( ( pCmd->buttons & IN_ATTACK ) ) return;
	if ( !g_pLocalWeapon ) return;
	//if ( g_pLocalWeapon->IsDormant() ) return;

	ClientClass *wepClass = g_pLocalWeapon->GetClientClass();

	if ( !wepClass )
		return;

	const char *classname = wepClass->m_pNetworkName;

	if ( !classname )
		return;

	if ( g_AppID == 4000 )
	{
		classname = NetVar__GetClassname( g_pLocalWeapon );
		if ( !classname ) return;
	}

	
	if ( strstr(classname, "Knife") == 0 && strstr(classname, "knife") == 0 && strstr(classname, "melee") == 0 && strstr(classname, "Melee") == 0 )
		return;

	if ( g_AppID == 440 && TF2__NetVar__GetReadyToBackstab( g_pLocalEntity ) )
	{
		pCmd->buttons |= IN_ATTACK;
		return;
	}

    Vector Forward;
    AngleVectors( pCmd->viewangles, &Forward );

    Vector origForward( Forward );

	Forward *= menu->S_AutoStabDist;
	
    Vector Start = NetVar__GetEyePosition(g_pLocalEntity);
    Vector End = Start + Forward;

	static	Vector HeadMins( -16, -16, -18 );
	static	Vector HeadMaxs( 16, 16, 18 );

	pRay.Init( Start, End, HeadMins, HeadMaxs );
	g_pEngineTrace->TraceRay( pRay, 0x46004003, &filter, &tr );

	if ( !tr.m_pEnt )
		return;

	if ( !utils->IsAlive( tr.m_pEnt ) )
		return;

	if ( !menu->M_Aim_TeamKill && utils->IsSameTeam(g_pLocalEntity, tr.m_pEnt) )
		return;

	IClientEntity* pEntityTouched = tr.m_pEnt;

	AngleVectors( pEntityTouched->GetRenderAngles( ), &Forward );

	D3DXVECTOR2 MyLOS( origForward.x, origForward.y );
	D3DXVec2Normalize( &MyLOS, &MyLOS );

	D3DXVECTOR2 TargetLOS( Forward.x, Forward.y );
	D3DXVec2Normalize( &TargetLOS, &TargetLOS );

	float DotProduct = D3DXVec2Dot( &MyLOS, &TargetLOS );

	if ( menu->M_AutoStab_BackOnly )
	{
		if ( DotProduct > 0.80f )
			pCmd->buttons |= IN_ATTACK;
		else
			return;
	}

	pCmd->buttons |= IN_ATTACK;


}

int iNextReload = 0;

void m_Weapon::AutoReload( CUserCmd *pCmd )
{

	int m_iClip1;


	if ( g_pLocalWeapon )
	{
		m_iClip1 = NetVar__GetClip1( g_pLocalWeapon );
	}

	if ( m_iClip1 == 0 )
	{
		clock_t t;
		t = clock();

		if ( iNextReload > t )
		{
			return;
		}

		iNextReload = t + ( CLOCKS_PER_SEC * 3 );

		pCmd->buttons |= IN_RELOAD;
		
		if ( (pCmd->buttons & IN_ATTACK) )
			pCmd->buttons &= ~IN_ATTACK;

	}

}