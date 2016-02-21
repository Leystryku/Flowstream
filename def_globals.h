#pragma once

#include "includes.h"


//HMODULE's and HINSTANCE's
extern HMODULE g_InstDLL;

extern HMODULE g_ClientDLL;
extern HMODULE g_EngineDLL;
extern HMODULE g_VstdLibDLL;

//DWORD's


//INT's
extern int g_ScrW;
extern int g_ScrH;
extern int g_AppID;
extern int g_HighSeedNum;
extern int g_iSpeedCmds;

//BOOL's
extern bool g_ShouldDraw;
extern bool g_bIsReady;
extern bool g_bBlockCmd;

//CHAR's
extern const char* g_viewClassName;

//FLOAT's
extern float g_flNextPrimaryAttack;

//HANDLE's
extern HANDLE g_Thread1;
extern HANDLE g_Thread2;
extern HANDLE g_Thread3;

//HWND's
extern HWND g_GameWindow;
extern HWND g_OverlayWindow;

//MARGIN's
extern const MARGINS margin;

//Engine Related Stuff

//Matrixes
extern VMatrix g_WorldToScreenMatrix;

//Interface Definitions
extern IBaseClientDLL *g_pClient;
extern IVEngineClient *g_pEngine;
extern IClientEntityList *g_pEntList;
extern IEngineTrace *g_pEngineTrace;
extern IVModelInfoClient *g_pModelInfo;
extern ICvar *g_pCVar;
extern CInput *g_pInput;
extern IGameEventManager2 *g_pGameManager;
extern CPrediction *g_pPrediction;
extern IGameMovement *g_pMovement;

extern CGlobalVarsBase *g_pGlobals;

//Interface Creations
extern CreateInterfaceFn g_ClientFactory;
extern CreateInterfaceFn g_EngineFactory;
extern CreateInterfaceFn g_VstdFactory;

//Other Stuff
extern C_BasePlayer *g_pLocalEntity;
extern C_BaseEntity *g_pLocalWeapon;

extern Vector g_CurrentSpread;

extern INetChannel *g_pNetChan;

extern int g_LocalPlayerNum;