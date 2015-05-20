#include "WinCommon.h"
#include "DXCommon.h"
#include "Axis.h"

Axis::Axis(void)
{
	m_pVB			= NULL;
	m_pVtxs			= NULL; 
}

Axis::~Axis(void)
{
	Release();
}

void Axis::Init( _AXISINFO info)
{
	D3DXMatrixIdentity(&m_mTM); 

	m_AxisInfo	= info;

	//라인 3개 + 글자 출력 좌표 3개.
	m_pVtxs = new D3DFVF_XYZ_COLOR[6+3];

	// x축
	m_pVtxs[0].vPos = D3DXVECTOR3(0, 0, 0);		
	m_pVtxs[1].vPos = D3DXVECTOR3(m_AxisInfo.length, 0, 0);
	m_pVtxs[0].color = D3DXCOLOR(1, 0, 0, 1);
	m_pVtxs[1].color = D3DXCOLOR(1, 0, 0, 1);

	// y축
	m_pVtxs[2].vPos = D3DXVECTOR3(0, 0, 0);		
	m_pVtxs[3].vPos = D3DXVECTOR3(0, m_AxisInfo.length, 0);
	m_pVtxs[2].color = D3DXCOLOR(0, 1, 0, 1);
	m_pVtxs[3].color = D3DXCOLOR(0, 1, 0, 1);

	// z축
	m_pVtxs[4].vPos = D3DXVECTOR3(0, 0, 0);		
	m_pVtxs[5].vPos = D3DXVECTOR3(0, 0, m_AxisInfo.length);
	m_pVtxs[4].color = D3DXCOLOR(0, 0.3f, 1, 1);
	m_pVtxs[5].color = D3DXCOLOR(0, 0.3f, 1, 1);


	// 축표시 문자
	m_pVtxs[6].vPos = D3DXVECTOR3(m_AxisInfo.length+0.1f, 0, 0);
	m_pVtxs[7].vPos = D3DXVECTOR3(0, m_AxisInfo.length+0.1f, 0);
	m_pVtxs[8].vPos = D3DXVECTOR3(0, 0, m_AxisInfo.length+0.1f);
	m_pVtxs[6].color = D3DXCOLOR(1, 0, 0, 1);
	m_pVtxs[7].color = D3DXCOLOR(0, 1, 0, 1);
	m_pVtxs[8].color = D3DXCOLOR(0, 0.3f, 1, 1);
}


void Axis::Update(void) 
{
	// 뷰 행렬 얻기
	D3DXMATRIX mView;			
	g_pDevice->GetTransform(D3DTS_VIEW, &mView);	 

	// 프로젝션 행렬 얻기
	D3DXMATRIX mProj;	
	g_pDevice->GetTransform(D3DTS_PROJECTION, &mProj);


	D3DXMATRIX mVP; 							 
	D3DXMatrixIdentity(&mVP);

	// 3D좌표계 -> 2D좌표계 변환 행렬
	mVP._11 = m_AxisInfo.width * 0.5f;				
	mVP._22 = -(m_AxisInfo.height * 0.5f);			
	mVP._41 = m_AxisInfo.width * 0.5f;				
	mVP._42 = m_AxisInfo.height - m_AxisInfo.height * 0.5f;	

	// 최종 행렬
	D3DXMATRIX mTM;			
	D3DXMatrixIdentity(&mTM);

	//글자 출력위치 최종 계산.
	mTM = mView * mProj * mVP; 

	for(int i=6, k=0; i<9; i++ , k++)
		D3DXVec3TransformCoord(&m_vTextPos[k], &m_pVtxs[i].vPos, &mTM);
}

void Axis::Render(DWORD width, DWORD height)
{
	g_pDevice->SetTransform(D3DTS_WORLD, &m_mTM);

	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, false); 	
	g_pDevice->SetFVF(FVF_XYZ_COLOR);	

	g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, m_pVtxs, sizeof(D3DFVF_XYZ_COLOR)); 

	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, true);

	// 문자 출력
	DrawText((int)m_vTextPos[0].x,		(int)m_vTextPos[0].y-5, m_pVtxs[6].color,  "X");
	DrawText((int)m_vTextPos[1].x-4,	(int)m_vTextPos[1].y-5, m_pVtxs[7].color,  "Y" );
	DrawText((int)m_vTextPos[2].x,		(int)m_vTextPos[2].y-5, m_pVtxs[8].color,  "Z" );
}

void Axis::Release()
{
	SAFE_RELEASE(m_pVB);
	SAFE_DELETE(m_pVtxs);	
}
