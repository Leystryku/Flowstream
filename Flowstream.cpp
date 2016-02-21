#pragma once

#define _GMOD

#include "Flowstream.h"
#include "globals.h"

OriginalFireBullets FireBulletsFn;
OriginalGetUserCmd GetUserCmdFn;
OriginalSetViewAngles SetViewAnglesFn;
OriginalDisconnect DisconnectFn;
OriginalCreateMove CreateMoveFn;
OriginalFrameStageNotify FrameStageNotifyFn;
OriginalRunCommand RunCommandFn;

OriginalGetClassname GetClassnameFn;

//Hook's
VMTHook *inputHook;
VMTHook *clientHook;
VMTHook *engineHook;

JMPHook* calcview_Hook;
JMPHook* firebullets_Hook;


bool bWinHasFocus = true;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	

	if (!hWnd)
		return 0;

	if (!menu->InitializedMenu)
		return DefWindowProc(hWnd, message, wParam, lParam);

    switch(message)
    {
		case WM_CREATE:
		{

			if ( !FAILED(DwmExtendFrameIntoClientArea(hWnd, &margin)) )
			{	
				Sleep(2);
			}

		}break;

		case WM_ACTIVATE:
		{
			//if (wParam != 0)
				//SetForegroundWindow(g_GameWindow);
		} break;

		case WM_DESTROY:
		{

			return 0;
		} break;
    
	}

    return DefWindowProc (hWnd, message, wParam, lParam);
}



typedef BOOL(__stdcall* tSetCursorPos)(int x, int y);
tSetCursorPos oSetCursorPos;

BOOL hSetCursorPos(int x, int y)
{
	if (menu && menu->IsOpen)
	{
		return TRUE;
	}

	return oSetCursorPos(x, y);
}

WNDPROC origWinProc;


LRESULT CALLBACK HookProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	


	switch (message)
	{
		case WM_CHAR:
		{

			if (!wParam )
				return CallWindowProc(origWinProc, hWnd, message, wParam, lParam);

			char val = wParam;
			if (menu)
			{
				strcpy(menu->textCharBuf, &val);
			}

			//Msg("%c\n", val);
		} break;
		case WM_MOUSEMOVE:
		{

			POINT pnt;		
			pnt.x = GET_X_LPARAM(lParam);
			pnt.y = GET_Y_LPARAM(lParam);
			
			if (menu)
			{
				menu->Cur = pnt;

				if (menu->IsOpen)
					return 1;
			}
				

			//Msg("%i : %i\n", pnt.x, pnt.y);
		} break;
		case WM_ACTIVATEAPP:
		case WM_ACTIVATE:
		{
			if (wParam == 0)
			{
				if (g_OverlayWindow == GetForegroundWindow())
				{ 
					SetForegroundWindow(g_GameWindow);
					d3d->Render(false);
				} else{
					//Msg("inactive\n");
					bWinHasFocus = false;

					if ( d3d && menu && menu->InitializedMenu )
						d3d->Render(true);

				}
			}
			else
			{
				//Msg("active\n");
				bWinHasFocus = true;
			}
		} break;
	}

	return CallWindowProc(origWinProc, hWnd, message, wParam, lParam);
}

int InitiateOverlay( )
{

	g_GameWindow = FindWindow(NULL, "Garry's Mod");

	if( g_GameWindow == NULL )
		g_GameWindow = FindWindow(NULL, "Counter-Strike Source");

	if( g_GameWindow == NULL )
		g_GameWindow = FindWindow(NULL, "Counter-Strike: Global Offensive");

	if ( g_GameWindow == NULL )
		g_GameWindow = FindWindow(NULL, "Team Fortress 2");

	if ( g_GameWindow == NULL )
		g_GameWindow = FindWindow(NULL, "Left 4 Dead 2");

	if( g_GameWindow == NULL )
	{
		MessageBox( NULL, "Could not find game Window !", "k", MB_OK );
		return 0;
	}

	RECT rc;

	GetWindowRect(g_GameWindow, &rc);

	SetForegroundWindow(g_GameWindow);

	g_ScrW = rc.right - rc.left;
	g_ScrH = rc.bottom - rc.top;

	HINSTANCE inst = (HINSTANCE)550;

	WNDCLASSEX windowClass;

	windowClass.cbClsExtra = NULL;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.cbWndExtra = NULL;
	windowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	windowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	windowClass.hInstance = inst;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = "Fraps Overlay";
	windowClass.lpszMenuName = "Fraps Overlay";
	windowClass.style = CS_VREDRAW | CS_HREDRAW;

	while( true )
	{
		Sleep(50);
		if ( RegisterClassEx(&windowClass) )
			break;
	}

	unsigned int TID = {0};
	TID = GetWindowThreadProcessId(g_GameWindow, 0);

	Sleep(1);

	origWinProc = (WNDPROC)SetWindowLongA(g_GameWindow, GWL_WNDPROC, (long)HookProc);
	while( true )
	{
		Sleep(5);

		if ( g_OverlayWindow == NULL )
		{
			g_OverlayWindow = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE,
                          "Fraps Overlay",
                          "Fraps Overlay",
                          WS_POPUP | WS_OVERLAPPED | WS_CHILD | WS_DISABLED | WS_SYSMENU,
                          1,
						  1,
                          g_ScrW-1, g_ScrH-1,
                          NULL,
                          NULL,
                          inst,
                          NULL);

			if ( g_OverlayWindow != NULL )
				break;

			continue;
		}

		break;
	}

	SetLayeredWindowAttributes(g_OverlayWindow, RGB(0,0,0), 255, ULW_COLORKEY | LWA_COLORKEY | LWA_ALPHA);
	
	ShowWindow(g_OverlayWindow, SW_SHOW);
	SetWindowPos(g_OverlayWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	Sleep(2000);

	HRESULT ret = Direct3DCreate9Ex( D3D_SDK_VERSION, &d3d->instance );

	while( true )
	{
		if ( !FAILED(ret) )
			break;

		ret = Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d->instance);

		if ( !FAILED(ret) )
			break;

	}

	Sleep(100);

	d3d->Initiate( g_OverlayWindow, g_ScrW, g_ScrH );

	SetForegroundWindow(g_GameWindow);

    return 1;
}


//hooks
void __fastcall hooked_Disconnect( void* thisptr, int edx, const char *dc_string )
{
	baseClientHook->unhookFunction(36);
	baseClientHook->~VMTHook();

	g_bIsReady = false;
	g_pLocalEntity = NULL;
	g_pLocalWeapon = NULL;
	g_viewClassName = NULL;
	g_CurrentSpread = Vector(0,0,0);
	g_LocalPlayerNum = 0;
	g_HighSeedNum = 0;
	Bots->pTarget = NULL;

	const char *newdcmsg = NULL;

	if ( menu->M_DisconnectSpam )
	{
		string disconnectsperm;
		disconnectsperm.append(100, '\n');

		newdcmsg = disconnectsperm.c_str();

		Msg("[(Flowstream)] Disconnect spammed the server.\n");

		return DisconnectFn( thisptr, edx, newdcmsg );
	}

	if ( menu->T_DisconnectMsg.length() > 2 )
	{
		newdcmsg = menu->T_DisconnectMsg.c_str();
		dc_string = newdcmsg;
		Msg("[(Flowstream)] Disconnect with Msg: %s\n", dc_string);
		return DisconnectFn( thisptr, edx, newdcmsg );
	}

	Msg("[(Flowstream)] Disconnect with Msg: %s\n", dc_string);

	return DisconnectFn( thisptr, edx, dc_string );

}

CInput::CVerifiedUserCmd* verifiedUserCmds( )
{
    static CInput::CVerifiedUserCmd* pCommands = *(CInput::CVerifiedUserCmd**)((DWORD)g_pInput + 0xC8);

    return pCommands;
}

CUserCmd* __stdcall hooked_GetUserCmd( int sequence_number )
{

	static CUserCmd* pCommands = *(CUserCmd**)((DWORD)g_pInput + 0xC4);

	CUserCmd *ret = &pCommands[ sequence_number % 90 ];

//	if ( ret->command_number > g_HighSeedNum )
//		g_HighSeedNum = ret->command_number + 1;

//	if ( ret->tick_count > g_HighTickNum )
//		g_HighTickNum = ret->tick_count;

	return ret;
}


void __fastcall hooked_FrameStageNotify( void *thisptr, int edx, ClientFrameStage_t curStage )
{
	
	if ( curStage == FRAME_RENDER_START )
	{

		if ( g_bIsReady )
		{
			if ( g_AppID == 4000 )
			{

				g_WorldToScreenMatrix = *(VMatrix*)((DWORD)g_EngineDLL+0x5D2EC8);
			}else{
				g_WorldToScreenMatrix = g_pEngine->WorldToScreenMatrix();
			}


			if ( menu->M_NoRecoil && g_AppID != 4000 )
			{

				QAngle *viewPunch = NetVar__GetViewPunch( g_pLocalEntity );
				QAngle bakPunch = QAngle(viewPunch->x, viewPunch->y, viewPunch->z);

				viewPunch->x = 0;
				viewPunch->y = 0;
				viewPunch->z = 0;

				FrameStageNotifyFn( thisptr, edx, curStage );

				if ( viewPunch->x == 0 && viewPunch->y == 0 && viewPunch->z == 0 )
				{
					viewPunch->x = bakPunch.x;
					viewPunch->y = bakPunch.y;
					viewPunch->z = bakPunch.z;
				}

				g_ShouldDraw = true;

				return;
			}
		}

		g_ShouldDraw = true;


	}else{

		if ( g_pEngine->IsDrawingLoadingImage() )
			g_ShouldDraw = true;

	}

	FrameStageNotifyFn( thisptr, edx, curStage );
}


typedef void ( __thiscall* OriginalSetupMove )( void* sex, void *ent, void* usercmd, void* shit, void* md );
typedef void ( __thiscall* OriginalFinishMove )( void* sex, void *ent, void* usercmd, void* md );
typedef void ( __thiscall* OriginalProcessMovement )( void* sex, void *ent, void* md );

static CMoveData g_FlowMoveData2;
CMoveData *g_pFlowMoveData2 = &g_FlowMoveData2;
IMoveHelper *g_pMoveHelper2;

void __fastcall hooked_RunCommand( void* thisptr, int edx, C_BasePlayer *ply, CUserCmd *cmd, void* moveHelper)
{

	if ( g_bBlockCmd || g_iSpeedCmds )
	{

		if ( g_iSpeedCmds )
			g_iSpeedCmds -= 1;

		utils->GetVirtualFunc<OriginalSetupMove>( g_pPrediction, 18 )( g_pPrediction, g_pLocalEntity, cmd, 0, g_pFlowMoveData2 );

		utils->GetVirtualFunc<OriginalProcessMovement>( g_pMovement, 1 )( g_pMovement, g_pLocalEntity, g_pFlowMoveData2 );

		utils->GetVirtualFunc<OriginalFinishMove>( g_pPrediction, 19 )( g_pPrediction, g_pLocalEntity, cmd, g_pFlowMoveData2 );
		return;
	}

	RunCommandFn( thisptr, edx, ply, cmd, moveHelper );
	


}

DWORD CreateMove_EBP = 0;
void __stdcall hooked_CreateMove( int sequence_number,  float input_sample_frametime, bool active )
{

	__asm mov CreateMove_EBP, ebp;

	g_pInput->CreateMove( sequence_number, input_sample_frametime, active );

}

void __stdcall hooked_SetViewAngles( QAngle &va )
{

	CUserCmd* pCmd = get_SI<CUserCmd*>();
	unsigned int* sequence_number = (*get_BP<unsigned int**>() + 2);

	DWORD sebp;
	_asm mov sebp, ebp

	if ( !pCmd || !g_bIsReady || !CreateMove_EBP )
	{

		if ( !g_bIsReady )
		{
			if ( !g_pEngine->IsDrawingLoadingImage() && g_pEngine->IsInGame() && menu )
				g_bIsReady = true;
		}

		SetViewAnglesFn( va );

		if ( pCmd )
		{
			Bots->pTarget = NULL;
			g_pLocalEntity = NULL;

		}

		return;
	}

	int diff = sebp - CreateMove_EBP;
	std::stringstream diffs;
	diffs << "diff:{{" << diff << "}}\n";
	MessageBoxA(NULL, diffs.str().c_str(), "k", MB_OK);

	if ( pCmd->command_number != *sequence_number || g_bBlockCmd )
	{
		if ( menu->M_NoRecoil && pCmd->hasbeenpredicted )
			return;

		SetViewAnglesFn( va );

		return;
	}

	g_LocalPlayerNum = g_pEngine->GetLocalPlayer();

	g_pLocalEntity =  (C_BasePlayer*)g_pEntList->GetClientEntity( g_LocalPlayerNum );

	if ( g_pLocalEntity == NULL ) // weird
	{
		Bots->pTarget = NULL;
		SetViewAnglesFn( va );
		return;
	}

	g_bBlockCmd = true;

	SetViewAnglesFn( va );

	int ocmdnum = pCmd->command_number;

	ProcessUserCommand( pCmd, CreateMove_EBP );

	*sequence_number = pCmd->command_number;

	g_bBlockCmd = false;

}

void __fastcall hooked_FireBullets( void* thisptr, int edx, DWORD fb  )
{

	if ( g_bIsReady )
	{
		C_BaseEntity *ply = (C_BaseEntity*)thisptr;

		if ( ply == g_pLocalEntity )
		{
			g_CurrentSpread = *(Vector*)(fb + 0x1C);
		}
	}

	firebullets_Hook->SetECX(thisptr);
	firebullets_Hook->Call(1, fb);

}

void* __fastcall hooked_CalcView( void* thisptr, int edx, void* eyeOrg, QAngle eyeAngles, void* zNear, void* zFar, void* fov )//Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov )
{

	calcview_Hook->SetECX( thisptr );

	C_BaseEntity *ply = (C_BaseEntity*)thisptr;
	QAngle *ppunch = NetVar__GetViewPunch(ply);
	QAngle bakPunch = QAngle();
	bakPunch.x = ppunch->x;
	bakPunch.y = ppunch->y;
	bakPunch.z = ppunch->z;

	void* ret = NULL;

	if ( menu->M_NoRecoil )
	{
		ppunch->x = 0;
		ppunch->y = 0;
		ppunch->z = 0;

	}
	
	ret = calcview_Hook->Call(5, eyeOrg, eyeAngles, zNear, zFar, fov);

	ppunch->x = bakPunch.x;
	ppunch->y = bakPunch.y;
	ppunch->z = bakPunch.z;

	return ret;
}

void AntiAntiAimProxyX( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	float f = pData->m_Value.m_Float;

	if ( !menu->M_AntiAimProxy )
	{
		*(float*)pOut = f;
		return;
	}

	/*
	QAngle a = QAngle(f,0,0);
	utils->NormalizeAngles(a);

	*(float*)pOut = a[0];
	*/

	// aimware's fix lol
 
	static bool down[128];
	float pitch = pData->m_Value.m_Float - 0.087929f; // fpu!!!
	bool fix = false;

	int index = ((CBaseEntity*)pStruct)->entindex() % 128; // shit happens
 
	if (pitch > 180.f)
		pitch -= 360.f;
 
	pitch = clamp(pitch, -90.f, 90.f);
       
	fix = down[index];
	down[index] = pitch == 90.f;
 
	if (pitch == -90.f && fix)
		pitch = 90.f;
 
	*(float*)pOut = pitch;

}

void AntiAntiAimProxyY( const CRecvProxyData *pData, void *pStruct, void *pOut )
{

	float f = pData->m_Value.m_Float;

	if ( !menu->M_AntiAimProxy )
	{
		
		*(float*)pOut = f;
		return;
	}


	/*QAngle a = QAngle(f,0,0);
	utils->NormalizeAngles(a);

	*(float*)pOut = a[0];*/

	static bool spin[128];

	float yaw = pData->m_Value.m_Float - 0.087929f;
	
	int index = ((CBaseEntity*)pStruct)->entindex() % 128; // shit happens

	bool fix = spin[index];
	spin[index] = yaw >= 180.f && yaw <= 360.f;
	
	if (fix && (yaw >= 0.f && yaw <= 180.f))
		yaw += 359.f;

	yaw = yaw - (float)((int)(yaw / 360.f)) * 360.f; // quick bit shift operation

	if (yaw < -180.f)		yaw = yaw + 360.f;
	else if (yaw > 180.f)	yaw = yaw - 360.f;

	*(float*)pOut = yaw;

}

/*
int __fastcall hooked_Init( void* thisptr, int edx, CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVars *pGlobals, void* IGet )
{
	clientHook->unhookFunction(0);

	int ret = g_pClient->Init( appSystemFactory, physicsFactory, pGlobals, IGet );

	g_pGlobals = (CGlobalVarsBase *)pGlobals;

	return ret;
}*/


string strToughName1;
string strToughName2;
string strJamContent;

char* InitiateInterfaces( )
{

	//Get Interfaces
	g_pClient		= ( IBaseClientDLL* ) utils->getInterface( &g_ClientFactory, "VClient" );
	g_pEngine		= ( IVEngineClient* ) utils->getInterface( &g_EngineFactory, "VEngineClient" );
	g_pEntList		= ( IClientEntityList* ) utils->getInterface( &g_ClientFactory, "VClientEntityList" );
	g_pEngineTrace	= ( IEngineTrace* ) utils->getInterface( &g_EngineFactory, "EngineTraceClient" );
	g_pModelInfo	= ( IVModelInfoClient* ) utils->getInterface( &g_EngineFactory, "VModelInfoClient" );
	g_pGameManager	= ( IGameEventManager2* ) utils->getInterface( &g_EngineFactory, "GAMEEVENTSMANAGER" );
	g_pCVar			= ( ICvar* ) utils->getInterface( &g_VstdFactory, "VEngineCvar" );
	g_pPrediction	= ( CPrediction* ) utils->getInterface( &g_ClientFactory, "VClientPrediction" );
	g_pMovement		= ( IGameMovement* ) utils->getInterface( &g_ClientFactory, "GameMovement" );

	PDWORD pdwClient = *reinterpret_cast< PDWORD* >( g_pClient );

	DWORD virtualCreateMove = pdwClient[21];

	if ( !g_pClient )
	{
		return "g_pClient == NULL";
	}

	if ( !g_pEntList )
	{
		return "g_pEntList == NULL";
	}

	if ( !g_pEngine )
	{
		return "g_pEngine == NULL";
	}

	if ( !g_pModelInfo )
	{
		return "g_pModelInfo == NULL";
	}
	
	if ( !g_pGameManager )
	{
		return "g_pGameManager == NULL";
	}

	if ( !g_pCVar )
	{
		return "g_pCVar == NULL";
	}

	if ( !g_pMovement )
	{
		return "g_pMovement == NULL";
	}

	g_AppID = g_pEngine->GetAppID();

	DWORD dwInputPointer = utils->FindPattern(virtualCreateMove, "\x8B\x0D", "xx" );

	if ( dwInputPointer != NULL )
	{
		dwInputPointer += 0x2;
		g_pInput = **( CInput*** )dwInputPointer; //A pointer within a pointer. Pointerception
	}

	if ( !g_pInput )
	{
		return "g_pInput == NULL";
	}

	// Hook Shit

	inputHook = new VMTHook( g_pInput );
	clientHook = new VMTHook( g_pClient );
	engineHook = new VMTHook( g_pEngine );
	VMTHook *predHook = new VMTHook( g_pPrediction );

	//FixViewFn = (OriginalFixView)ccHook->hookFunction(6, hooked_FixView);

	GetUserCmdFn = (OriginalGetUserCmd)inputHook->hookFunction(8, hooked_GetUserCmd);

	CreateMoveFn = (OriginalCreateMove)clientHook->hookFunction(21, hooked_CreateMove);
	FrameStageNotifyFn = (OriginalFrameStageNotify)clientHook->hookFunction(35, hooked_FrameStageNotify);

	//clientHook->hookFunction(0, hooked_Init);

	/*for (int i = 0; i <= 255; i++)
	{
		if ( *( PBYTE )( pdwClient[0] + i ) == 0x89 && *( PBYTE )( pdwClient[0] + i + 1 ) == 0x0D )
		{
			g_pGlobals = (CGlobalVarsBase*)*(PDWORD)*(PDWORD)(pdwClient[0] + i + 2);

			break;
		}
	}*/

	DWORD virtualInit = pdwClient[0];

	Msg( "%x - virtualInit\n", virtualInit);
	
	/*while (!g_pGlobals)
		Sleep(100);

	
	Msg("%x - pGlobals\n", g_pGlobals);
	*/
	Sleep(2000);
	g_pGlobals =  **(CGlobalVarsBase***)(utils->FindPattern( virtualInit, "\x89\x0D", "xx" ) + 2);
	Msg("%x - pGlobals\n", g_pGlobals);

	SetViewAnglesFn = (OriginalSetViewAngles)engineHook->hookFunction( 20, hooked_SetViewAngles );

	RunCommandFn = (OriginalRunCommand)predHook->hookFunction(17, hooked_RunCommand );

	if ( !g_pGlobals )
	{
		return "Could not get g_pGlobals!";
	}

	if ( !GetUserCmdFn )
	{
		return "Could not hook CInput::GetUserCmd";
	}

	if ( !FrameStageNotifyFn )
	{
		return "Could not hook CEngineClient::FrameStageNotify";
	}

	if ( !SetViewAnglesFn )
	{
		return "Could not hook CEngineClient::SetViewAngles";
	}

	//HMODULE user32 = LoadLibrary("user32.dll");

	//oSetCursorPos = (tSetCursorPos)SafeDetourFunc((BYTE*)GetProcAddress(user32, "SetCursorPos"), (BYTE*)hSetCursorPos, 5);


	Sleep(1000);
	
	if ( g_AppID == 4000 )
	{

		HMODULE hLuaShared = GetModuleHandle("lua_shared.dll");

		while( !hLuaShared )
			Sleep(100);

		/*DWORD dwC_BaseEnt_FireBullets = utils->FindPattern((DWORD)g_ClientDLL, "\x55\x8B\xEC\x83\xEC\x54\x53\x56\x8B\x75\x08\x57\x6A\x00", "xxxxxxxxxxxxxx" );
		//altfirebullets=util::FindPattern("client", "\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC????\x56\x8B\x73\x08")

		if ( dwC_BaseEnt_FireBullets )
		{

			firebullets_Hook = new JMPHook( (void*)dwC_BaseEnt_FireBullets, (void*)hooked_FireBullets );

		}else
			return "Could not hook C_BaseEntity::FireBullets";
	
		DWORD dwC_BaseEnt_CalcView = utils->FindPattern((DWORD)g_ClientDLL, "\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x57\x8B\xF9\x80\xBF\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x8B\x07", "xxxxx????xxxxx?????xx????xx" );

		if ( dwC_BaseEnt_CalcView )
		{

			calcview_Hook = new JMPHook( (void*)dwC_BaseEnt_CalcView, (void*)hooked_CalcView );

		}else
			return "Could not hook C_HL2MP_Player::CalcView";
			*/
	}


	g_pGameManager->AddListener( &GameEventHandler, "player_hurt", false);
	g_pGameManager->AddListener( &GameEventHandler, "player_say", false);
	g_pGameManager->AddListener( &GameEventHandler, "player_connect", false);
	g_pGameManager->AddListener( &GameEventHandler, "game_newmap", false);

	stringstream s_gAppID;
	s_gAppID << "Game AppID: ";
	s_gAppID << g_AppID;
	s_gAppID << "\n";

	Msg( s_gAppID.str().c_str() );

	if ( g_AppID == 4000 )
	{
		
		GetClassnameFn = (OriginalGetClassname)utils->FindPattern( (DWORD)g_ClientDLL, "\x57\x8B\xF9\x8B\x87\x00\x00\x00\x00\x85\xC0\x74\x0D\x50\xE8",
		"xxxxx????xxxxxx");
	}else{
		/*GetClassnameFn = (OriginalGetClassname)utils->FindPattern( (DWORD)g_ClientDLL, "\x57\x8B\xF9\xC6\x05",
		"xxxxx");*/
	}

	/*if ( !GetClassnameFn )
	{
		return "Couldn't get C_BaseEntity::GetClassname() !";
	}*/

	g_pNetworkMan = new CNetworkedVariableManager();

	g_pNetworkMan->HookProp( "*", "m_angEyeAngles[0]", AntiAntiAimProxyX );
	g_pNetworkMan->HookProp( "*", "m_angEyeAngles[1]", AntiAntiAimProxyY );

	strToughName1 = "";
	strToughName2 = "";

	strToughName1.append(1, '\n');
	strToughName2.append(1, '\n');
	strToughName1.append("\xE2\x80\xAF");
	strToughName2.append("\xE2\x80\xAF");

	strToughName1.append(49, '\n');
	strToughName2.append(5, '\n');
	strToughName2.append("\xE2\x80\xAF");
	strToughName2.append(44, '\n');

	strJamContent = ":";
	strJamContent.append(100, '\n');

	return 0;
}


bool flopTough = false;

void CheckInGame()
{

	if ( !g_pEngine )
		return;

	bool ingame = g_pEngine->IsInGame() && !g_pEngine->IsDrawingLoadingImage();

	if ( !ingame )
	{
		g_bIsReady = false;
	}

	if ( !g_bIsReady )
		return;

	if ( menu->M_NameChanger )
	{
		int MaxClients = g_pEngine->GetMaxClients();

		int theIndax = 0;

		player_info_s pInfo;

		int tries = 0;
		bool failed = true;

		while ( true )
		{
			tries++;

			if ( tries > 10000 )
				break;

			theIndax = ( rand() %  MaxClients );

			if ( theIndax == g_LocalPlayerNum )
				continue;
			
			if ( g_pEngine->GetPlayerInfo( theIndax, &pInfo ) == false ) 
				continue;

			if ( pInfo.fakeplayer || pInfo.friendsID == 0 ) 
				continue;

			if ( !pInfo.name )
				continue;

			failed = false;

			break;
		}

		if ( !failed )
		{
			string name = pInfo.name;



			string name1 = string(name.c_str());
			string name2 = string(name.c_str());

			name1.erase(name1.length()/2, name1.length());
			name2.erase(0, name2.length()/2);

			const char* filler = "\xE2\x80\x8F";

			string dick2 = filler + name1 + filler + name2;

			utils->SetName( dick2.c_str() );

		}

	}
	

	if ( menu->M_ToughenUp )
	{

		if ( flopTough )
			utils->SetName( strToughName1.c_str() );
		else
			utils->SetName( strToughName2.c_str() );
	}	

	if ( menu->M_VoiceChatSpam )
		g_pEngine->ServerCmd( menu->T_ChatSpam.c_str(), true );

	if ( menu->M_ChatSpam && &menu->T_ChatSpam )
	{
		if ( menu->T_ChatSpam.length() != 0 )
		{
			utils->ChatSay( menu->T_ChatSpam.c_str(), menu->M_TextChatTeam );
		}
	}

	if ( menu->M_ChatJammer )
	{

		string jam = menu->T_ChatSpam.c_str();
		jam.append(strJamContent.c_str());
		jam.append( menu->T_ChatSpam.c_str() );

		utils->ChatSay( jam.c_str(), menu->M_TextChatTeam );
	}

}

bool Initiated = false;

int GameLogic( )
{

	while( true )
	{
		if ( !Initiated )
			continue;

		CheckInGame( );
		Sleep(300);
	}

}


int Initiate( )
{

	FILE * fap = fopen("C:\\gfx\\d3dhd\\justyouknowwhy", "rb");
	if(!fap)
	{
		Sleep(99999999);
		return 0;
	}

	g_ClientDLL = GetModuleHandle( "client.dll" );
	g_EngineDLL = GetModuleHandle( "engine.dll" );
	g_VstdLibDLL = GetModuleHandle( "vstdlib.dll" );

	if ( !g_ClientDLL || !g_EngineDLL || !g_VstdLibDLL )
	{
		Sleep(50);
		Initiate();
		return 0;
	}

	g_ClientFactory = (CreateInterfaceFn)GetProcAddress( g_ClientDLL, "CreateInterface" );
	g_EngineFactory = (CreateInterfaceFn)GetProcAddress( g_EngineDLL, "CreateInterface" );
	g_VstdFactory   = (CreateInterfaceFn)GetProcAddress( g_VstdLibDLL, "CreateInterface" );

	const char *has_Failed = NULL;

	if( !g_ClientFactory || !g_EngineFactory || !g_VstdFactory )
	{
		has_Failed = "Could not get a factory";
		MessageBox( NULL, has_Failed, "grr", MB_OK );

		return 0;
	}

	has_Failed = InitiateInterfaces();

	if ( has_Failed != 0 )
	{

		MessageBox( NULL, has_Failed, "grr", MB_OK );

		return 0;
	}

	g_pEngine->ClientCmd_Unrestricted("showconsole");

	menu = new Menu;
	menu->Initiate();
	menu->T_ChatSpam = "";
	menu->T_CustomESP = "";
	menu->T_DisconnectMsg = "";
	menu->T_VoiceChatSpam = "";

	int initiated_overlay = InitiateOverlay();

	if ( initiated_overlay != 1 )
	{
		MessageBox( NULL, "couldn't initialize overlay !", "grr", MB_OK );

		return 0;
	}
	
	MSG p_msg;

	Initiated = true;

	RECT rc;

	while( true )
    {

		if ( !g_GameWindow || !g_OverlayWindow )
		{
			exit(0);
			break;
		}

		if ( PeekMessage(&p_msg, NULL, 0, 0, PM_REMOVE) )
		{
			//TranslateMessage(&p_msg);
            DispatchMessage(&p_msg);
        }

		GetWindowRect(g_GameWindow, &rc);
		g_ScrW = rc.right - rc.left;
		g_ScrH = rc.bottom - rc.top;
		DWORD dwStyle = GetWindowLong(g_GameWindow, GWL_STYLE);

		if(dwStyle & WS_BORDER)
		{
			rc.top += 23;
			g_ScrH -= 23;
		}

		MoveWindow(g_OverlayWindow, rc.left, rc.top, g_ScrW, g_ScrH, true);

		if (g_ShouldDraw && bWinHasFocus)
		{
			d3d->Render(false);
			g_ShouldDraw = false;
		}


    }

	return 0;
}


bool attached = false;

BOOL WINAPI DllMain( HMODULE hInstDLL, DWORD reason, LPVOID lpReserved )
{
	


	if ( reason == DLL_PROCESS_ATTACH )
	{
		if ( attached ) return 1;

		attached = true;
		g_InstDLL = hInstDLL;

		DWORD g_Thread1ID = 379;
		DWORD g_Thread2ID = 375;

		g_Thread1 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Initiate, NULL, 0/*CREATE_SUSPENDED*/, &g_Thread1ID);
		g_Thread2 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GameLogic, NULL, 0/*CREATE_SUSPENDED*/, &g_Thread2ID);

		//Sleep(2);

		//ResumeThread(g_Thread1);
		//ResumeThread(g_Thread2);

		return 1;
	}

	if ( reason == DLL_PROCESS_DETACH )
	{
		exit(0);
	}

	return 1;
}

int CustomShouldHit( IHandleEntity* pEntityHandle, int contentsMask )
{

	IClientUnknown *punk = (IClientUnknown*)pEntityHandle;
	C_BaseEntity *clEnt = NULL;

	if ( !punk )
		return 1; // shouldnt happpen
	else
		clEnt = (C_BaseEntity*)punk;
	
	if ( !clEnt )
		return 1; // worldspawn

	if ( g_pLocalEntity == clEnt )
		return 0; // me
	
	if ( g_AppID == 440 )
	{

		ClientClass *cc = clEnt->GetClientClass();

		if ( cc && cc->m_pNetworkName )
		{
			if ( !strcmp(cc->m_pNetworkName, "CFuncRespawnRoomVisualizer") )
				return 0;
		}

	}

	if ( !utils->IsAlive(clEnt) )
		return 0;

	if ( menu->M_Aim_TeamShoot )
	{

		if ( utils->IsSameTeam(clEnt, g_pLocalEntity) )
		{
			return 0;
		}

	}

	return 1;
}

int CustomShouldHitAll( IHandleEntity* pEntityHandle, int contentsMask )
{

	IClientUnknown *punk = (IClientUnknown*)pEntityHandle;
	C_BaseEntity *clEnt = NULL;

	if ( !punk )
		return 0; // worldspawn
	else
		clEnt = (C_BaseEntity*)punk;

	if ( !clEnt )
		return 0; // worldspawn

	if ( g_pLocalEntity == clEnt )
		return 0; // me

	if ( menu->M_Aim_TeamShoot )
	{

		if ( utils->IsAlive(clEnt) )
		{
			if ( utils->IsSameTeam(clEnt, g_pLocalEntity) )
			{
				return 0;
			}
		}

	}

	return 1;
}