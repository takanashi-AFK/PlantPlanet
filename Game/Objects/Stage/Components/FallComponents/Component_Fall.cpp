#include "Component_Fall.h"

// �C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"


// �R���X�g���N�^
Component_Fall::Component_Fall(StageObject* _holder)
    : Component(_holder,"Component_Fall",Fall), fallSpeed_(0), riseSpeed_(0), isRising_(false),isFirstTime_(true)
{
}

// ������
void Component_Fall::Initialize()
{
    // ������
    fallSpeed_ = 0.5f; 
    fallSpeedplus_ = 0.05f;
    riseSpeed_ = 0.05f; 
}

// �X�V
void Component_Fall::Update()
{   
   
    if (isFirstTime_) {
        
        // ���݂̃��[�J���ʒu���擾
        localPosition = holder_->GetLocalPosition();

        // ���~�E�㏸�̊J�n�ʒu��ێ�����ϐ�
        startRisePosition_.y = localPosition.y;
        startFallPosition_.y = localPosition.y;
        
        isFirstTime_ = false;
    }
    if (isActive_) {
        if (isRising_) {
            // �㏸���̏���
            if (localPosition.y < startRisePosition_.y) {
                localPosition.y += riseSpeed_;
                
            }
            else {
                // ���̂���荂���ɒB������㏸���~���A�~�����J�n����
                isRising_ = false;
                isActive_ = false;
                startFallPosition_.y = localPosition.y;  // �~���J�n�ʒu���L�^
                fallSpeed_ = 0.5f;  // �~�����x��������
            }
        }
        else {
            // �~�����̏���
            if (localPosition.y > startFallPosition_.y - 5.0f) {
                localPosition.y -= (fallSpeed_ += fallSpeedplus_);
            }
            else {
                // ���̂����̍����ɒB������㏸���J�n����
                isRising_ = true;
                //startRisePosition_.y = localPosition.y;  // �㏸�J�n�ʒu���L�^
            }
        }
        // ���[�J���ʒu��ݒ�
        holder_->SetLocalPosition(localPosition.x, localPosition.y, localPosition.z);
        holder_->SetPosition(localPosition);
        
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
    if (_loadObj.contains("fallSpeed")) {
        fallSpeed_ = _loadObj["fallSpeed"];
    }
    if (_loadObj.contains("riseSpeed")) {
        riseSpeed_ = _loadObj["riseSpeed"];
    }
}

// ImGui�\��
void Component_Fall::DrawData()
{
    // ImGui�`��
    ImGui::Text("Component_Fall");
    ImGui::DragFloat("fallSpeed_", &fallSpeed_, 0.1f);
    if (ImGui::Button("Falling")) {
        Execute();
    }
    //ImGui::SliderFloat("Fall Speed", &fallSpeed_, 0.0f, 10.0f);
    //ImGui::SliderFloat("Rise Speed", &riseSpeed_, 0.0f, 10.0f);
}
