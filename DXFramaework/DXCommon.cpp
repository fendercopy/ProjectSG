#include "WinCommon.h"
#include "DXCommon.h"


LPDIRECT3D9						g_pD3D		= NULL;
LPDIRECT3DDEVICE9				g_pDevice	= NULL;
LPD3DXFONT						g_pFont		= NULL;
D3DDISPLAYMODE					g_Mode		= { 0, 0, 0, D3DFMT_A8R8G8B8 };


// 화면 2D 좌표 텍스처 출력 함수 //
void DrawText( int _x, int _y, D3DXCOLOR color, char* msg, ...)
{
	char buff[256];
	va_list vl;
	va_start(vl, msg); 
	vsprintf(buff, msg, vl);
	va_end(vl);

	RECT rc = { _x+1, _y+1, 0, 0};
	g_pFont->DrawText(NULL, buff, strlen(buff), &rc, DT_NOCLIP, D3DXCOLOR(0,0,0,1));

	rc.left = _x-1;
	rc.top =  _y-1;
	g_pFont->DrawText(NULL, buff, strlen(buff), &rc, DT_NOCLIP, color);
}