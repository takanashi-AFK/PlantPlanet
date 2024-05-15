#include "Component_Fall.h"

//�C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
// �R���X�g���N�^
Component_Fall::Component_Fall(StageObject* _holder)
    : Component(_holder,"Component_Fall",Fall), fallSpeed_(0.0f), riseSpeed_(0.0f)
{
}

// ������
void Component_Fall::Initialize()
{
    // ������
    fallSpeed_ = 1.0f; 
    riseSpeed_ = 1.0f; 
}

// �X�V
void Component_Fall::Update()
{
    //�~���Ə㏸
    if (holder_->GetPosition().y > 0) {
        holder_->SetPosition(holder_->GetPosition().x, holder_->GetPosition().y - fallSpeed_, holder_->GetPosition().z);
    }
    else {
        holder_->SetPosition(holder_->GetPosition().x, holder_->GetPosition().y + riseSpeed_, holder_->GetPosition().z);
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
    ImGui::SliderFloat("Fall Speed", &fallSpeed_, 0.0f, 10.0f);
    ImGui::SliderFloat("Rise Speed", &riseSpeed_, 0.0f, 10.0f);
}