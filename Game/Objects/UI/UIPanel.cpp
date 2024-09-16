#include "UIPanel.h"
#include "../../Otheres/GameEditor.h"

UIPanel* UIPanel::instance_ = nullptr;

UIPanel::UIPanel()
	: UIObject()
{
}

UIPanel* UIPanel::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new UIPanel();
	}
		
	return instance_;
}

void UIPanel::Initialize()
{
}

void UIPanel::Update()
{
	
}

void UIPanel::Draw()
{
	//���C���[�ԍ��ŃI�u�W�F�N�g���\�[�g
	SortUIObject();
}

void UIPanel::Release()
{
}

void UIPanel::Save(json& _saveObj)
{
	// �e�I�u�W�F�N�g�̕ۑ�����
	for (auto obj : childList_)obj->ChildSave(_saveObj[obj->GetObjectName()]);
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
	}
}

void UIPanel::DrawData()
{
	// �e�I�u�W�F�N�g�̃c���[�̕`��
	for (auto obj : childList_)
	{
		obj->DrawData();
	}
}

void UIPanel::DeleteUIObject(UIObject* _object)
{
	// �I�u�W�F�N�g���폜����
	_object->KillMe();

	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(childList_.begin(), childList_.end(), _object);
	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != childList_.end()) childList_.erase(it);
}

void UIPanel::DeleteAllUIObject()
{
	// �S�ẴI�u�W�F�N�g���폜
	for (auto obj : childList_)obj->KillMe();
	childList_.clear();
}


void UIPanel::SortUIObject()
{
	// ���C���[�ԍ��Ń\�[�g
	std::sort(childList_.begin(), childList_.end(), UIObject::CompareLayerNumber);
}

UIObject* UIPanel::GetUIObject(std::string _name)
{
	// ���O�ŃI�u�W�F�N�g������
	for (auto obj : childList_)
	{
		if (obj->GetObjectName() == _name)return obj;
	}
	return nullptr;
}

vector<UIObject*> UIPanel::GetUIObject(UIType _type)
{
	// �^�C�v�ŃI�u�W�F�N�g������
	vector<UIObject*> result;
	for (auto obj : childList_)
	{
		if (obj->GetType() == _type)result.push_back(obj);
	}
	return result;
}
