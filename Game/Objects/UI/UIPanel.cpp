#include "UIPanel.h"

#include "../../Objects/UI/UIButton.h"

UIPanel::UIPanel(GameObject* _parent)
	: UIObject("UIPanel", _parent)
{
}

UIPanel::~UIPanel()
{
}

void UIPanel::Initialize()
{
	//UIButton�N���X�̃C���X�^���X�𐶐�
	UIButton * button = new UIButton(this);

	button->Initialize();
	AddChild(button);
}

void UIPanel::Update()
{
	//�q�I�u�W�F�N�g���X�V
	for (UIObject* child : childList) {
		child->Update();
	}

	//UIObject::Update();
}

void UIPanel::Draw()
{
	//�q�I�u�W�F�N�g��`��
	for (UIObject* child : childList) {
		child->Draw();
	}
	//UIObject::Draw();
}

void UIPanel::Release()
{
	// �q�I�u�W�F�N�g�����ׂč폜
	ClearChildren();
}

void UIPanel::Save(json& _saveUiobj)
{
}
	
void UIPanel::Load(json& _loadUiobj)
{
}

void UIPanel::AddChild(UIObject* _child)
{
	// �q�I�u�W�F�N�g��ǉ�
	childList.push_back(_child);
}

void UIPanel::RemoveChild(UIObject* _child)
{
	// �q�I�u�W�F�N�g���폜
	childList.remove(_child);
}

void UIPanel::ClearChildren()
{
	// childList�̒��g���Q�Ƃ��ă��[�v����
	for (UIObject* child : childList) {
		// �q�I�u�W�F�N�g�����
		child->Release();
		delete child;
	}
	// childList�̒��g��S�č폜
	childList.clear();
}