#include "UIText.h"
#include "../../../Engine/ResourceManager/Text.h"
#include "../../../Engine/ImGui/imgui.h"

UIText::UIText(string _name, GameObject* parent)
	: UIObject(_name, UI_TEXT, parent), pText_(nullptr), drawText_("")
{
}

void UIText::Initialize()
{
	// �e�L�X�g�I�u�W�F�N�g�𐶐���������
	pText_ = new Text;
	pText_->Initialize();
}

void UIText::Update()
{
}

void UIText::Draw()
{
	// �e�L�X�g�`��
	pText_->Draw(30,30,drawText_.c_str()) ;
}

void UIText::Release()
{
	// �e�L�X�g�I�u�W�F�N�g�̉��
	pText_->Release();
}

void UIText::Save(json& saveObj)
{
}

void UIText::Load(json& loadObj)
{
}

void UIText::DrawData()
{
	// ��������������������������������������������
	// �\������e�L�X�g��ύX
	// ��������������������������������������������
	/*if (ImGui::TreeNode("name")) {
		ImGui::Text("Current Text : %s", drawText_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New name...", buffer, IM_ARRAYSIZE(buffer)))
			this->objectName_ = buffer;
		ImGui::TreePop();
	}*/
}
