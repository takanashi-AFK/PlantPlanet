#include "UIText.h"
#include "../../../Engine/ResourceManager/Text.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"

UIText::UIText(string _name, GameObject* parent)
	: UIObject(_name, UI_TEXT, parent), pText_(nullptr), drawText_(""), size_(1.f), floatNum_(nullptr)
{
}

void UIText::Initialize()
{
	// �e�L�X�g�I�u�W�F�N�g�𐶐���������
	pText_ = new Text;
	pText_->Initialize();

	LockScale();
	LockRotate();

}

void UIText::Update()
{
}

void UIText::Draw()
{
	// �\���T�C�Y���v�Z
	pText_->SetScale(size_);

	// �\���ʒu���v�Z
	float drawX = (transform_.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);		
	float drawY = (transform_.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);
	
	// �e�L�X�g��`��
	if (floatNum_ == nullptr)pText_->Draw(drawX, drawY, drawText_.c_str());
	else pText_->Draw(drawX, drawY,*floatNum_);
}

void UIText::Release()
{
	// �e�L�X�g�I�u�W�F�N�g�̉��
	pText_->Release();
}

void UIText::Save(json& saveObj)
{
	saveObj["text"] = drawText_;
	saveObj["size"] = size_;
}

void UIText::Load(json& loadObj)
{
	if (loadObj.contains("text"))drawText_ = loadObj["text"].get<string>();
	if (loadObj.contains("size"))size_ = loadObj["size"].get<float>();
}

void UIText::DrawData()
{
	// ��������������������������������������������
	// �\������e�L�X�g��ύX
	// ��������������������������������������������
	if(floatNum_ == nullptr)
	if (ImGui::TreeNode("TextString")) {
		ImGui::Text("Current Text : %s", drawText_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New Text...", buffer, IM_ARRAYSIZE(buffer)))
			this->drawText_ = buffer;
		ImGui::TreePop();
	}

	// ��������������������������������������������
	// �t�H���g�T�C�Y��ύX
	// ��������������������������������������������
	ImGui::DragFloat("size", &size_, 0.01f);
}
