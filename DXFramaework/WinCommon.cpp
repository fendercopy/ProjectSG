#include "WinCommon.h"

// 초당 프레임 체크 함수
float GetDeltaTime( void )
{
	//프레임 동기화
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();
	float dTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;

	return dTime;
}



HWND				g_hWnd		= NULL;
bool				g_bLoop		= TRUE;
char*				g_ClassName	= "도움말 켜기/끄기(F1키)";
char*				g_WindowName= "도움말 켜기/끄기(F1키)";
SWDXDISPLAYMODE		g_Mode		= {1024, 768};
HFONT				g_hFont		= NULL;
COLORREF			g_FontColor = RGB(0, 255, 0);	 