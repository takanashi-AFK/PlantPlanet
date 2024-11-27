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
	// effekseer: :Effect�̓ǂݍ���
	EFFEKSEERLIB::gEfk->AddEffect("foot", "Effects/foot.efk");/*������*/
}

void Component_WASDInputMove::Update()
{

	// ���̃R���|�[�l���g���A�N�e�B�u�łȂ��ꍇ�A�������I��
	if (isActive_ == false) return;
	isMove_ = false;

	// ��{�̃x�N�g����p�ӁA������
	dir_ = XMVectorZero();

	TPSCamera* tpsCamera = (TPSCamera*)holder_->FindObject("TPSCamera");
	if (tpsCamera == nullptr) return;

	XMFLOAT2 angle = tpsCamera->GetAngle();

	// �J�����̎����x�N�g�����擾�AY������0�ɂ��Đ��K��
	XMVECTOR sightLine = Camera::GetSightLine();
	sightLine = XMVectorSetY(sightLine, 0);
	sightLine = XMVector3Normalize(sightLine);
	angles_ = holder_->GetRotate();

	// ���͂ɉ����ĕ����x�N�g����ݒ� (�L�[�{�[�h��WASD�L�[)
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

	// �R���g���[������ (�Q�[���p�b�h�̍��X�e�B�b�N) ���擾
	XMFLOAT3 padMove = Input::GetPadStickL();
	XMVECTOR padDir = XMVectorSet(padMove.x, 0, padMove.y, 0);

	if (!XMVector3Equal(padDir, XMVectorZero())) {
		// �p�b�h���̓x�N�g�����J�����̌����ɍ��킹�ĉ�]������
		XMVECTOR rotatedPadDir = XMVector3Transform(padDir, XMMatrixRotationY(XMConvertToRadians(angle.y - ROTATE_OFFSET)));

		dir_ = XMVector3Normalize(rotatedPadDir);

		// �V������]�p�x���v�Z���A�L�����N�^�[��Y����]�ɔ��f
		float newAngle = std::atan2(XMVectorGetX(dir_), XMVectorGetZ(dir_));
		if (isRotate) holder_->SetRotateY(XMConvertToDegrees(newAngle));

		isMove_ = true;
	}

	// �ړ��x�N�g���𐳋K�����A���x���|�����킹��
	dir_ = XMVector3Normalize(dir_);
	XMVECTOR move = dir_ * speed;

	// �ړ��x�N�g����XMFLOAT3�ɕϊ����A�L�����N�^�[�̐V�����ʒu��ݒ�
	XMFLOAT3 pos = holder_->GetPosition();
	XMStoreFloat3(&pos, XMLoadFloat3(&pos) + move);

	// �V�����ʒu��ݒ�
	holder_->SetPosition(pos);

	angles_.y = XMConvertToDegrees(std::atan2(XMVectorGetX(dir_), XMVectorGetZ(dir_)));

	ImGui::Text(std::format("cita : {}", XMConvertToDegrees(std::atan2(XMVectorGetX(dir_), XMVectorGetZ(dir_)))).c_str());

	//�����̍X�V���s��
	if (isRotate && isMove_)	holder_->SetRotate(angles_);

	static int count = 0;
	count++;


	//FIX:�G�t�F�N�g�͂����ō�肽���Ȃ�
	if (isMove_ && count % EFFECT_RATE == 0) {
		// effekseer: :Effect�̍Đ����̐ݒ�
		EFFEKSEERLIB::EFKTransform effectTransform;/*������*/
		DirectX::XMStoreFloat4x4(&(effectTransform.matrix), holder_->GetWorldMatrix());/*������*/
		effectTransform.isLoop = false;/*������*/
		effectTransform.maxFrame = EFFECT_FRAME;/*������*/
		effectTransform.speed = EFFECT_SPEED;/*������*/
		// effekseer: :Effect�̍Đ�
		mt = EFFEKSEERLIB::gEfk->Play("foot", effectTransform);/*������*/
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
