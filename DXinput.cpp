#include "WinCommon.h"
#include "DXinput.h"

extern DXinput		g_DXInput;

DXinput::DXinput(void)
{
	m_pDI				= NULL;
	m_pKeyBoardDevice	= NULL;
	m_pMouseDevice		= NULL;

	memset(m_MouseKeyState, 0, MT_MAX);
	memset(m_MouseOldKeyState, 0, MT_MAX);
	memset(m_MouseNewKeyState, 0, MT_MAX);

	memset(m_KeyBoardState, 0, KEYBOARDMAX);
	memset(m_OldKeyState, 0, KEYBOARDMAX);
	memset(m_NewKeyState, 0, KEYBOARDMAX);
}

DXinput::~DXinput(void)
{
}

//
// 다이렉트 인풋 초기화
//
bool DXinput::IintDirectInput( HINSTANCE hInst, HWND hWnd )
{
	m_hInstance	= hInst;
	m_hWnd		= hWnd;

	// 기존의 값이 있다면 제거
	ReleaseDirectInput();

	// dx input 인터페이스 생성
	if( FAILED(DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
									(void**)&m_pDI, NULL)))
	{
		MessageBox(m_hWnd, "Direct Input Create Failed", "error", MB_OK);
		return false;
	}

	// 키보드 디바이스 생성
	if( _InitKeyBorad() == false )
	{
		ReleaseDirectInput();
		return false;
	}

	// 마우스 디바이스 생성
	if( _InitMouse() == false )
	{
		ReleaseDirectInput();
		return false;
	}

	return true;
}


//
// 다이렉트 인풋 업데이트( 상태 얻어오는 부분 )
//
void DXinput::UpdateDirectInput( void )
{
	_ReadKeyBoradState();
	_ReadMouseState();
}


void DXinput::_ReadKeyBoradState( void )
{
	if( m_pKeyBoardDevice == false )
		return;

	HRESULT res;

	m_pKeyBoardDevice->Poll();

	res = m_pKeyBoardDevice->GetDeviceState(sizeof(m_NewKeyState), m_NewKeyState);

	if(res != DI_OK)
	{
		_DXinputRestore();
		return;
	}

	_ReadKeyUpDownBitCheck();
}

void DXinput::_ReadKeyUpDownBitCheck( void )
{
	for( int i = 0; i < KEYBOARDMAX; ++i )
	{
		// 키가 눌려졌다면(KeyDown)
		if( m_NewKeyState[i] & 0x80 )
		{
			// 처음 눌러진 키라면
			if( !(m_OldKeyState[i] & 0x80) )
			{
				m_KeyBoardState[i] = 0x01;
			}
			// 계속 눌려지고 있다면
			else
			{
				if( m_OldKeyState[i] & 0x80 )
				{
					m_KeyBoardState[i] = 0x81;
				}
			}
		}

		// 키가 눌러지지 않았다면(KeyUp)
		else
		{
			// 이전에 키가 눌러진적이 없다면
			if( !(m_OldKeyState[i] & 0x80) )
			{
				m_KeyBoardState[i] = 0x00;
			}
			// 눌렀다가 떨어졌다면
			else
			{
				if( m_OldKeyState[i] & 0x80 )
				{
					m_KeyBoardState[i] = 0x80;
				}
			}
		}
	}

	memcpy(m_OldKeyState, m_NewKeyState, sizeof(BYTE)*KEYBOARDMAX);
}

void DXinput::_ReadMouseState( void )
{
	if( m_pMouseDevice == false ) return;

	HRESULT res;

	m_pMouseDevice->Poll();

	res = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);

	if(res != DI_OK)
	{
		_DXinputRestore();
		return;
	}

	
	_ReadMouseKeyUpDownBitCheck();
}

void DXinput::_ReadMouseKeyUpDownBitCheck( void )
{
	for( int i = 0; i < MT_MAX; ++i )
	{
		m_MouseNewKeyState[i] = m_MouseState.rgbButtons[i];

		// 버튼 눌림
		if( m_MouseNewKeyState[i] & 0x80 )
		{
			// 처음눌림
			if( !(m_MouseOldKeyState[i] & 0x80) )
			{
				m_MouseKeyState[i] = 0x01;
			}

			// 지속적 눌림
			else
			{
				if( m_MouseOldKeyState[i] & 0x80 )
				{
					m_MouseKeyState[i] = 0x81;
				}
			}
		}

		// 버튼 안눌림
		else
		{
			// 이전에 안눌림
			if( !(m_MouseOldKeyState[i] & 0x80) )
			{
				m_MouseKeyState[i] = 0x00;
			}

			// 이전에 눌렀다가 떨어짐
			else
			{
				if( m_MouseOldKeyState[i] & 0x80 )
				{
					m_MouseKeyState[i] = 0x80;
				}
			}
		}
	}

	memcpy(m_MouseOldKeyState, m_MouseNewKeyState, sizeof(BYTE)*MT_MAX);
}




//
// 메모리 해제
//
void DXinput::ReleaseDirectInput( void )
{
	if( m_pKeyBoardDevice )
		m_pKeyBoardDevice->Unacquire();

	if( m_pMouseDevice )
		m_pMouseDevice->Unacquire();

	SAFE_RELEASE(m_pMouseDevice);
	SAFE_RELEASE(m_pKeyBoardDevice);
	SAFE_RELEASE(m_pDI);
}

//
// 키보드 디바이스 생성
//
bool DXinput::_InitKeyBorad( void )
{
	// 키보드 디바이스 생성
	if( FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyBoardDevice, NULL)))
	{
		MessageBox(m_hWnd, "KeyBorad Dievce Create Failed", "error", MB_OK);
		return false;
	}

	// 데이터 포맷 설정
	if( FAILED(m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(m_hWnd, "KeyBorad Format Failed", "error", MB_OK);
		return false;
	}

	// 협력레벨 설정
	if( FAILED(m_pKeyBoardDevice->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )))
	{
		MessageBox(m_hWnd, "KeyBorad Level Failed", "error", MB_OK);
		return false;
	}

	// 키보드 
	m_pKeyBoardDevice->Acquire();

	return true;
}


//
// 마우스 디바이스 생성
//
bool DXinput::_InitMouse( void )
{
	// 마우스 디바이스 생성
	if( FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL)))
	{
		MessageBox(m_hWnd, "Mouse Dievce Create Failed", "error", MB_OK);
		return false;
	}

	// 데이터 포맷 설정
	if( FAILED(m_pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(m_hWnd, "Mouse Format Failed", "error", MB_OK);
		return false;
	}

	// 협력레벨 설정
	if( FAILED(m_pMouseDevice->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )))
	{
		MessageBox(m_hWnd, "Mouse Level Failed", "error", MB_OK);
		return false;
	}

	// 키보드 
	m_pMouseDevice->Acquire();

	return true;
}

bool DXinput::_DXinputRestore( void )
{
	if(m_pKeyBoardDevice)
	{
		HRESULT res = m_pKeyBoardDevice->Acquire();

		while(res == DIERR_INPUTLOST)
			res = m_pKeyBoardDevice->Acquire();
	}

	if(m_pMouseDevice)
	{
		HRESULT res = m_pMouseDevice->Acquire();

		while(res == DIERR_INPUTLOST)
			res = m_pMouseDevice->Acquire();
	}

	return true;
}

// 외부 키값 처리 //
bool DXinput::_KeyDown( BYTE value )
{
	if( m_KeyBoardState[value] == 0x81 )
		return true;

	return false;
}

bool DXinput::_KeyUp( BYTE value )
{
	if( m_KeyBoardState[value] == 0x80 )
	{
		m_KeyBoardState[value] = 0x00;
		return true;
	}

	return false;
}


void DXinput::_GetMouseRelativePt( LONG* pX, LONG* pY, LONG* pZ )
{
	if(pX) *pX = m_MouseState.lX;
	if(pY) *pY = m_MouseState.lY;
	if(pZ) *pZ = m_MouseState.lZ;
}


// 키 처리 확산성 위해 //
bool KeyDown(BYTE value)
{
	if( g_DXInput._KeyDown(value) )
		return true;

	return false;
}

bool KeyUp( BYTE value )
{
	if( g_DXInput._KeyUp(value) )
		return true;

	return false;
}

 void GetMouseRelativePt( LONG* pX, LONG* pY, LONG* pZ )
 {
	g_DXInput._GetMouseRelativePt(pX, pY, pZ);
 }