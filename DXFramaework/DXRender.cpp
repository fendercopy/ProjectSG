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
// ����Ÿ �ε�
//
enINITERROR DXRender::DataLoding( void )
{
	enINITERROR result;
	
	CameraInit();							// ī�޶�

	if( (result = InitGrid()) != INIT_OK )	// �׸���
		return result;

	if( (result = InitAxis()) != INIT_OK )  // ��ǥ��
		return result;


	return result;
}


//////////////////////////////////////////////////////////////////////////
//
// ������Ʈ 
//
void DXRender::Update(float dTime)
{
	SystemUpdate();

	CameraUpdate(dTime);			// ī�޶�
	UpdateAxis(dTime);				// ��ǥ��
}


//////////////////////////////////////////////////////////////////////////
//
// ������
//
void DXRender::Render(void)
{
	// ������Ʈ �������� �Ʒ�����..
	// ���� �������� ���⼭����



	// ������Ʈ ������ ������ 
	// �׸���, ��ǥ��, ���� �޽����� ������
	RenderGrid();	// �׸���
	RenderAxis();	// ��ǥ��

	if( m_bHelp )
	{
		DebugHelpText();
		DrawFPS(5,5);
	}
}



//////////////////////////////////////////////////////////////////////////
//
// �ý��� ������Ʈ 
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
// ������
//
void DXRender::GameLoop( void )
{
	// ������Ʈ ===============================================================//
	float dTime = GetDeltaTime();
	Update(dTime);

	
	// ������ ================================================================//
	// �׸��� �غ�
	g_pDevice->BeginScene();
	{
		// ����Ÿ��(����ȭ��) �����(ä���)
		g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0,0,1,1), 1.0f, 0);

		Render();		
	}
	// �׸��� ����
	g_pDevice->EndScene();

	// Flip(����)
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}


//////////////////////////////////////////////////////////////////////////
//
// ȭ�� �ؽ�Ʈ ���
//
void DXRender::DebugHelpText( void )
{
	int x = 5, y = 30;

	DrawText(x, y, D3DXCOLOR(1,1,1,1), "[ �ɼ����� ]");
	DrawText(x, y+=14, D3DXCOLOR(1,1,1,1), "- ��ǥ��(F2) : %s", m_bAxis ? "ON" : "OFF");
	DrawText(x, y+=14, D3DXCOLOR(1,1,1,1), "- �׸���(F3) : %s", m_bGrid ? "ON" : "OFF");

	y+=14;
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "[ ī�޶� ���� ]");
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "- �ڵ�ȸ��(INSERTŰ) : %s", 
								m_pCamera->m_bCameraAutoRotate ? "ON" : "OFF");
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "- ��ġ(%.2f, %.2f, %.2f)",
								m_pCamera->m_vEye.x, m_pCamera->m_vEye.y, m_pCamera->m_vEye.z) ;
	DrawText(x, y+=14, D3DXCOLOR(1,1,0,1), "- �ٶ󺸴°�(%.2f, %.2f, %.2f)",
								m_pCamera->m_vLookAt.x, m_pCamera->m_vLookAt.y, m_pCamera->m_vLookAt.z) ;
}

/////////////////////////////////////////////////////////////////////////////
//
// �ʴ� �����Ӽ� ���.
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
// �޸� ���� �� ��ȯ
//
void DXRender::DataRelease( void )
{
	ReleaseGrid();
	ReleaseAxis();
}


//////////////////////////////////////////////////////////////////////////
//
// �׸��� �ʱ�ȭ
//
enINITERROR DXRender::InitGrid( void )
{
	m_pGrid = new Grid;

	// �׸��� �ʱ�ȭ
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
// �׸��� ������
//
void DXRender::RenderGrid( void )
{
	if( m_bGrid )
		m_pGrid->Render_Grid();
}


//////////////////////////////////////////////////////////////////////////
//
// �׸��� ��ü ��ȯ / ����
//
void DXRender::ReleaseGrid( void )
{
	SAFE_DELETE(m_pGrid);
}


//������������������������������������������������������������������������������������������������//
// �׸��� ����
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


//������������������������������������������������������������������������������������������������//
// ī�޶� ����

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
