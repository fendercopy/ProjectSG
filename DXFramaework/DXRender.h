//
// DXRender Basic FrameWork
//
// DXRender.h : 렌더링 부분 담당 클래스
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
	// 옵션변수 //
	bool	m_bHelp;
	bool	m_bAxis;
	bool    m_bGrid;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 기본 함수
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



	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 카메라 관련
private:
	Camera*			m_pCamera;

	void CameraInit(void);
	void CameraUpdate( float dTime );

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 그리드 관련
private:
	Grid*			m_pGrid;
	
	enINITERROR		InitGrid(void);
	void			RenderGrid(void);
	void			ReleaseGrid(void);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 좌표축 관련
	Axis*			m_pAxis;

	enINITERROR		InitAxis(void);
	void			RenderAxis(void);
	void			UpdateAxis(float dTime);
	void			ReleaseAxis(void);

};
