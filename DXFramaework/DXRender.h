//
// DXRender Basic FrameWork
//
// DXRender.h : ������ �κ� ��� Ŭ����
//
// ==================================================================//
// 2012.7.23
//

#pragma once

class Camera;
class Axis;
class Grid;
class DXRender
{
	// �ɼǺ��� //
	bool	m_bHelp;
	bool	m_bAxis;
	bool    m_bGrid;

	//������������������������������������������������������������������������������������������������//
	// �⺻ �Լ�
public:
	enINITERROR DataLoding(void);
	void Update(float dTime);
	void Render(void);
	void SystemUpdate(void);
	void GameLoop(void);
	void DataRelease(void);  

protected:
	void DebugHelpText(void);
	void DrawFPS(int _x, int _y);

public:
	DXRender(void);
	virtual ~DXRender(void);



	//������������������������������������������������������������������������������������������������//
	// ī�޶� ����
private:
	Camera*			m_pCamera;

	void CameraInit(void);
	void CameraUpdate( float dTime );

	//������������������������������������������������������������������������������������������������//
	// �׸��� ����
private:
	Grid*			m_pGrid;
	
	enINITERROR		InitGrid(void);
	void			RenderGrid(void);
	void			ReleaseGrid(void);

	//������������������������������������������������������������������������������������������������//
	// ��ǥ�� ����
	Axis*			m_pAxis;

	enINITERROR		InitAxis(void);
	void			RenderAxis(void);
	void			UpdateAxis(float dTime);
	void			ReleaseAxis(void);

};
