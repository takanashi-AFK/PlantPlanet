#include "Component_Fall.h"

//�C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"

// �R���X�g���N�^
Component_Fall::Component_Fall(StageObject* _holder)
    : Component(_holder,"Component_Fall",Fall), fallSpeed_(0), riseSpeed_(0), isRising(false)
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
    // ���݂̃��[�J���ʒu���擾
    DirectX::XMFLOAT3 localPosition_ = holder_->GetLocalPosition();
    
    if (isRising) {
        // �㏸���̏���
        if (localPosition_.y < 5.0f){
            localPosition_.y += riseSpeed_;
        }
        else{
            // ���̂���荂���ɒB������㏸���~����
            isRising = false;
        }
    }
    else {
        // �~�����̏���
        if (localPosition_.y > -5.0f) {
            localPosition_.y -= (fallSpeed_ += fallSpeedplus_);   
        }
        else {
            // ���̂����̍����ɒB������㏸���J�n����
            isRising = true;
            fallSpeed_ = 0.5f; // fallSpeed_�����Z�b�g
        }
    }
    // ���[�J���ʒu��ݒ�
    holder_->SetLocalPosition(localPosition_.x, localPosition_.y, localPosition_.z);
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
    //ImGui::SliderFloat("Fall Speed", &fallSpeed_, 0.0f, 10.0f);
    //ImGui::SliderFloat("Rise Speed", &riseSpeed_, 0.0f, 10.0f);
}