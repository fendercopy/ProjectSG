//
// SWRender Basic FrameWork
//
// DXCommon.h : DX관련
//
// ==================================================================//
// 2012.7.23
//

#pragma once

// Direct 헤더 및 lib 설정 ======================================================//
#include "d3d9.h"
#include "d3dx9.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "DXinput.h"

// 전역함수 =====================================================================//
//
// 텍스트 화면출력
void DrawText( int _x, int _y, D3DXCOLOR color, char* msg, ...);


// 정점규칙 =====================================================================//
//
// 위치 + 컬러
struct D3DFVF_XYZ_COLOR
{
	D3DXVECTOR3 vPos;	// 위치
	DWORD		color;	// 컬러
};
#define FVF_XYZ_COLOR ( D3DFVF_XYZ | D3DFVF_DIFFUSE )


// extern =======================================================================//
extern LPDIRECT3D9						g_pD3D;
extern LPDIRECT3DDEVICE9				g_pDevice;
extern LPD3DXFONT						g_pFont;		
extern D3DDISPLAYMODE					g_Mode;		
