#include "Component.h"

Component::Component(StageObject* _holder, string _name,ComponentType _type)
    :holder_(_holder), name_(_name),type_(_type)
{
}

void Component::ChildIntialize()
{
	// ���g�̏�����
	this->Initialize();

	// �q�R���|�[�l���g�̏�����
	for (auto comp : childComponents_)comp->ChildIntialize();
}

void Component::ChildUpdate()
{
	// ���g�̍X�V
	this->Update();

	// �q�R���|�[�l���g�̍X�V
	for (auto comp : childComponents_)comp->ChildUpdate();
}

void Component::ChildRelease()
{
	// �q�R���|�[�l���g�̊J��
	for (auto comp : childComponents_) comp->ChildRelease();
	childComponents_.clear();
	
	// ���g�̊J��
	this->Release();
}

void Component::ChildSave(json& _saveObj)
{
	// ���g�̕ۑ�
	this->Save(_saveObj);

	// �q�R���|�[�l���g�̏�����
	for (auto comp : childComponents_)comp->ChildSave(_saveObj);
}

void Component::ChildLoad(json& _loadObj)
{
	// ���g�̓Ǎ�
	this->Load(_loadObj);

	// ���g�̃R���|�[�l���g��Ǎ�
	for (auto it = _loadObj["childComponents_"].begin(); it != _loadObj["childComponents_"].end(); ++it) LoadComponent((ComponentType)std::stoi(it.key()), it.value());
}

void Component::ChildDrawData()
{
	// ���g�̏���`��
	this->DrawData();

	// �q�R���|�[�l���g�̏���`��
	for (auto comp : childComponents_)comp->ChildDrawData();
}

Component* LoadComponent(ComponentType _type, json& _loadObj)
{
	// �R���|�[�l���g�𐶐�
	Component* comp = nullptr;
	switch (_type){

	}

	// �R���|�[�l���g�ɕK�v�ȏ���Ǎ�
	comp->Load(_loadObj);
	return comp;
}
