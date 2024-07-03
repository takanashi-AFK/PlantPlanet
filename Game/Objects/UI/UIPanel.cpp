#include "UIPanel.h"
#include "UIButton.h"
#include "UIImage.h"
#include "../../Otheres/GameEditor.h"

UIPanel::UIPanel(std::string name, UIObject* parent, int layerNum)
	: UIObject(name, UI_PANEL, parent, layerNum), objects_()
{
}

void UIPanel::Initialize()
{
	UIPanel* pUIButton_ = (UIPanel*)CreateUIObject(objectName_, UI_BUTTON, nullptr, 0);
	UIPanel* pUIImage_ = (UIPanel*)CreateUIObject(objectName_, UI_IMAGE, nullptr, 0);
}

void UIPanel::Update()
{
}

void UIPanel::Draw()
{
	//���C���[�ԍ��ŃI�u�W�F�N�g���\�[�g
	SortUIObject();

	// �e�I�u�W�F�N�g�̕`�揈��
	for (auto obj : objects_)
	{
		//obj->SortChildren();
		obj->DrawData();

	}

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

		// ���C���[�ԍ���ǂݎ��
		int layerNumber = it.value().value("layerNumber", 0);  // �f�t�H���g�l0

		// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
		UIObject* obj = CreateUIObject(it.key(), it.value()["type_"], this,layerNumber);

		// �I�u�W�F�N�g����Ǎ�
		obj->ChildLoad(it.value());

		// �I�u�W�F�N�g�����X�g�ɒǉ�
		AddUIObject(obj);
	}
}

void UIPanel::DrawData()
{
	// �e�I�u�W�F�N�g�̕`�揈��
	for (auto obj : objects_)
	{
		obj->DrawData();
	}
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

void UIPanel::SortUIObject()
{
	// ���C���[�ԍ��Ń\�[�g
	std::sort(objects_.begin(), objects_.end(), UIObject::CompareLayerNumber);
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
