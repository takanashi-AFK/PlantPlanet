#include "StageObject.h"

StageObject::StageObject()
{
}

bool StageObject::AddComponent(Component* _comp)
{
	// �G���[����
	if (_comp == nullptr) 
	return false;

	// �R���|�[�l���g��vector�ɒǉ�����
	myComponents_.push_back(_comp); 
	return true;
}

bool StageObject::DeleteComponent(Component* _comp)
{
	// �C�e���[�^�ɁAcomponents_���ŒT�������f�[�^��o�^
	auto it = std::find(myComponents_.begin(), myComponents_.end(), _comp);

	//�C�e���[�^�����X�g�̍Ō�܂œ��B������֐��I��
	if (it == myComponents_.end()) 
	return false;

	//�C�e���[�^�̃R���|�[�l���g������
	myComponents_.erase(it); 
	return true;
}

void StageObject::Initialize()
{
	for (auto comp : myComponents_)
		comp->ChildIntialize();
}

void StageObject::Update()
{
	for (auto comp : myComponents_)
		comp->ChildUpdate();
}