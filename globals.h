#pragma once

#include "globals.h"

//HMODULE's and HINSTANCE's
HMODULE g_InstDLL;

HMODULE g_ClientDLL;
HMODULE g_EngineDLL;
HMODULE g_VstdLibDLL;

//DWORD's


//INT's
int g_ScrW;
int g_ScrH;
int g_AppID = 0;
int g_HighSeedNum = 0;
int g_iSpeedCmds = 0;

//BOOL's
bool g_ShouldDraw;
bool g_bIsReady;
bool g_bBlockCmd;

//CHAR's
const char *g_viewClassName;

//FLOAT's
float g_flNextPrimaryAttack;

//HANDLE's
HANDLE g_Thread1;
HANDLE g_Thread2;

//HWND's
HWND g_GameWindow;
HWND g_OverlayWindow;

//MARGIN's
const MARGINS  margin = {-1,-1,-1,-1};

//Engine Related Stuff

//Matrixes
VMatrix g_WorldToScreenMatrix;

//Interface Definitions
IBaseClientDLL *g_pClient;
IVEngineClient *g_pEngine;
IClientEntityList *g_pEntList;
IEngineTrace *g_pEngineTrace;
IVModelInfoClient *g_pModelInfo;
ICvar *g_pCVar;
CInput *g_pInput;
IGameEventManager2 *g_pGameManager;
CPrediction *g_pPrediction;
IGameMovement *g_pMovement;

CGlobalVarsBase *g_pGlobals;

//Interface Creations
CreateInterfaceFn g_ClientFactory;
CreateInterfaceFn g_EngineFactory;
CreateInterfaceFn g_VstdFactory;

//Other stuff
C_BasePlayer *g_pLocalEntity;
C_BaseEntity *g_pLocalWeapon;

Vector g_CurrentSpread;

INetChannel *g_pNetChan;

int g_LocalPlayerNum;