#pragma once

#include "includes.h"

class m_d3d
{
public:

	void DrawText( string pString, int x, int y, D3DCOLOR col, ID3DXFont *font);

	void DrawFilledRectangle( int X, int Y, int Width, int Height, DWORD dwColor );

	void DrawBorder( int x, int y, int w, int h, DWORD color1, DWORD color2 );

	void DrawNonFilledRectangle( int x, int y, int w, int h, DWORD dwColor );

	void DrawBorderWithFill( int x, int y, int w, int h, DWORD color1, DWORD color2, DWORD Fill );

	void DrawLine( int x, int y, int x2, int y2, int width, DWORD color);

	void DrawPoint(int x, int y, int w, int h, DWORD color );

	void DrawCircle(int X, int Y, int radius, int numSides, DWORD Color);

	void DrawBorder( int x, int y, int w, int h, int px, D3DCOLOR BorderColor );

	void DrawGradientRect( int X, int Y, int Width, int Height, DWORD startCol, DWORD endCol );

	void DrawColorFill( int x, int y, int w, int h, D3DCOLOR color );

	void DrawColorFillOutline( int x, int y, int w, int h, int lw, D3DCOLOR color);

	void DrawBox( int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BorderColor );

	void Render( bool bReset );

	void DoDraw( );

	void Initiate( HWND hWnd, int ScrW, int ScrH );

	void DrawViewESP( );

	void DrawEntESP( );

	void DrawLivingESP( C_BaseEntity *pBaseEntity, const char *name, bool is_npc );

	void DrawWeaponESP( C_BaseEntity *pBaseEntity, const char *name );

	void DrawCustomESP( C_BaseEntity *pBaseEntity, const char *name );

	void DrawCrosshair( );

	LPDIRECT3D9EX instance;

	bool bNeedsFlush;
	bool bFlushDraw;

	LPD3DXFONT font1;
	LPD3DXFONT font2;
	LPD3DXFONT font3;
	LPD3DXSPRITE sprite;
	ID3DXLine *line;

	IDirect3DDevice9* device;

};

extern m_d3d *d3d;

//Draw Structs
struct DXUT_SCREEN_VERTEX 
{ 
	float x, y, z, h;
	D3DCOLOR color;
	float tu, tv;
	static DWORD FVF; 
};

class CVertexList
{
public:
    FLOAT X, Y, Z;
    DWORD dColor;
};

struct D3DTLVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};



//Colors
const D3DCOLOR Red			= D3DCOLOR_ARGB (255,	255,	0,		0	);
const D3DCOLOR Yellow		= D3DCOLOR_ARGB (255,	255,	255,	0	);
const D3DCOLOR Green		= D3DCOLOR_ARGB (255,	0,		255,	0	);
const D3DCOLOR Blue			= D3DCOLOR_ARGB (255,	0,		0,		255 );
const D3DCOLOR Purple		= D3DCOLOR_ARGB (255,	102,	0,		153 );
const D3DCOLOR Orange		= D3DCOLOR_ARGB (255,	255,	165,	0	);
const D3DCOLOR Pink			= D3DCOLOR_ARGB (255,	255,	20,		147 );
const D3DCOLOR YellowGreen	= D3DCOLOR_ARGB (255,	154,	205,	50	);
const D3DCOLOR LightGrey	= D3DCOLOR_ARGB (255,	211,	211,	211	);
const D3DCOLOR LightCyan	= D3DCOLOR_ARGB (255,	122,	139,	139	);
const D3DCOLOR Snow			= D3DCOLOR_ARGB (255,	139,	137,	137	);
const D3DCOLOR TransBlue	= D3DCOLOR_ARGB (150,	0,		0,		255 );
const D3DCOLOR TransBlack	= D3DCOLOR_ARGB (150,	0,		0,		0	);
const D3DCOLOR TransBlack2	= D3DCOLOR_ARGB (80,	0,		0,		0	);
const D3DCOLOR Aqua			= D3DCOLOR_ARGB (255,	54,		231,	255	);
const D3DCOLOR White		= D3DCOLOR_ARGB (255,	255,	255,	255 );
const D3DCOLOR Black		= D3DCOLOR_ARGB (255,	0,		0,		0	);
const D3DCOLOR DGrey		= D3DCOLOR_ARGB (255,	59,		55,		57	);
const D3DCOLOR Grey			= D3DCOLOR_ARGB (255,	84,		79,		81	);
const D3DCOLOR Outline		= D3DCOLOR_ARGB (255,	140,	140,	140	);