#include "camera.h"
#include "../DirectX/Direct3D.h"

XMFLOAT3 _position;
XMFLOAT3 _target;
XMMATRIX _view;
XMMATRIX _proj;
XMMATRIX _billBoard;

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
	_position = XMFLOAT3(0, 3, -10);	//カメラの位置
	_target = XMFLOAT3( 0, 0, 0);	//カメラの焦点

	//プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//更新（ビュー行列作成）
void Camera::Update()
{

	// もしポジションがnullだったら
	if (_position.x < -1.0e18f || _position.y < -1.0e18f || _position.z < -1.0e18f ||
		_position.x > 1.0e18f || _position.y > 1.0e18f || _position.z > 1.0e18f ||
		_target.x < -1.0e18f || _target.y < -1.0e18f || _target.z < -1.0e18f ||
		_target.x > 1.0e18f || _target.y > 1.0e18f || _target.z > 1.0e18f)
	{
		Camera::SetPosition({ 0,30,-20 });
		Camera::SetTarget({ 0,-20,20 });

	}
	//ビュー行列
	_view = XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, 0),
		XMVectorSet(_target.x, _target.y, _target.z, 0), XMVectorSet(0, 1, 0, 0));


	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);
}

//焦点を設定
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }
void Camera::SetTarget(float x, float y, float z) { Camera::SetTarget(XMFLOAT3(x, y, z)); }

//位置を設定
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }
void Camera::SetPosition(float x, float y, float z) { Camera::SetPosition(XMFLOAT3(x, y, z)); }

//プロジェクション行列を設定
void Camera::SetProjectionMatrix(XMMATRIX proj) { _proj = proj; }
void Camera::SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ) { SetProjectionMatrix(XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ)); }
void Camera::SetProjectionMatrix(float aspect) { SetProjectionMatrix(XM_PIDIV4, aspect, 0.1f, 1000.0f); }


//焦点を取得
XMFLOAT3 Camera::GetTarget() { return _target; }

//位置を取得
XMFLOAT3 Camera::GetPosition() { return _position; }

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return _view; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }

XMVECTOR Camera::GetSightLine() { return XMLoadFloat3(&_target) - XMLoadFloat3(&_position); }
