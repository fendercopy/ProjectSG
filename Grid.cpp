#include "WinCommon.h"
#include "DXCommon.h"
#include "Grid.h"

Grid::Grid(void)
{
	m_iLineCount	= 0;
	m_pVB			= NULL;
	m_pVertex		= NULL;
}

Grid::~Grid(void)
{
	SAFE_RELEASE(m_pVB);
	SAFE_DELETE_ARRAY(m_pVertex);
}


//////////////////////////////////////////////////////////////////////////
//
// 그리드 초기화
//
enINITERROR Grid::Init_Grid( GRIDINFO pGrid )
{
	m_Info		= pGrid;

	int iCountX   = (m_Info._w / (int)m_Info._scale) + 1;
	int iCountY   = (m_Info._h / (int)m_Info._scale) + 1;
	int iVtxCount = (iCountX + iCountY) * 2;

	m_pVertex = new D3DFVF_XYZ_COLOR[iVtxCount];
	ZeroMemory(m_pVertex, sizeof(D3DFVF_XYZ_COLOR)*iVtxCount);

	// 그리드 시작위치 
	float fX = m_Info._w * 0.5f;
	float fY = m_Info._h * 0.5f;

	// x축
	int k = 0;
	for( int i = 0; i < iCountX; i++, k+=2 )
	{
		float _fX 				= -fX;
		float _fZ 				=  fY - i * m_Info._scale;

		// 정점
		m_pVertex[k].vPos		= D3DXVECTOR3(_fX, 0.0f, _fZ);
		m_pVertex[k+1].vPos		= D3DXVECTOR3(_fX + m_Info._w, 0.0f, _fZ);
		
		// 색상
		m_pVertex[k].color		= (DWORD)(iCountX/2 == i) ? D3DXCOLOR(0, 0, 0, 1) : m_Info._color;
		m_pVertex[k+1].color	= (DWORD)(iCountX/2 == i) ? D3DXCOLOR(0, 0, 0, 1) : m_Info._color;
	}

	// z축
	for(int i=0; i < iCountX; i++, k+=2)
	{
		float _fX = -fX + i * m_Info._scale;		// 오른쪽으로
		float _fZ = fY;

		// 정점
		m_pVertex[k].vPos		= D3DXVECTOR3(_fX, 0.0f, _fZ);
		m_pVertex[k+1].vPos		= D3DXVECTOR3(_fX, 0.0f, _fZ - m_Info._h);

		// 색상
		m_pVertex[k].color		= (DWORD)(iCountX/2 == i) ? D3DXCOLOR(0, 0, 0, 1) : m_Info._color;
		m_pVertex[k+1].color	= (DWORD)(iCountX/2 == i) ? D3DXCOLOR(0, 0, 0, 1) : m_Info._color;
	}

	// 정점 버퍼 구성 //
	HRESULT res = S_OK;
	int iSize = iVtxCount * sizeof(D3DFVF_XYZ_COLOR);

	if( FAILED( res = g_pDevice->CreateVertexBuffer(iSize, 0, FVF_XYZ_COLOR, D3DPOOL_MANAGED, &m_pVB, NULL)) )
		return INIT_D3D_VB_ERROR;

	VOID* pBuff = NULL;
	if( FAILED( res = m_pVB->Lock(0, iSize, (VOID**)&pBuff, NULL)) )
		return INIT_D3D_LOCK_ERROR;

	memcpy(pBuff, m_pVertex, iSize);
	m_pVB->Unlock();

	m_iLineCount = iVtxCount / 2;

	D3DXMatrixIdentity(&m_mTM);

	return INIT_OK;
}


//////////////////////////////////////////////////////////////////////////
//
// 그리드 렌더링
//
void Grid::Render_Grid( void )
{
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pDevice->SetTransform(D3DTS_WORLD, &m_mTM);
	g_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(D3DFVF_XYZ_COLOR));
	g_pDevice->SetFVF(FVF_XYZ_COLOR);
	g_pDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_iLineCount);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
}