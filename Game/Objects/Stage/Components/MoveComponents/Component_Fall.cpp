#include "Component_Fall.h"

//�C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
// �R���X�g���N�^
Component_Fall::Component_Fall(StageObject* _holder)
    : Component(_holder,"Component_Fall",Fall), fallSpeed_(0), riseSpeed_(0), isRising_(false), tempFallSpeed_(0)
{
}

// ������
void Component_Fall::Initialize()
{
    // ������
    //fallSpeed_ = 0.5f; 
    //fallSpeedplus_ = 0.05f;
    //riseSpeed_ = 0.05f; 
   
}

// �X�V
void Component_Fall::Update()

{   // ���݂̈ʒu���擾
    XMFLOAT3 position_ = holder_->GetPosition();


    if (isRising_ == true) {
        // �㏸���̏���
        if (position_.y < 5.0f) { 
            holder_->SetPosition(position_.x, position_.y += riseSpeed_, position_.z);
        }
        else {
            // ���̂���荂���ɒB������㏸���~����
            isRising_ = false;
            fallSpeed_ = tempFallSpeed_;
        }
    }
    else {
        // �~�����̏���
        if (position_.y > -5.0f) {
            //holder_->SetPosition(position_.x, position_.y -= fallSpeed_, position_.z);
            holder_->SetPosition(position_.x, position_.y -= (fallSpeed_ += fallSpeedplus_), position_.z);
        }
        else {
            // ���̂����̍����ɒB������㏸���J�n����
            isRising_ = true;
           
        }
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
    ImGui::DragFloat("fallSpeed_", &tempFallSpeed_, 0.1f);

    if (ImGui::Button("fall")) {
        Falling();
    }
    if (ImGui::Button("rise")) {
		Rising();
	}
    //ImGui::SliderFloat("Fall Speed", &fallSpeed_, 0.0f, 10.0f);
    //ImGui::SliderFloat("Rise Speed", &riseSpeed_, 0.0f, 10.0f);
}