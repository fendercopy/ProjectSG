#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARDMAX			256

#include "dinput.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib") 

enum MOUSEBUTTON_TYPE { MT_LEFT, MT_RIGHT, MT_MIDDLE, MT_MAX };


class DXinput
{
	HINSTANCE				m_hInstance;
	HWND					m_hWnd;

	LPDIRECTINPUT8			m_pDI;
	LPDIRECTINPUTDEVICE8	m_pKeyBoardDevice;				// 키보드 디바이스
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;					// 마우스 디바이스

	DIMOUSESTATE			m_MouseState;					// 마우스 상태
	POINT					m_MousePoint;
	BYTE					m_MouseKeyState[MT_MAX];
	BYTE					m_MouseOldKeyState[MT_MAX];
	BYTE					m_MouseNewKeyState[MT_MAX];

	BYTE					m_KeyBoardState[KEYBOARDMAX];	// 키보드 상태
	BYTE					m_OldKeyState[KEYBOARDMAX];
	BYTE					m_NewKeyState[KEYBOARDMAX];

public:
	bool IintDirectInput(HINSTANCE hInst, HWND hWnd);		// 초기화
	void UpdateDirectInput(void);							// 업데이트
	void ReleaseDirectInput(void);							// 해제

	bool _InitKeyBorad(void);								// 키보드 디바이스 생성
	bool _InitMouse(void);									// 마우스 디바이스 생성

	void _ReadKeyBoradState(void);							// 키보드 상태 얻기
	void _ReadMouseState(void);								// 마우스 상태 얻기

	void _ReadKeyUpDownBitCheck(void);
	void _ReadMouseKeyUpDownBitCheck(void);

	// 복구
	bool _DXinputRestore(void);

	
public:
	// 외부 키값 처리 //
	bool _KeyDown(BYTE value);
	bool _KeyTickDown(BYTE value);
	bool _KeyUp(BYTE value);

	void _GetMouseRelativePt(LONG* pX, LONG* pY, LONG* pZ);

	bool _MouseDown(BYTE value);
	bool _MouseTickDown(BYTE value);
	bool _MouseUp(BYTE value);

	// 키 처리 확산성 위해 //
	friend bool KeyDown(BYTE value);
	friend bool KeyTickDown(BYTE value);
	friend bool KeyUp( BYTE value );
	
	friend void GetMouseRelativePt(LONG* pX, LONG* pY, LONG* pZ);

	friend bool MouseDown(BYTE value);
	friend bool MouseTickDown(BYTE value);
	friend bool MouseUp(BYTE value);

public:
	DXinput(void);
	virtual ~DXinput(void);
};
