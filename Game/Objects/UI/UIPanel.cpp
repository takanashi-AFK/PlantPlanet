#include "UIPanel.h"
#include "UIButton.h"
#include "UIImage.h"
#include "../../Otheres/GameEditor.h"

UIPanel* pUIPanel_ = new UIPanel;

UIPanel::UIPanel()
	: UIObject("UIPanel",UI_PANEL,nullptr,0)
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
	//���C���[�ԍ��ŃI�u�W�F�N�g���\�[�g
	SortUIObject();
	// �e�I�u�W�F�N�g�̕`�揈��
	for (auto obj : childList_)
	{
		//obj->SortChildren();
		obj->DrawData();

	}

	//SortChildren();

	/*SortUIObject();
	DrawData();*/

}

void UIPanel::Release()
{
	DeleteAllUIObject();
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

		// �I�u�W�F�N�g�����X�g�ɒǉ�
		//AddUIObject(obj);
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

//void UIPanel::AddUIObject(UIObject* _object)
//{
//	// ���X�g�ɒǉ�
//	if (_object != nullptr)childList_.push_back(_object);
//}

void UIPanel::DeleteUIObject(UIObject* _object)
{
	// �I�u�W�F�N�g���폜����
	_object->KillMe();

	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(childList_.begin(), childList_.end(), _object);
	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != childList_.end()) childList_.erase(it);

	////chaildList_�ɂ���q�I�u�W�F�N�g��Release���Ă�
	//for (auto obj : childList_) {
	//	obj->Release();
	//}



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


//UIObject* UIPanel::GetUIObject(string _name)
//{
//	UIObject* result = nullptr;
//
//	for (auto obj : childList_) {
//		if (obj->GetObjectName() == _name) {
//			result = obj;
//			break;
//		}
//	}
//	return result;
//}
