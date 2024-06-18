#include "UIPanel.h"

// �C���N���[�h
#include "UIObject.h"

UIPanel::UIPanel(GameObject* parent)
	: GameObject(parent, "UIPanel"), objects_()
{
}

void UIPanel::Initialize()
{
}

void UIPanel::Update()
{
}

void UIPanel::Draw()
{
}

void UIPanel::Release()
{
}

void UIPanel::Save(json& _saveObj)
{
	// �e�I�u�W�F�N�g�̕ۑ�����
	for (auto obj : objects_)obj->ChildSave(_saveObj[obj->GetObjectName()]);
}

void UIPanel::Load(json& _loadObj)
{
	// �S�ẴI�u�W�F�N�g���폜
	DeleteAllUIObject();

	for (auto it = _loadObj.begin(); it != _loadObj.end(); ++it) {

		// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
		UIObject* obj = CreateUIObject(it.key(), it.value()["type_"], this);

		// �I�u�W�F�N�g����Ǎ�
		obj->ChildLoad(it.value());

		// �I�u�W�F�N�g�����X�g�ɒǉ�
		AddUIObject(obj);
	}
}

void UIPanel::DrawData()
{
	// �e�I�u�W�F�N�g�̕`�揈��
	for (auto obj : objects_)obj->DrawData();
}

void UIPanel::AddUIObject(UIObject* _object)
{
	// ���X�g�ɒǉ�
	if (_object != nullptr)objects_.push_back(_object);
}

void UIPanel::DeleteUIObject(UIObject* _object)
{
	// �I�u�W�F�N�g���폜����
	_object->KillMe();

	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(objects_.begin(), objects_.end(), _object);

	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != objects_.end()) objects_.erase(it);
}

void UIPanel::DeleteAllUIObject()
{
	// �S�ẴI�u�W�F�N�g���폜
	for (auto obj : objects_)obj->KillMe();
	objects_.clear();
}

UIObject* UIPanel::GetUIObject(string _name)
{
	UIObject* result = nullptr;

	for (auto obj : objects_) {
		if (obj->GetObjectName() == _name) {
			result = obj;
			break;
		}
	}
	return result;
}
