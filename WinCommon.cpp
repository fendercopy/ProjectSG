#include "WinCommon.h"

// �ʴ� ������ üũ �Լ�
float GetDeltaTime( void )
{
	//������ ����ȭ
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();
	float dTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;

	return dTime;
}



HWND				g_hWnd		= NULL;
bool				g_bLoop		= TRUE;
char*				g_ClassName	= "���� �ѱ�/����(F1Ű)";
char*				g_WindowName= "���� �ѱ�/����(F1Ű)";
SWDXDISPLAYMODE		g_Mode		= {1024, 768};
HFONT				g_hFont		= NULL;
COLORREF			g_FontColor = RGB(0, 255, 0);	 