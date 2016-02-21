#pragma once

#include "includes.h"
#include "netvars.h"

const char* NetVar__GetClassname( C_BaseEntity *pEnt );

Vector* NetVar__GetViewOffset( C_BaseEntity *pEnt );

int NetVar__GetFlags( C_BaseEntity *pEnt );

int NetVar__GetHealth( C_BaseEntity *pEnt );

BYTE* NetVar__GetLifeState( C_BaseEntity *pEnt );

int NetVar__GetTeamNumber( C_BaseEntity *pEnt );

const Vector& NetVar__GetOrigin( C_BaseEntity *pEnt );

QAngle* NetVar__GetViewPunch( C_BaseEntity *pEnt );

Vector& NetVar__GetVelocity( C_BaseEntity *pEnt );

float NetVar__GetNextPrimaryAttack( C_BaseEntity *pEnt );

int NetVar__GetClip1( C_BaseEntity *pEnt );

HANDLE NetVar__GetActiveWeapon( C_BaseEntity *pEnt );

C_BaseEntity *NetVar__C_GetActiveWeapon( C_BaseEntity *pEnt );

int NetVar__GetTickBase( C_BaseEntity *pEnt );

QAngle& NetVar__GetEyeAngles( C_BaseEntity *pEnt );

Vector& NetVar__GetVecEyeAngles( C_BaseEntity *pEnt );

Vector& NetVar__GetEyePosition( C_BaseEntity *pEnt );

float NetVar__GetSimulationTime( C_BaseEntity *pEnt );

int NetVar__GetWeaponID(  void *pEnt );

QAngle* NetVar__GetRotation( C_BaseEntity *pEnt );

ColorRGBExp32& NetVar__GetColor( C_BaseEntity *pEnt );

bool TF2__NetVar__GetReadyToBackstab( C_BaseEntity *pEnt );

DWORD* TF2__NetVar__GetPlayerCond( C_BaseEntity *pEnt );

float TF2__NetVar__LastFireTime( C_BaseEntity *pEnt );
