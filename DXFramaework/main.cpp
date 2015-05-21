#include "WinCommon.h"
#include "DXCommon.h"
#include "DXRender.h"
#include "DXinput.h"

//////////////////////////////////////////////////////////////////////////
// ������ ����
enINITERROR 	InitWindow(int w, int h );
enINITERROR 	InitD3D(HWND hWnd);
void 			ResizeWindow(HWND hWnd, UINT w, UINT h);
BOOL 			MessagePump(void);
void 			ReleaseWindow(void);
//////////////////////////////////////////////////////////////////////////


// ������ �޴��� Ŭ����
DXRender	g_DXRender;

// Direct Input
DXinput		g_DXInput;

//////////////////////////////////////////////////////////////////////////
//
// �����Լ�
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	enINITERROR InitResult		= INIT_OK;
	char		strResult[256]	= {0,};

	// �⺻ ������ ���� //
	if( (InitResult = InitWindow(800, 600)) != INIT_OK )
	{
		sprintf(strResult, "Init Error Code : %d", InitResult);
		MessageBox(NULL, strResult, "error", MB_OK);
		return 0;
	}

	// DX �ʱ�ȭ //
	if( (InitResult = InitD3D(g_hWnd)) != INIT_OK )
	{
		sprintf(strResult, "Init Error Code : %d", InitResult);
		MessageBox(NULL, strResult, "error", MB_OK);
		return 0;
	}

	// Direct Input //
	if( !g_DXInput.IintDirectInput(hInstance, g_hWnd) )
	{
		MessageBox(NULL, "DirectInput Fail", "error", MB_OK);
		return 0;
	}

	// ����Ÿ �ε� //
 	if( (InitResult = g_DXRender.DataLoding()) != INIT_OK )
	{
		sprintf(strResult, "Init Error Code : %d", InitResult);
		MessageBox(NULL, strResult, "error", MB_OK);
 		g_bLoop = FALSE;
	}

	while( g_bLoop )
	{
		if( MessagePump() == false )
			break;

		g_DXInput.UpdateDirectInput();
		g_DXRender.GameLoop();
	}

	g_DXInput.ReleaseDirectInput();
	g_DXRender.DataRelease();
	ReleaseWindow();

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
// �޽��� ���ν���
//
LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);

}

//////////////////////////////////////////////////////////////////////////
//
// ������ ����
//
enINITERROR InitWindow( int w, int h )
{
	WNDCLASSEX wc = { 
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
		::GetModuleHandle(NULL), NULL, NULL, 
		(HBRUSH)GetStockObject(DKGRAY_BRUSH), 
		NULL,g_ClassName, NULL 
	};

	RegisterClassEx(&wc);
	HWND hWnd = ::CreateWindow( g_ClassName, g_WindowName, 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, w, h, 
		GetDesktopWindow(), NULL,  
		wc.hInstance, NULL
		);

	g_Mode.Width	= w;
	g_Mode.Height	= h;

	if(hWnd == NULL) 
		return INIT_WININIT_HWND_ERROR;

	g_hWnd = hWnd;

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	ResizeWindow(hWnd, w, h);

	return INIT_OK;
}

//////////////////////////////////////////////////////////////////////////
//
// ������ ������ ����
//
void ResizeWindow( HWND hWnd, UINT w, UINT h )
{
	//���� �������� ��Ÿ�� ���
	RECT oldrc;
	DWORD sytle  = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE);

	//���� ������ ũ�� ���
	GetWindowRect(hWnd, &oldrc);

	//Ŭ���̾�Ʈ ����
	RECT newrc;
	newrc.left = 0;  
	newrc.top = 0;
	newrc.right = w;
	newrc.bottom = h;

	// Ŭ���̾�Ʈ ���� �缳��
	AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);


	//������ �������� �ʺ�� ���� ���Ѵ�. 
	int width = (newrc.right - newrc.left);
	int height = (newrc.bottom - newrc.top);


	//���ο� ũ�⸦ �����쿡 �����Ѵ�.
	SetWindowPos(hWnd, HWND_NOTOPMOST, 
		oldrc.left, oldrc.top, 
		width, height, SWP_SHOWWINDOW);
}


//////////////////////////////////////////////////////////////////////////
//
// Direct �ʱ�ȭ
//
enINITERROR InitD3D(HWND hWnd)
{
	// D3D��ü ���� //
	if( (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL )
		return INIT_D3D_CREATE_ERROR;

	// ���÷��� ���� //
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp) );

	d3dpp.BackBufferWidth		= g_Mode.Width;						// ����� ����
	d3dpp.BackBufferHeight		= g_Mode.Height;					// ����� ����
	d3dpp.BackBufferCount		= 1;								// ����� ����(����1��)
	d3dpp.Windowed				= true;								// true : â���,   
	d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;			// ��������
	d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;	// ��������ȭ ����
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;



	// ����̽� ���� //
	if( FAILED(
					g_pD3D->CreateDevice(
											D3DADAPTER_DEFAULT,						// ������(0)�� ���� ������(�����)
											D3DDEVTYPE_HAL,							// �ϵ���� ������
											hWnd,									// ������ �ڵ�
											D3DCREATE_HARDWARE_VERTEXPROCESSING,	// ���� ó����� (GPU)
											&d3dpp,									// ȭ�� ���� �ɼ�
											&g_pDevice								// ������ ����̽� ���� ����
									    )
		      )
	  )
	{
		return INIT_D3D_DEVICE_ERROR;
	}


	// ��Ʈ ���� //
	D3DXCreateFont(
						g_pDevice,						// ����̽�
						12,								// ����
						6,								// ����
						1,								// �β�
						0,								// Mip Level
						false,							// bUseItalic
						DEFAULT_CHARSET,				//
						OUT_DEFAULT_PRECIS,				// 9.0b 
						DEFAULT_QUALITY,				//
						DEFAULT_PITCH | FF_DONTCARE,	//
						(LPSTR)"����",					// �۾�ü
						&g_pFont						// ������ ��Ʈ�� ���� ����
				  );

	if( g_pFont == NULL )
		return INIT_D3D_FONT_ERROR;

	return INIT_OK;
}

//////////////////////////////////////////////////////////////////////////
//
// �޽��� ó��
//
BOOL MessagePump( void )
{
	MSG msg;	
	ZeroMemory(&msg, sizeof(msg));

	while(1)
	{
		//�޼��� ť�� �޼����� ������...ó���ϰ�..
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)			//������ ���Ḧ ���� ����.
				return FALSE;		

			//������ �޼��� ó��.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else  //Ư���� �޼����� ���ٸ� ����(������)�� �����Ѵ�..
		{
			return TRUE;		
		}
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//
// ��ü ���� �� ��ȯ
//
void ReleaseWindow( void )
{
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pDevice);
	SAFE_RELEASE(g_pD3D);
}