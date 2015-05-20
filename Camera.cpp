#include "WinCommon.h"
#include "DXCommon.h"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////////
//
// 생성 및 소멸자
//
Camera::Camera(void)
{
	m_vEye		= D3DXVECTOR3(0,5,-15);
	m_vLookAt	= D3DXVECTOR3(0,0,0);
	m_vUp		= D3DXVECTOR3(0,1,0);

	m_vOrgEye	= m_vEye;
	m_vOrgLookAt= m_vLookAt;

	m_fFov		= D3DXToRadian(45);		
	m_fAspect	= (float)g_Mode.Width / g_Mode.Height;	
	m_fZnear	= 1.0f;	
	m_fZfar		= 1000.0f;	

	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mInvView);
	D3DXMatrixIdentity(&m_mProj);
	D3DXMatrixIdentity(&m_mRotate);

	m_bCameraAutoRotate	= false;
}

Camera::Camera( D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
			   float _Fov, float _Aspect, float _near, float _far )
{
	m_vEye		= _vEye;
	m_vLookAt	= _vLookAt;
	m_vUp		= _vUp;

	m_fFov		= _Fov;
	m_fAspect	= _Aspect;
	m_fZnear	= _near;
	m_fZfar		= _far;

	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mInvView);
	D3DXMatrixIdentity(&m_mProj);
	D3DXMatrixIdentity(&m_mRotate);

	m_bCameraAutoRotate	= false;
}

Camera::~Camera(void)
{
}



//////////////////////////////////////////////////////////////////////////
//
// 외부 호출 함수
//

// 뷰변환 //
void Camera::ViewTransFrom( void )
{
	D3DXMatrixLookAtLH(&m_mView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pDevice->SetTransform(D3DTS_VIEW, &m_mView);
}

// 프로젝션 변환 //
void Camera::ProjectionTransFrom( void )
{
	D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFov, m_fAspect, m_fZnear, m_fZfar);		
	g_pDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);	
}

// 카메라 업데이트 //
void Camera::UpdateCamera( float dTime )
{
	if( KeyUp(DIK_INSERT) )
		m_bCameraAutoRotate ^= true;

	if( m_bCameraAutoRotate )
	{
		static float angle = 0.0f;

		angle += 1.0f * dTime;
		D3DXMatrixRotationY(&m_mRotate, angle);

		D3DXVec3TransformCoord(&m_vEye, &m_vOrgEye, &m_mRotate);
		//D3DXVec3TransformCoord(&m_vLookAt, &m_vOrgEye, &m_mRotate);

		ViewTransFrom();
	}
}