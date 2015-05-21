#include "WinCommon.h"
#include "DXCommon.h"
#include "Grid.h"
#include "Camera.h"

#include "Axis.h"
#include "DXRender.h"

DXRender::DXRender(void)
{
	m_pGrid		= NULL;
	m_bAxis		= true;
	m_bGrid		= true;
	m_bHelp		= true;
}

DXRender::~DXRender(void)
{
}


//////////////////////////////////////////////////////////////////////////
//
// 데이타 로딩
//
enINITERROR DXRender::DataLoding( void )
{
	enINITERROR result;
	
	CameraInit();							// 카메라

	if( (result = InitGrid()) != INIT_OK )	// 그리드
		return result;

	if( (result = InitAxis()) != INIT_OK )  // 좌표축
		return result;


	return result;
}


//////////////////////////////////////////////////////////////////////////
//
// 업데이트 
//
void DXRender::Update(float dTime)
{
	SystemUpdate();

	CameraUpdate(dTime);			// 카메라
	UpdateAxis(dTime);				// 좌표축
}


//////////////////////////////////////////////////////////////////////////
//
// 렌더링
//
void DXRender::Render(void)
{
	// 오브젝트 렌더링은 아래부터..
	// 실제 랜더링은 여기서부터



	// 오브젝트 렌더링 다음에 
	// 그리드, 좌표축, 헬프 메시지를 보여줌
	RenderGrid();	// 그리드
	RenderAxis();	// 좌표축

	if( m_bHelp )
	{
		DebugHelpText();
		DrawFPS(5,5);
	}
}



//////////////////////////////////////////////////////////////////////////
//
// 시스템 업데이트 
//
void DXRender::SystemUpdate( void )
{
	if( KeyUp(DIK_F1) )
		m_bHelp ^= true;

	if( KeyUp(DIK_F2) )
		m_bAxis ^= true;

	if( KeyUp(DIK_F3) )
		m_bGrid ^= true;
}



//////////////////////////////////////////////////////////////////////////
//
// 렌더링
//
void DXRender::GameLoop( void )
{
	// 업데이트 ===============================================================//
	float dTime = GetDeltaTime();
	Update(dTime);

	
	// 렌더링 ================================================================//
	// 그리기 준비
	g_pDevice->BeginScene();
	{
		// 렌더타켓(바탕화면) 지우기(채우기)
		g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0,0,1,1), 1.0f, 0);

		Render();		
	}
	// 그리기 종료
	g_pDevice->EndScene();

	// Flip(스왑)
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}


//////////////////////////////////////////////////////////////////////////
//
// 화면 텍스트 출력
//
void DXRender::DebugHelpText( void )
{
	int x = 5, y = 30;

	DrawText(x, y, D3DXCOLOR(1,1,1,1), "[ 옵션정보 ]");
	DrawText(x, y+=14, D3DXCOLOR(1,1,1,1), "- 좌표축(F2) : %s", m_bAxis ? "ON" : "OFF");
	DrawText(x, y+=14, D3DXCOLOR(1,1,1,1), "- 그리드(F3) : %s", m_bGrid ? "ON" : "OFF");

	y+=14;
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "[ 카메라 정보 ]");
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "- 자동회전(INSERT키) : %s", 
								m_pCamera->m_bCameraAutoRotate ? "ON" : "OFF");
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "- 위치(%.2f, %.2f, %.2f)",
								m_pCamera->m_vEye.x, m_pCamera->m_vEye.y, m_pCamera->m_vEye.z) ;
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "- 바라보는곳(%.2f, %.2f, %.2f)",
								m_pCamera->m_vLookAt.x, m_pCamera->m_vLookAt.y, m_pCamera->m_vLookAt.z) ;
}

/////////////////////////////////////////////////////////////////////////////
//
// 초당 프레임수 출력.
//
void DXRender::DrawFPS( int _x, int _y )
{
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();

	static int frmcnt = 0;
	static float fps = 0.0f;

	++frmcnt;


	int time = nowtime - oldtime;
	if( time >= 1000 )
	{
		oldtime = nowtime;

		fps = (float)frmcnt;
		frmcnt = 0;
	}


	char msg[40];
	sprintf(msg,"FPS:%.1f/%d", fps, frmcnt );

	RECT rc = { _x, _y, 0, 0};

	g_pFont->DrawText(NULL, msg, strlen(msg), &rc, DT_NOCLIP, D3DXCOLOR(0, 1, 0, 1));
}


//////////////////////////////////////////////////////////////////////////
//
// 메모리 해제 및 반환
//
void DXRender::DataRelease( void )
{
	ReleaseGrid();
	ReleaseAxis();
}


//////////////////////////////////////////////////////////////////////////
//
// 그리드 초기화
//
enINITERROR DXRender::InitGrid( void )
{
	m_pGrid = new Grid;

	// 그리드 초기화
	GRIDINFO info;
	ZeroMemory(&info, sizeof(GRIDINFO));

	info._color = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	info._w		= 100;
	info._h		= 100;
	info._scale	= 10;

	return m_pGrid->Init_Grid(info);
}



//////////////////////////////////////////////////////////////////////////
//
// 그리드 렌더링
//
void DXRender::RenderGrid( void )
{
	if( m_bGrid )
		m_pGrid->Render_Grid();
}


//////////////////////////////////////////////////////////////////////////
//
// 그리드 객체 반환 / 해제
//
void DXRender::ReleaseGrid( void )
{
	SAFE_DELETE(m_pGrid);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
// 그리드 관련
enINITERROR DXRender::InitAxis( void )
{
	m_pAxis = new Axis;

	_AXISINFO ai;
	ZeroMemory(&ai, sizeof(ai));

	ai.length	= 5.0f; 
	ai.width	= g_Mode.Width;
	ai.height	= g_Mode.Height;

	m_pAxis->Init(ai);		

	return INIT_OK;
}

void DXRender::UpdateAxis( float dTime )
{
	m_pAxis->Update( ); 
}

void DXRender::RenderAxis( void )
{
	if( m_bAxis )
		m_pAxis->Render( g_Mode.Width, g_Mode.Height); 
}

void DXRender::ReleaseAxis( void )
{
	SAFE_DELETE(m_pAxis);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
// 카메라 관련

void DXRender::CameraInit( void )
{
	m_pCamera = new Camera;

	if( !m_pCamera ) return;

	m_pCamera->ViewTransFrom();
	m_pCamera->ProjectionTransFrom();
}

void DXRender::CameraUpdate( float dTime )
{
	if( !m_pCamera ) return;

	m_pCamera->UpdateCamera(dTime);
}
