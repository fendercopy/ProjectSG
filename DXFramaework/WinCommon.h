//
// SWRender Basic FrameWork
//
// WinCommon.h : windows����
//
// ==================================================================//
// 2012.7.23
//

#pragma once

#include "windows.h"			// ������ ���
#include "stdio.h"

#pragma warning(disable:4996)


// define ============================================================= //

//
// �޸� ���� �� ��ȯ 
//
#define SAFE_RELEASE(P)			if((P)){ (P)->Release(); (P) = NULL; }
#define SAFE_DELETE(P)			if((P)){ delete (P); (P) = NULL; }
#define SAFE_DELETE_ARRAY(P)	if((P)){ delete [] (P); (P) = NULL; }


// struct =========================================================== //

//
// �ػ� ���� ����ü
//
struct SWDXDISPLAYMODE 
{ 
	DWORD Width, Height;
};

//eunm ===============================================================//
enum enINITERROR
{
	INIT_OK = 0,					// �ʱ�ȭ �� ����
	INIT_WININIT_HWND_ERROR,		// �ڵ��� ���� ��
	INIT_D3D_CREATE_ERROR,			// D3D��ü ���� ����
	INIT_D3D_DEVICE_ERROR,			// ����̽� ���� ����
	INIT_D3D_FONT_ERROR,			// ��Ʈ ���� ����
	INIT_D3D_VB_ERROR,				// ���ؽ� ���� ���� ����
	INIT_D3D_LOCK_ERROR,			// Lock����
};


// �����Լ� ===============================================================//
float GetDeltaTime(void);


// extern =========================================================== //
extern HWND					g_hWnd;
extern bool					g_bLoop;
extern char*				g_ClassName;
extern char*				g_WindowName;



