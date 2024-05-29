#include "Component_Fall.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../StageObject.h"


// �R���X�g���N�^
Component_Fall::Component_Fall(StageObject* _holder)
    : Component(_holder,"Component_Fall",Fall)
	, fallSpeed_(0.5f), fallSpeedplus_(0.05f), riseSpeed_(0.05f),fallDistance_(5.0f),
	isRising_(false), isFirstTime_(true)
{
}

// ������
void Component_Fall::Initialize()
{
    
}

// �X�V
void Component_Fall::Update()
{   
	if (isActive_) {

		XMFLOAT3 pPos = holder_->GetPosition();
		if (isFirstTime_) {
			// ���~�E�㏸�̊J�n�ʒu��ێ�����ϐ�
			startRisePosition_.y = pPos.y;
			startFallPosition_.y = pPos.y;

			isFirstTime_ = false;
		}

		if (isRising_) {
			// �㏸���̏���
			if (pPos.y < startRisePosition_.y) {
				pPos.y += riseSpeed_;
			}
			else {
				// ���̂���荂���ɒB������㏸���~���A�~�����J�n����
				isRising_ = false;
				isActive_ = false;
				isFirstTime_ = true;
				pPos.y = startRisePosition_.y;  // �㏸�J�n�ʒu�ɖ߂�
				startFallPosition_.y = pPos.y;  // �~���J�n�ʒu���L�^
			}
		}
		else {
			// �~�����̏���
			if (pPos.y > startFallPosition_.y - fallDistance_) {
				pPos.y -= (fallSpeed_ + fallSpeedplus_);
			}
			else {
				// ���̂����̍����ɒB������㏸���J�n����
				isRising_ = true;
				pPos.y = startFallPosition_.y - fallDistance_;
			}
		}

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
    _saveObj["fallSpeed"] = fallSpeed_;
    _saveObj["riseSpeed"] = riseSpeed_;
}

// �Ǎ�
void Component_Fall::Load(json& _loadObj)
{
    // �Ǎ�
    if (_loadObj.contains("fallSpeed"))fallSpeed_ = _loadObj["fallSpeed"];
    
    if (_loadObj.contains("riseSpeed"))riseSpeed_ = _loadObj["riseSpeed"];
    
}

// ImGui�\��
void Component_Fall::DrawData()
{
    // ImGui�`��
    ImGui::Text("Component_Fall");
    ImGui::DragFloat("fallSpeed_", &fallSpeed_, 0.1f);
	ImGui::DragFloat("fallSpeedPuls_", &fallSpeedplus_, 0.1f);
	ImGui::DragFloat("riseSpeed_", &riseSpeed_, 0.1f);
	ImGui::DragFloat("fallDistance_", &fallDistance_, 0.1f);
    if (ImGui::Button("Falling")) {
        Execute();
    }
    //ImGui::SliderFloat("Fall Speed", &fallSpeed_, 0.0f, 10.0f);
    //ImGui::SliderFloat("Rise Speed", &riseSpeed_, 0.0f, 10.0f);
}
