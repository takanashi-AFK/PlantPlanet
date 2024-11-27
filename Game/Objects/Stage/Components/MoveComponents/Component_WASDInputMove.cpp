#include "Component_WASDInputMove.h"
#include "../../../../../Engine/directX/Input.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../StageObject.h"
#include "../../../../../Engine/GameObject/Camera.h"
#include "../../../../../Engine/GameObject/GameObject.h"
#include "../../../Camera/TPSCamera.h"
#include <cmath>
#include <format>

namespace
{
	float speed = 0.1f;
	const int EFFECT_RATE = 15;
	const int ROTATE_OFFSET = 25;
	const int ROTATE_BESIDE = 90;
	const int ROTATE_BACK = 180;
	const int EFFECT_FRAME = 40;
	const int EFFECT_SPEED = 1;
}


Component_WASDInputMove::Component_WASDInputMove(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, WASDInputMove, _parent), isRotate(true),
	isMove_(false),
	dir_(XMVectorSet(0, 0, 0, 0))
{
}

void Component_WASDInputMove::Initialize()
{
	// effekseer: :Effectの読み込み
	EFFEKSEERLIB::gEfk->AddEffect("foot", "Effects/foot.efk");/*★★★*/
}

void Component_WASDInputMove::Update()
{

	// このコンポーネントがアクティブでない場合、処理を終了
	if (isActive_ == false) return;
	isMove_ = false;

	// 基本のベクトルを用意、初期化
	dir_ = XMVectorZero();

	TPSCamera* tpsCamera = (TPSCamera*)holder_->FindObject("TPSCamera");
	if (tpsCamera == nullptr) return;

	XMFLOAT2 angle = tpsCamera->GetAngle();

	// カメラの視線ベクトルを取得、Y成分を0にして正規化
	XMVECTOR sightLine = Camera::GetSightLine();
	sightLine = XMVectorSetY(sightLine, 0);
	sightLine = XMVector3Normalize(sightLine);
	angles_ = holder_->GetRotate();

	// 入力に応じて方向ベクトルを設定 (キーボードのWASDキー)
	if (Input::IsKey(DIK_W)) {
		dir_ += sightLine;
		isMove_ = true;

		direcionType_ = DIRECTION::FORWARD;
		angles_.y = angle.y - ROTATE_OFFSET;
	}
	if (Input::IsKey(DIK_A)) {
		dir_ += XMVector3Transform(sightLine, XMMatrixRotationY(XMConvertToRadians(-ROTATE_BESIDE)));
		isMove_ = true;

		direcionType_ = DIRECTION::LEFT;
		angles_.y = (angle.y - ROTATE_OFFSET) - ROTATE_BESIDE;
	}
	if (Input::IsKey(DIK_S)) {
		dir_ += -sightLine;
		isMove_ = true;

		direcionType_ = DIRECTION::BACK;
		angles_.y = (angle.y - ROTATE_OFFSET) + ROTATE_BACK;
	}
	if (Input::IsKey(DIK_D)) {
		dir_ += XMVector3Transform(sightLine, XMMatrixRotationY(XMConvertToRadians(ROTATE_BESIDE)));
		isMove_ = true;

		direcionType_ = DIRECTION::RIGHT;
		angles_.y = (angle.y - ROTATE_OFFSET) + ROTATE_BESIDE;
	}

	// コントローラ入力 (ゲームパッドの左スティック) を取得
	XMFLOAT3 padMove = Input::GetPadStickL();
	XMVECTOR padDir = XMVectorSet(padMove.x, 0, padMove.y, 0);

	if (!XMVector3Equal(padDir, XMVectorZero())) {
		// パッド入力ベクトルをカメラの向きに合わせて回転させる
		XMVECTOR rotatedPadDir = XMVector3Transform(padDir, XMMatrixRotationY(XMConvertToRadians(angle.y - ROTATE_OFFSET)));

		dir_ = XMVector3Normalize(rotatedPadDir);

		// 新しい回転角度を計算し、キャラクターのY軸回転に反映
		float newAngle = std::atan2(XMVectorGetX(dir_), XMVectorGetZ(dir_));
		if (isRotate) holder_->SetRotateY(XMConvertToDegrees(newAngle));

		isMove_ = true;
	}

	// 移動ベクトルを正規化し、速度を掛け合わせる
	dir_ = XMVector3Normalize(dir_);
	XMVECTOR move = dir_ * speed;

	// 移動ベクトルをXMFLOAT3に変換し、キャラクターの新しい位置を設定
	XMFLOAT3 pos = holder_->GetPosition();
	XMStoreFloat3(&pos, XMLoadFloat3(&pos) + move);

	// 新しい位置を設定
	holder_->SetPosition(pos);

	angles_.y = XMConvertToDegrees(std::atan2(XMVectorGetX(dir_), XMVectorGetZ(dir_)));

	ImGui::Text(std::format("cita : {}", XMConvertToDegrees(std::atan2(XMVectorGetX(dir_), XMVectorGetZ(dir_)))).c_str());

	//向きの更新を行う
	if (isRotate && isMove_)	holder_->SetRotate(angles_);

	static int count = 0;
	count++;


	//FIX:エフェクトはここで作りたくない
	if (isMove_ && count % EFFECT_RATE == 0) {
		// effekseer: :Effectの再生情報の設定
		EFFEKSEERLIB::EFKTransform effectTransform;/*★★★*/
		DirectX::XMStoreFloat4x4(&(effectTransform.matrix), holder_->GetWorldMatrix());/*★★★*/
		effectTransform.isLoop = false;/*★★★*/
		effectTransform.maxFrame = EFFECT_FRAME;/*★★★*/
		effectTransform.speed = EFFECT_SPEED;/*★★★*/
		// effekseer: :Effectの再生
		mt = EFFEKSEERLIB::gEfk->Play("foot", effectTransform);/*★★★*/
		count = 0;

	}

}

void Component_WASDInputMove::Release()
{
}

void Component_WASDInputMove::DrawData()
{
	ImGui::Checkbox("Active", &isActive_);
	ImGui::SliderFloat("Speed", &speed, 0.01f, 1.0f);
}

void Component_WASDInputMove::Save(json& _saveObj)
{
	_saveObj["isActive_"] = isActive_;
	_saveObj["speed_"] = speed;
}

void Component_WASDInputMove::Load(json& _loadObj)
{
	if (_loadObj.contains("isActive_"))isActive_ = _loadObj["isActive_"];
	if (_loadObj.contains("speed_"))speed = _loadObj["speed_"];
}

void Component_WASDInputMove::Move(XMVECTOR _dir, float _speed)
{
	XMFLOAT3 pos = holder_->GetPosition();
	XMStoreFloat3(&pos, XMLoadFloat3(&pos) + _dir * _speed);
	holder_->SetPosition(pos);
}
