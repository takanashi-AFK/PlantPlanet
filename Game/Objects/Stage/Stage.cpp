#include "Stage.h"

// �C���N���[�h
#include "StageObject.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/GameObject/Camera.h"

Stage::Stage(GameObject* _parent)
	:GameObject(_parent,"Stage"),objects_()
{
}

void Stage::Initialize()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
}

void Stage::Release()
{
}

void Stage::Save(json& _saveObj)
{
	// �I�u�W�F�N�g�Q��ۑ�
	for (auto obj : objects_)obj->Save(_saveObj[obj->GetObjectName()]);
}

void Stage::Load(json& _loadObj)
{
	// �X�e�[�W�I�u�W�F�N�g�����ׂč폜
	DeleteAllStageObject();

	for (auto it = _loadObj.begin(); it != _loadObj.end();++it) {
		
		// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
		StageObject* obj = CreateStageObject(it.key(), it.value()["modelFilePath_"], this);

		// �I�u�W�F�N�g����Ǎ�
		obj->Load(it.value());

		// �I�u�W�F�N�g�����X�g�ɒǉ�
		AddStageObject(obj);
	}
}

void Stage::DrawData()
{
	// �I�u�W�F�N�g��`��
	for (auto obj : objects_)obj->DrawData();
}

void Stage::AddStageObject(StageObject* _obj)
{
	// ���X�g�ɒǉ�
	if (_obj != nullptr)objects_.push_back(_obj);
}

void Stage::DeleteStageObject(StageObject* _obj)
{

	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(objects_.begin(), objects_.end(), _obj);

	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != objects_.end()) objects_.erase(it);

	// �I�u�W�F�N�g���폜����
	_obj->KillMe();
}

void Stage::DeleteAllStageObject()
{
	// ���X�g���ɂ���v�f�����ׂč폜
	for (auto obj : objects_) obj->KillMe();
	objects_.clear();
}

StageObject* Stage::GetStageObject(string _name)
{
	// �I�u�W�F�N�g������
	for (auto obj : objects_) {
		if (obj->GetObjectName() == _name)return obj;
	}
	return nullptr;

}

Component* Stage::FindComponent(string _name)
{
	Component* comp = nullptr;
	for (auto obj : objects_) {
		comp = obj->FindComponent(_name);
		if (comp != nullptr)return comp;
	}
	return nullptr;
}

vector<Component*> Stage::FindComponents(ComponentType _type)
{
	vector<Component*> result;
	for (auto obj : objects_) {

		// �R���|�[�l���g���ċA�I�Ɍ���
		auto comps = obj->FindComponent(_type);
		result.insert(result.end(), comps.begin(), comps.end());
	}
	return result;
}
