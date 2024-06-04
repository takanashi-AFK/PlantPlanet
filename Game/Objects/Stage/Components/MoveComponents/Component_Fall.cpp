#include "Component_Fall.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../StageObject.h"

// �R���X�g���N�^
Component_Fall::Component_Fall(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder,_name,Fall,_parent)
	, fallSpeed_(), riseSpeed_(),fallDistance_(),
	isFalling_(false), isFirstTime_(true), isActive_(false), startRisePosition_(), startFallPosition_()
{
}

// ������
void Component_Fall::Initialize()
{
}

// �X�V
void Component_Fall::Update()
{   
	// ���s�������ǂ���
	if (isActive_) {

		XMFLOAT3 pPos = holder_->GetPosition();
		if (isFirstTime_) {
			// ���~�E�㏸�̊J�n�ʒu��ێ�����ϐ�
			startRisePosition_.y = pPos.y;
			startFallPosition_.y = pPos.y;

			isFirstTime_ = false;
		}

		if (isFalling_) {
			// �~�����̏���
			if (pPos.y > startFallPosition_.y - fallDistance_) {
				pPos.y -= fallSpeed_;
			}
			else {
				// ���̂����̍����ɒB������㏸���J�n����
				isFalling_ = false;
				pPos.y = startFallPosition_.y - fallDistance_;
			}
		}
		else {
			
			// �㏸���̏���
			if (pPos.y < startRisePosition_.y) {
				pPos.y += riseSpeed_;
			}
			else {
				// ���̂���荂���ɒB������㏸���~���A�~�����J�n����
				isFalling_ = true;
				isActive_ = false;
				isFirstTime_ = true;
				pPos.y = startRisePosition_.y;  // �㏸�J�n�ʒu�ɖ߂�
				startFallPosition_.y = pPos.y;  // �~���J�n�ʒu���L�^
			}
		}
		// �ʒu���m��
		holder_->SetPosition(pPos);
	}
}

// �J��
void Component_Fall::Release()
{
}

// �ۑ�
void Component_Fall::Save(json& _saveObj)
{
	// �ۑ�
	_saveObj["fallSpeed_"] = fallSpeed_;
	_saveObj["riseSpeed_"] = riseSpeed_;
	_saveObj["fallDistance_"] = fallDistance_;
}

// �Ǎ�
void Component_Fall::Load(json& _loadObj)
{
	// �Ǎ�
	if (_loadObj.contains("fallSpeed_"))fallSpeed_ = _loadObj["fallSpeed_"];
	if (_loadObj.contains("riseSpeed_"))riseSpeed_ = _loadObj["riseSpeed_"];
	if (_loadObj.contains("fallDistance_"))fallDistance_ = _loadObj["fallDistance_"];
}

// ImGui�\��
void Component_Fall::DrawData()
{
	// ImGui�`��
	ImGui::Text("Component_Fall");
	ImGui::DragFloat("fallSpeed_", &fallSpeed_, 0.1f);
	ImGui::DragFloat("riseSpeed_", &riseSpeed_, 0.1f);
	ImGui::DragFloat("fallDistance_", &fallDistance_, 0.1f);
	if (ImGui::Button("Execute"))Execute();
	
}
