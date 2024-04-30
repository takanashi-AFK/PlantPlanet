#include "Component.h"

Component::Component(StageObject* _holder, string _name)
    :holder_(_holder), name_(_name)
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

void Component::ChildDrawData()
{
	// ���g�̏���`��
	this->DrawData();

	// �q�R���|�[�l���g�̏���`��
	for (auto comp : childComponents_)comp->ChildDrawData();
}
