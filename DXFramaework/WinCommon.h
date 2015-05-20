//
// SWRender Basic FrameWork
//
// WinCommon.h : windows관련
//
// ==================================================================//
// 2012.7.23
//

#pragma once

#include "windows.h"			// 윈도우 헤더
#include "stdio.h"

#pragma warning(disable:4996)


// define ============================================================= //

//
// 메모리 해제 및 반환 
//
#define SAFE_RELEASE(P)			if((P)){ (P)->Release(); (P) = NULL; }
#define SAFE_DELETE(P)			if((P)){ delete (P); (P) = NULL; }
#define SAFE_DELETE_ARRAY(P)	if((P)){ delete [] (P); (P) = NULL; }


// struct =========================================================== //

//
// 해상도 정보 구조체
//
struct SWDXDISPLAYMODE 
{ 
	DWORD Width, Height;
};

//eunm ===============================================================//
enum enINITERROR
{
	INIT_OK = 0,					// 초기화 시 성공
	INIT_WININIT_HWND_ERROR,		// 핸들이 없을 시
	INIT_D3D_CREATE_ERROR,			// D3D객체 생성 실패
	INIT_D3D_DEVICE_ERROR,			// 디바이스 생성 실패
	INIT_D3D_FONT_ERROR,			// 폰트 생성 실패
	INIT_D3D_VB_ERROR,				// 버텍스 버퍼 생성 실패
	INIT_D3D_LOCK_ERROR,			// Lock실패
};


// 전역함수 ===============================================================//
float GetDeltaTime(void);


// extern =========================================================== //
extern HWND					g_hWnd;
extern bool					g_bLoop;
extern char*				g_ClassName;
extern char*				g_WindowName;



