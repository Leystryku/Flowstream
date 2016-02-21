#pragma once

#include "d3d.h"
#include "menu.h"

m_d3d *d3d = new m_d3d;

void m_d3d::DrawText( string pString, int x, int y, D3DCOLOR col, ID3DXFont *font)
{
	if ( !device )
		return;

	if ( !font )
		return;

	if ( !&pString )
		return;

	LPCSTR lpcStr = pString.c_str();

	bNeedsFlush = true;

	RECT FontRect = { x, y, x, y };

	font->DrawText( NULL, lpcStr, strlen(lpcStr), &FontRect, DT_NOCLIP, col);

}

void m_d3d::DrawFilledRectangle( int X, int Y, int Width, int Height, DWORD dwColor )
{
	if( !device )
		return;

    if( bNeedsFlush )
    {
        sprite->Flush();
        bNeedsFlush = FALSE;
    }

    CVertexList VertexList[4] = 
    {
        { X, Y + Height, 0, dwColor },
        { X, Y, 0, dwColor },
        { X + Width, Y + Height, 0, dwColor },
        { X + Width, Y, 0, dwColor },
    };

    device->SetTexture( 0, NULL );
    device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, VertexList, sizeof( CVertexList ) );
}

void m_d3d::DrawGradientRect( int X, int Y, int Width, int Height, DWORD startCol, DWORD endCol )
{
	if( !device )
		return;

    if( bNeedsFlush )
    {
        sprite->Flush();
        bNeedsFlush = FALSE;
    }

    CVertexList VertexList[4] = 
    {
        { X, Y + Height, 0, endCol },
        { X, Y, 0, startCol },
        { X + Width, Y + Height, 0, endCol },
        { X + Width, Y, 0, startCol },
    };
 
    device->SetTexture( 0, NULL );
    device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, VertexList, sizeof( CVertexList ) );
}

void m_d3d::DrawBorder( int x, int y, int w, int h, DWORD color1, DWORD color2 )
{
	if( !device )
		return;

    if( bNeedsFlush )
    {
        sprite->Flush();
        bNeedsFlush = FALSE;
    }

	DXUT_SCREEN_VERTEX vertices[6] =
	{
		x + w, y, 0, 1, color2, 0,0,
		x + w, y + h, 0, 1, color2, 0,0,
		x, y + h, 0, 1, color2, 0,0,

		x, y + h, 0, 1, color1, 0,0,
		x, y, 0, 1, color1, 0,0,
		x + w, y, 0, 1, color1, 0,0,
	};

	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
    device->SetTexture( 0, NULL );
	device->DrawPrimitiveUP( D3DPT_LINESTRIP, 5, vertices, sizeof(DXUT_SCREEN_VERTEX) );
	device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
}

void m_d3d::DrawNonFilledRectangle( int x, int y, int w, int h, DWORD dwColor )
{
	if( !device )
		return;

    if( bNeedsFlush )
    {
        sprite->Flush();
        bNeedsFlush = FALSE;
    }

	DXUT_SCREEN_VERTEX vertices[6] =
	{
		x + w, y,	  0, 1, dwColor, 0, 0,
		x + w, y + h, 0, 1, dwColor, 0, 0,
		x, y + h, 0, 1, dwColor, 0, 0,

		x, y + h, 0, 1, dwColor, 0, 0,
		x, y,	  0, 1, dwColor, 0, 0,
		x + w, y, 0, 1, dwColor, 0, 0,
	};

	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
	device->SetTexture( 0, NULL );
	device->DrawPrimitiveUP( D3DPT_LINESTRIP, 5, vertices, sizeof(DXUT_SCREEN_VERTEX) );
	device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
}

void m_d3d::DrawBorderWithFill( int x, int y, int w, int h, DWORD color1, DWORD color2, DWORD Fill )
{
	if( !device )
		return;

	DXUT_SCREEN_VERTEX vertices[6] =
	{
		x + w, y, 0, 1, color2, 0,0,
		x + w, y + h, 0, 1, color2, 0,0,
		x, y + h, 0, 1, color2, 0,0,

		x, y + h, 0, 1, color1, 0,0,
		x, y, 0, 1, color1, 0,0,
		x + w, y, 0, 1, color1, 0,0,
	};

	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
    device->SetTexture( 0, NULL );
	device->DrawPrimitiveUP( D3DPT_LINESTRIP, 5, vertices, sizeof(DXUT_SCREEN_VERTEX) );
	DrawFilledRectangle( x + 1, y + 1, w - 1, h - 1, Fill );
	device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

}


void m_d3d::DrawLine( int x, int y, int x2, int y2, int width, DWORD color)
{

	if ( !line )
		return;

	float flw = (float)width;
	float flx = (float)x;
	float fly = (float)y;
	float flx2 = (float)x2;
	float fly2 = (float)y2;

    D3DXVECTOR2 vLine[2];
    line->SetWidth( flw );
    line->SetAntialias( false );
    line->SetGLLines( true );
    vLine[0].x = flx;
    vLine[0].y = fly;
    vLine[1].x = flx2;
    vLine[1].y = fly2;
    line->Begin();
    line->Draw( vLine, 2, color );
    line->End();
}

void m_d3d::DrawColorFill( int x, int y, int w, int h, D3DCOLOR color )
{
	if ( !device )
		return;
	
	if ( bNeedsFlush )
	{
        if ( sprite )
			sprite->Flush();
       
		bNeedsFlush = FALSE;
    }

	if ( !x )x = 0;
	if ( !y )y = 0;
	if ( !w )w = 0;
	if ( !h )h = 0;

    if( w < 0 )w = 1;
    if( h < 0 )h = 1;
    if( x < 0 )x = 1;
    if( y < 0 )y = 1;

	D3DRECT rec = { x, y, x + w, y + h };
	device->Clear( 1, &rec, D3DCLEAR_TARGET, color, 0, 0 );
}

void m_d3d::DrawColorFillOutline( int x, int y, int w, int h, int lw, D3DCOLOR color)
{

	DrawColorFill( x, y, w, lw, color );
	DrawColorFill( x, y, lw, h, color );
	DrawColorFill( x, y + h, w + lw, lw, color );
	DrawColorFill( x + w, y, lw, h, color );

}

void m_d3d::DrawCircle( int X, int Y, int radius, int numSides, DWORD Color) 
{ 

    D3DXVECTOR2 Line[128];
	float pi = (float)PI;
    float Step = pi * 2.0f / numSides; 
    int Count = 0; 
    for (float a=0; a < PI*2.0; a += Step) 
    { 
        float X1 = radius * cos(a) + X; 
        float Y1 = radius * sin(a) + Y; 
        float X2 = radius * cos(a+Step) + X; 
        float Y2 = radius * sin(a+Step) + Y; 
        Line[Count].x = X1; 
        Line[Count].y = Y1; 
        Line[Count+1].x = X2; 
        Line[Count+1].y = Y2; 
        Count += 2; 
    } 
    line->Begin(); 
    line->Draw(Line,Count,Color); 
    line->End(); 
}

void m_d3d::DrawPoint(int x, int y, int w, int h, DWORD color )
{
    DrawColorFill((int)x, (int)y, (int)w, (int)h, color );

}

void m_d3d::DrawBorder( int x, int y, int w, int h, int px, D3DCOLOR BorderColor )
{
    DrawColorFill( x, (y + h - px), w, px,    BorderColor );
    DrawColorFill( x, y, px, h,                BorderColor );
    DrawColorFill( x, y, w, px,                BorderColor );
    DrawColorFill( (x + w - px), y, px, h,    BorderColor );
}

void m_d3d::DrawBox( int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BorderColor )
{
    DrawColorFill( x, y, w, h,        BoxColor );
    DrawBorder( x, y, w, h, 1,    BorderColor );
}

void m_d3d::Initiate( HWND hWnd, int ScrW, int ScrH )
{
    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;     // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = ScrW;    // set the width of the buffer
    d3dpp.BackBufferHeight = ScrH;    // set the height of the buffer
	d3dpp.MultiSampleQuality   = D3DMULTISAMPLE_NONE;
	d3dpp.Flags = D3DPRESENTFLAG_VIDEO | D3DPRESENTFLAG_RESTRICTED_CONTENT;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT res = instance->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
                      &d3dpp,
                      &device);

	while( true )
	{
		if ( !FAILED(res) )
			break;

		res = instance->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
                      &d3dpp,
                      &device);

		if ( !FAILED(res) )
			break;
	}

	D3DXCreateSprite( device, &sprite );
	D3DXCreateLine(  device, &line );

	D3DXCreateFontA( device, 12, 5, FW_THIN, 20, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Tahoma",&font1);
	D3DXCreateFontA( device, 30, 15, FW_BOLD, 20, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Arial",&font2);
	D3DXCreateFontA( device, 12, 5, FW_THIN, 20, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Tahoma",&font3);

}


void m_d3d::Render( bool bReset )
{
	if (!device) return;

	if ( !bReset )
	{
		bFlushDraw = true;
		device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1, 0);
		device->BeginScene();

			sprite->Begin( D3DXSPRITE_ALPHABLEND );

			device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

			DoDraw( );

			sprite->End();

		device->EndScene();
		device->Present(0, 0, 0, 0 );
	}

	if ( bReset && bFlushDraw )
	{
		device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1, 0);
		device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1, 0);
		device->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1, 0);

		device->BeginScene();
		device->EndScene();
		device->Present(0, 0, 0, 0 );
		bFlushDraw = false;
	}

}

void m_d3d::DrawCrosshair( )
{

	int CenterX = g_ScrW/2;
	int CenterY = g_ScrH/2;

	switch( menu->S_CrossHair )
	{
	case 1:

		DrawColorFill(CenterX-15, CenterY, 30, 1, Red);
		DrawColorFill(CenterX, CenterY-15, 1, 30, Red);
		break;
	case 2:
		DrawLine(CenterX+10, CenterY+10, CenterX-10, CenterY-10, 2, Red);
		DrawLine(CenterX-10, CenterY+10, CenterX+10, CenterY-10, 2, Red);
		break;
	case 3:
		DrawColorFill(CenterX-15, CenterY, 10, 1,Red);
		DrawColorFill(CenterX+6, CenterY, 10, 1,Red);
		DrawColorFill(CenterX, CenterY-15, 1, 10,Red);
		DrawColorFill(CenterX, CenterY+6, 1, 10,Red);
		DrawPoint(CenterX,CenterY,1,1,Green);
		break;
	case 4:
		DrawLine(CenterX+15,CenterY+3,CenterX+3,CenterY+3,2,Red);
		DrawLine(CenterX-15,CenterY+3,CenterX-3,CenterY+3,2,Red);
		DrawLine(CenterX+15,CenterY-3,CenterX+3,CenterY-3,2,Red);
		DrawLine(CenterX-15,CenterY-3,CenterX-3,CenterY-3,2,Red);
		DrawPoint(CenterX,CenterY,1,1,Green);
		break;
	case 5:
		DrawLine(CenterX+15,CenterY+15,CenterX+3,CenterY+3,2,Red);
		DrawLine(CenterX-15,CenterY+15,CenterX-3,CenterY+3,2,Red);
		DrawLine(CenterX+15,CenterY-15,CenterX+3,CenterY-3,2,Red);
		DrawLine(CenterX-15,CenterY-15,CenterX-3,CenterY-3,2,Red);
		DrawPoint(CenterX,CenterY,1,1,Green);
		break;
	}

}

void m_d3d::DrawLivingESP( C_BaseEntity *pBaseEntity, const char *name, bool is_npc )
{
	if ( !pBaseEntity || !name )
		return;

	Vector AbsScreen = Vector(0,0,0);
	Vector origin = pBaseEntity->GetAbsOrigin();

	if ( origin.x == 0 && origin.y == 0 && origin.z == 0 )
		return;

	if( utils->WorldToScreen( origin, AbsScreen ) == false )
		return;

	if ( AbsScreen.x == 0 && AbsScreen.y == 0 )
		return;

	if ( is_npc && g_AppID == 4000 )
		name = NetVar__GetClassname( pBaseEntity );

	D3DCOLOR DrawColor = D3DCOLOR_RGBA( 0, 107, 60, 255 );
	
	int health = 0;
	
	health = NetVar__GetHealth( pBaseEntity );
	
	if ( !utils->IsAlive(pBaseEntity) )
	{
		DrawColor = D3DCOLOR_RGBA( 0, 0, 0, 255 );
	}else{

		if ( !is_npc )
			DrawColor = utils->GetTeamColor( pBaseEntity );

	}

	char c_health[11];
	sprintf(c_health, "%d", health);

	if ( menu->M_ESP_Name )
		DrawText( name, AbsScreen.x, AbsScreen.y, DrawColor, font1);

	if ( menu->M_ESP_Health )
		DrawText( c_health, AbsScreen.x, AbsScreen.y + 8.0f, DrawColor, font1);

	if ( menu->M_ESP_Box && !is_npc )
	{
		Vector mon, nom;

		int flags = NetVar__GetFlags( pBaseEntity );

		if ( !flags )
			return;

		nom = pBaseEntity->GetAbsOrigin();


		if (flags & FL_DUCKING)
		{
			mon = nom + Vector( 0, 0, 50.f );
		}
		else
		{
			mon = nom + Vector( 0, 0, 70.f );
		}

		Vector bot, top;

		if( utils->WorldToScreen( nom, bot ) && utils->WorldToScreen( mon, top ) )
		{
			float h = ( bot.y - top.y );

			float w = h / 4.f;

			DrawColorFillOutline( top.x - w, top.y, w * 2, ( bot.y - top.y ), 1, DrawColor );
		}
	}

}

void m_d3d::DrawWeaponESP( C_BaseEntity *pBaseEntity, const char *name )
{

	if ( !pBaseEntity || !name )
		return;

	Vector AbsScreen = Vector(0,0,0);
	Vector origin = NetVar__GetOrigin(pBaseEntity);

	if ( origin.x == 0 && origin.y == 0 && origin.z == 0 )
		return;

	if( utils->WorldToScreen( origin, AbsScreen ) == false )
		return;

	if ( AbsScreen.x == 0 && AbsScreen.y == 0 )
		return;

	name = g_pModelInfo->GetModelName( pBaseEntity->GetModel() );

	string strName = name;
	
	strName.erase(0, strName.find("/w_")+3);

	strName.erase(strName.find(".mdl"));

	strName[0] = toupper( strName[0] );

	for( std::size_t i = 1 ; i < strName.length() ; ++i )
	{
		if ( !strName[i] )
			continue;

		strName[i] = toupper( strName[i] );

		if ( strName[i] == '_' )
			strName[i] = ' ';

	}

	name = strName.c_str();

	D3DCOLOR DrawColor = D3DCOLOR_RGBA(255, 255, 0, 255);

	DrawText( name, AbsScreen.x, AbsScreen.y, DrawColor, font1);
}

void m_d3d::DrawCustomESP( C_BaseEntity *pBaseEntity, const char *name )
{

	if ( !pBaseEntity || !name )
		return;

	if ( !strstr(menu->T_CustomESP.c_str(), name) )
		return;

	Vector AbsScreen = Vector(0,0,0);
	Vector origin = pBaseEntity->GetAbsOrigin();

	if ( origin.x == 0 && origin.y == 0 && origin.z == 0 )
	{
		origin = NetVar__GetOrigin(pBaseEntity);

		if ( origin.x == 0 && origin.y == 0 && origin.z == 0 )
			return;
	}

	if( utils->WorldToScreen( origin, AbsScreen ) == false )
		return;

	if ( AbsScreen.x == 0 && AbsScreen.y == 0 )
		return;

	D3DCOLOR DrawColor = D3DCOLOR_RGBA(255, 255, 0, 255);

	DrawText( name, AbsScreen.x, AbsScreen.y, DrawColor, font1);

}

void m_d3d::DrawEntESP( )
{

	if( !g_bIsReady )
		return;

	int HighestEntityIndex = g_pEntList->GetHighestEntityIndex();

	for( int i = 0; i < HighestEntityIndex; i++ )
	{

		if( i == g_LocalPlayerNum )
			continue;

		if ( i > HighestEntityIndex )
			break;

		C_BaseEntity *pBaseEntity = (C_BaseEntity*)g_pEntList->GetClientEntity(i);

		if ( !pBaseEntity )
			continue;

		if ( pBaseEntity->entindex() == 0 )
			continue;
		
		ClientClass *pClientClass = pBaseEntity->GetClientClass();

		if ( !pClientClass )
			continue;

		const char *classname = "";

		classname = pClientClass->m_pNetworkName;

		if ( !classname )
			continue;


		bool is_living = false;
		bool is_npc = false;

		if ( menu->M_ESP_Players == 1 && utils->IsPlayerIndex(pBaseEntity->entindex()) )
		{
			is_living = true;
		}else{

			if ( menu->M_ESP_NPCs && ( strstr(classname, "NPC") || strstr(classname, "NextBot") || ( g_AppID == 440 && strstr(classname, "Object") ) ) )
			{
				is_living = true;
				is_npc = true;
			}

		}

		if ( is_living )
		{
			Vector origin = NetVar__GetOrigin(pBaseEntity);

			if ( origin.x == 0 && origin.y == 0 && origin.z == 0 )
				continue;

			origin = pBaseEntity->GetAbsOrigin();

			if ( origin.x == 0 && origin.y == 0 && origin.z == 0 )
				continue;

			if ( is_living && !is_npc )
			{

				player_info_s plInfo;

				g_pEngine->GetPlayerInfo( i, &plInfo );


				if ( !plInfo.name )
				{
					DrawLivingESP( pBaseEntity, classname, is_npc);
					continue;
				}

				classname = plInfo.name;
			}

			DrawLivingESP( pBaseEntity, classname, is_npc);
			
			continue;
		}

		if ( g_AppID == 4000 )
		{
			classname = NetVar__GetClassname( pBaseEntity );

			if ( !classname )
				continue;
		}

		if ( menu->M_ESP_Weapons )
		{

			if ( g_AppID != 4000 )
			{
				if ( strstr(classname, "Weapon") )
				{
					DrawWeaponESP( pBaseEntity, classname );
					continue;
				}
			}else{
				if ( strstr(classname, "weapon") )
				{
					DrawWeaponESP( pBaseEntity, classname );
					continue;
				}
			}

			
		}

		if ( menu->T_CustomESP.length() > 0 )
		{
			DrawCustomESP( pBaseEntity, classname );
		}
	}

}

void m_d3d::DrawViewESP( )
{
	if ( !g_bIsReady )
		return;

	if ( g_viewClassName == NULL )
	{
		DrawText("worldspawn", g_ScrW - 300,2, Black, font2);
		return;
	}

	DrawText(g_viewClassName, g_ScrW - 300, 2, Black, font2);

}

void m_d3d::DoDraw( )
{
	
	if (!menu->InitializedMenu) return;

	if ( menu->S_CrossHair != 0 )
		DrawCrosshair( );

	if ( g_bIsReady )
	{


		if ( menu->M_ESP_Box == 1 || menu->M_ESP_Health == 1 || menu->M_ESP_Name == 1 || menu->M_ESP_Weapons == 1 || menu->T_CustomESP.length() > 1 )
			DrawEntESP( );

		if ( menu->M_ESP_ViewESP == 1 )
			DrawViewESP( );

		//if ( menu->M_AimEnable == 1 && menu->M_Aim_Silent == 1 )
		//	DrawAimLine( pDevice );
	}

	if ( menu->IsOpen )
	{
		menu->DrawMenu();
	}

	menu->MenuNavigation();

}