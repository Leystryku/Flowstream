#pragma once

#include "netvar_defs.h"

const char* NetVar__GetClassname( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return NULL;

	return GetClassnameFn( pEnt );
}

Vector* NetVar__GetViewOffset( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return &Vector(0,0,0);

	static DWORD offset = g_pNetworkMan->GetOffset( "*", "m_vecViewOffset[0]" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return &Vector(0,0,0);

	return (Vector*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

int NetVar__GetFlags( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BasePlayer", "m_fFlags" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return *(int*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

int NetVar__GetHealth( C_BaseEntity *pEnt )
{
	
	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BasePlayer", "m_iHealth" );

	int ret = 0;

	if ( ((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		ret = *(int*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));

	if ( ret < 1 && g_AppID == 440 )
	{
		static DWORD offset2 = g_pNetworkMan->GetOffset( "DT_BaseObject", "m_iHealth" );

		if ( ((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
			ret = *(int*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
	}

	return ret;
}

BYTE* NetVar__GetLifeState( C_BaseEntity *pEnt )
{
	
	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BasePlayer", "m_lifeState" );

	BYTE* ret = (BYTE*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));

	return ret;
}

int NetVar__GetTeamNumber( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BasePlayer", "m_iTeamNum" );

	if ( !offset )
		offset = g_pNetworkMan->GetOffset( "DT_BaseEntity", "m_iTeamNum" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return *(int*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

const Vector& NetVar__GetOrigin( C_BaseEntity *pEnt )
{

	if ( !pEnt )
	{
		Vector *shit = new Vector(0,0,0);
		return *shit;
	}

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BaseEntity", "m_vecOrigin" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
	{
		Vector *shit = new Vector(0,0,0);
		return *shit;
	}

	return *(Vector*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

QAngle* NetVar__GetViewPunch( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return &QAngle(0,0,0);

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_Local", "m_vecPunchAngle" );

	if ( !offset )
		offset = g_pNetworkMan->GetOffset( "DT_Local", "m_vecPunchAngleVel" );

	static DWORD offset2 = g_pNetworkMan->GetOffset( "DT_LocalPlayerExclusive", "m_Local" );
	
	if ( !((DWORD_PTR)(pEnt)+offset2+offset) )
		return &QAngle(0,0,0);

	return (QAngle*)((DWORD_PTR)pEnt+offset2+offset);
}

Vector& NetVar__GetVelocity( C_BaseEntity *pEnt )
{
	
	if ( !pEnt )
		return Vector(0,0,0);

	static DWORD offset = g_pNetworkMan->GetOffset( "*", "m_vecVelocity[0]" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return Vector(0,0,0);

	return *(Vector*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

float NetVar__GetNextPrimaryAttack( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_LocalActiveWeaponData", "m_flNextPrimaryAttack" );

	if ( !((DWORD_PTR)pEnt+offset) )
		return 0;

	if ( !(float*)((DWORD_PTR)pEnt+offset) )
		return 0;

	return *(float*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

int NetVar__GetClip1( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_LocalWeaponData", "m_iClip1" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return *(int*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

HANDLE NetVar__GetActiveWeapon( C_BaseEntity *pEnt )
{
	
	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BaseCombatCharacter", "m_hActiveWeapon" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return *(HANDLE*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

C_BaseEntity *NetVar__C_GetActiveWeapon( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return 0;

	HANDLE hWeapon = NetVar__GetActiveWeapon( pEnt );

	if ( !hWeapon )
		return NULL;

	int iWeapon = (DWORD)hWeapon & 0xFFF;

	return (C_BaseEntity*)g_pEntList->GetClientEntity( iWeapon );

}

int NetVar__GetTickBase( C_BaseEntity *pEnt )
{
	
	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_LocalPlayerExclusive", "m_nTickBase" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return *(int*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

QAngle& NetVar__GetEyeAngles( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return QAngle(0,0,0);

	static DWORD offset = g_pNetworkMan->GetOffset( "*", "m_angEyeAngles[0]" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return QAngle(0,0,0);

	return *(QAngle*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

Vector& NetVar__GetVecEyeAngles( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return Vector(0,0,0);

	static DWORD offset = g_pNetworkMan->GetOffset( "*", "m_angEyeAngles[0]" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return Vector(0,0,0);

	return *(Vector*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}

Vector& NetVar__GetEyePosition( C_BaseEntity *pEnt )
{

	if ( !pEnt )
	{
		return Vector(0,0,0);
	}

	Vector* pvoffset = NetVar__GetViewOffset( pEnt );

	Vector voffset = Vector(pvoffset->x, pvoffset->y,pvoffset->z);

	return pEnt->GetAbsOrigin() + voffset;
}

float NetVar__GetSimulationTime( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return 0;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BaseEntity", "m_flSimulationTime" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return *(float*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}


int NetVar__GetWeaponID( void *pEnt )
{

	if ( !pEnt )
		return 0;

	static DWORD offset1 = g_pNetworkMan->GetOffset( "DT_BaseAttributableItem", "m_AttributeManager" );
	static DWORD offset2 = g_pNetworkMan->GetOffset( "DT_AttributeContainer", "m_Item" );
	static DWORD offset3 = g_pNetworkMan->GetOffset( "DT_ScriptCreatedItem", "m_iItemDefinitionIndex" );

	return *(int*)((DWORD)pEnt+offset1+offset2+offset3);
}

QAngle *NetVar__GetRotation( C_BaseEntity *pEnt )
{

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BaseEntity", "m_angRotation" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return (QAngle*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));

}

ColorRGBExp32 &NetVar__GetColor( C_BaseEntity *pEnt )
{

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_BaseEntity", "m_clrRender" );

	return *(ColorRGBExp32*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));

}

bool TF2__NetVar__GetReadyToBackstab( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return false;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_TFWeaponKnife", "m_bReadyToBackstab" );

	if ( !((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset)) )
		return 0;

	return *(bool*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));
}


DWORD* TF2__NetVar__GetPlayerCond( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return false;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_TFPlayerShared", "m_nPlayerCond" );
	static DWORD offset2 = g_pNetworkMan->GetOffset( "DT_TFPlayer", "m_Shared" );

	return (DWORD*)((DWORD)pEnt+offset2+offset);
}

float TF2__NetVar__LastFireTime( C_BaseEntity *pEnt )
{

	if ( !pEnt )
		return 0.0f;

	static DWORD offset = g_pNetworkMan->GetOffset( "DT_LocalTFWeaponData", "m_flLastFireTime" );

	return *(float*)((DWORD_PTR)(pEnt)+(DWORD_PTR)(offset));

}
