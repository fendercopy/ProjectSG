#include "WinCommon.h"
#include "DXCommon.h"
#include "DXRender.h"
#include "DXinput.h"

//////////////////////////////////////////////////////////////////////////
// 윈도우 관련
enINITERROR 	InitWindow(int w, int h );
enINITERROR 	InitD3D(HWND hWnd);
void 			ResizeWindow(HWND hWnd, UINT w, UINT h);
BOOL 			MessagePump(void);
void 			ReleaseWindow(void);
//////////////////////////////////////////////////////////////////////////


// 렌더링 메니저 클래스
DXRender	g_DXRender;

// Direct Input
DXinput		g_DXInput;

//////////////////////////////////////////////////////////////////////////
//
// 메인함수
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	enINITERROR InitResult		= INIT_OK;
	char		strResult[256]	= {0,};

	// 기본 윈도우 생성 //
	if( (InitResult = InitWindow(800, 600)) != INIT_OK )
	{
		sprintf(strResult, "Init Error Code : %d", InitResult);
		MessageBox(NULL, strResult, "error", MB_OK);
		return 0;
	}

	// DX 초기화 //
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

	// 데이타 로딩 //
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
// 메시지 프로시저
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
// 윈도우 생성
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
// 윈도우 사이즈 변경
//
void ResizeWindow( HWND hWnd, UINT w, UINT h )
{
	//현재 윈도우의 스타일 얻기
	RECT oldrc;
	DWORD sytle  = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE);

	//현재 윈도우 크기 얻기
	GetWindowRect(hWnd, &oldrc);

	//클라이언트 영역
	RECT newrc;
	newrc.left = 0;  
	newrc.top = 0;
	newrc.right = w;
	newrc.bottom = h;

	// 클라이언트 영역 재설정
	AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);


	//보정된 윈도우의 너비와 폭을 구한다. 
	int width = (newrc.right - newrc.left);
	int height = (newrc.bottom - newrc.top);


	//새로운 크기를 윈도우에 설정한다.
	SetWindowPos(hWnd, HWND_NOTOPMOST, 
		oldrc.left, oldrc.top, 
		width, height, SWP_SHOWWINDOW);
}


//////////////////////////////////////////////////////////////////////////
//
// Direct 초기화
//
enINITERROR InitD3D(HWND hWnd)
{
	// D3D객체 생성 //
	if( (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL )
		return INIT_D3D_CREATE_ERROR;

	// 디스플레이 정보 //
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp) );

	d3dpp.BackBufferWidth		= g_Mode.Width;						// 백버퍼 넓이
	d3dpp.BackBufferHeight		= g_Mode.Height;					// 백버퍼 높이
	d3dpp.BackBufferCount		= 1;								// 백버퍼 개수(현재1개)
	d3dpp.Windowed				= true;								// true : 창모드,   
	d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;			// 스왑종류
	d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;	// 수직동기화 무시
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;



	// 디바이스 생성 //
	if( FAILED(
					g_pD3D->CreateDevice(
											D3DADAPTER_DEFAULT,						// 디폴드(0)번 비디오 어탭터(모니터)
											D3DDEVTYPE_HAL,							// 하드웨어 레스터
											hWnd,									// 윈도우 핸들
											D3DCREATE_HARDWARE_VERTEXPROCESSING,	// 정점 처리방법 (GPU)
											&d3dpp,									// 화면 설정 옵션
											&g_pDevice								// 생성된 디바이스 받을 변수
									    )
		      )
	  )
	{
		return INIT_D3D_DEVICE_ERROR;
	}


	// 폰트 생성 //
	D3DXCreateFont(
						g_pDevice,						// 디바이스
						12,								// 세로
						6,								// 가로
						1,								// 두께
						0,								// Mip Level
						false,							// bUseItalic
						DEFAULT_CHARSET,				//
						OUT_DEFAULT_PRECIS,				// 9.0b 
						DEFAULT_QUALITY,				//
						DEFAULT_PITCH | FF_DONTCARE,	//
						(LPSTR)"굴림",					// 글씨체
						&g_pFont						// 생성된 폰트를 받을 변수
				  );

	if( g_pFont == NULL )
		return INIT_D3D_FONT_ERROR;

	return INIT_OK;
}

//////////////////////////////////////////////////////////////////////////
//
// 메시지 처리
//
BOOL MessagePump( void )
{
	MSG msg;	
	ZeroMemory(&msg, sizeof(msg));

	while(1)
	{
		//메세지 큐에 메세지가 있으면...처리하고..
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)			//윈도우 종료를 위해 리턴.
				return FALSE;		

			//나머지 메세지 처리.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else  //특별한 메세지가 없다면 게임(렌더링)을 진행한다..
		{
			return TRUE;		
		}
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//
// 객체 해제 및 반환
//
void ReleaseWindow( void )
{
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pDevice);
	SAFE_RELEASE(g_pD3D);
}