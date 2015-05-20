//
// SWRender Basic FrameWork
//
// Camera.h : DX관련
//
// ==================================================================//
// 2012.7.24 : 간단한 변환만 갖는 클래스
//

#pragma once

class Camera
{
public:
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 기본정보
	D3DXVECTOR3				m_vEye;					// 카메라 위치
	D3DXVECTOR3				m_vLookAt;				// 카메라 바라보는 방향
	D3DXVECTOR3				m_vUp;					// 카메라 UP벡터
													//  : UP벡터는 추후 이동 및 회전 축을 구할때 사용

	D3DXVECTOR3				m_vOrgEye;				// 카메라 위치
	D3DXVECTOR3				m_vOrgLookAt;			// 카메라 바라보는 방향

	D3DXMATRIX				m_mView;				// 뷰변환 행렬
	D3DXMATRIX				m_mInvView;				// 뷰행렬의 역행열( 추후 사용 )
	D3DXMATRIX				m_mProj;				// 프로젝션 변환 행렬

	float					m_fFov;					// 시야각
	float					m_fAspect;				// 가로세로 비율
	float					m_fZnear;				// 시야 최소 거리
	float					m_fZfar;				// 시야 최대 거리

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 자동회전 관련
	bool					m_bCameraAutoRotate;	// 자동회전
	D3DXMATRIX				m_mRotate;



	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 생성 및 소멸자
public:
	Camera(void);
	Camera(D3DXVECTOR3 _vEye, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp,
			float _Fov, float _Aspect, float _near, float _far);
	virtual ~Camera(void);


	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━//
	// 외부 호출 함수
	void UpdateCamera(float dTime);
	void ViewTransFrom(void);
	void ProjectionTransFrom(void);
};
