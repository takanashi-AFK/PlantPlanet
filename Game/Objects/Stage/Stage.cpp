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
	static XMFLOAT3 camPos = { 1.4f,5.f,3.8f };
	static XMFLOAT3 camTgt = { -8.4f,-3.2f,25.f };
#ifdef _DEBUG
	ImGui::DragFloat3("Camera Position", &camPos.x, 0.1f);
	ImGui::DragFloat3("Camera Target", &camTgt.x, 0.1f);
#endif // _DEBUG

	// �J�����̈ʒu��ݒ�
	Camera::SetPosition(camPos);
	Camera::SetTarget(camTgt);
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
	// �I�u�W�F�N�g���폜����
	_obj->KillMe();

	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(objects_.begin(), objects_.end(), _obj);

	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != objects_.end()) objects_.erase(it);
}

void Stage::DeleteAllStageObject()
{
	// ���X�g���ɂ���v�f�����ׂč폜
	for (auto obj : objects_) obj->KillMe();
	objects_.clear();
}
