//
// SWRender Basic FrameWork
//
// Grid.h : 월드상의 위치를 확인하기 위한 그리드
//
// ==================================================================//
// 2012.7.23
//
#pragma once

// 그리드 정보 //
struct GRIDINFO
{
	int			_w;
	int			_h;
	float		_scale;
	D3DXCOLOR	_color;
};

class Grid
{
private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	D3DXMATRIX						m_mTM;

	D3DFVF_XYZ_COLOR*				m_pVertex;
	GRIDINFO						m_Info;

	int								m_iLineCount;

public:
	enINITERROR		Init_Grid(GRIDINFO pGrid);
	void			Render_Grid(void);

public:
	Grid(void);
	virtual ~Grid(void);
};
