#include "Component.h"
#include "../StageObject.h"

#include "RotationYComponent.h"


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

void Component::ChildDrawData()
{
	// ���g�̏���`��
	this->DrawData();

	// �q�R���|�[�l���g�̏���`��
	for (auto comp : childComponents_)comp->ChildDrawData();
}

void Component::ChildSave(json& _saveObj)
{
	// ���g�̏���ۑ�
	this->Save(_saveObj);

	// �q�R���|�[�l���g�̏���ۑ�
	for (auto comp : childComponents_)comp->Save(_saveObj["childComponents_"][comp->GetName()]);
}

void Component::ChildLoad(json& _loadObj)
{
	// ���g�̏���ۑ�
	this->Load(_loadObj);

	// �q�R���|�[�l���g�̏���Ǎ�
	for (auto comp : childComponents_)comp->Load(_loadObj["childComponents_"][comp->GetName()]);
}

bool Component::AddChildComponent(Component* _comp)
{
	// �k���`�F�b�N
	if (_comp == nullptr)
		return false;

	// ������
	_comp->ChildIntialize();

	// �R���|�[�l���g�̃��X�g�ɒǉ�����
	childComponents_.push_back(_comp);
	return true;
}

bool Component::DeleteChildComponent(Component* _comp)
{
	// �C�e���[�^�ɁA"childComponents_"���ŒT�������f�[�^��o�^
	auto it = std::find(childComponents_.begin(), childComponents_.end(), _comp);

	// �C�e���[�^�����X�g�̍Ō�܂œ��B������֐��I��
	if (it == childComponents_.end())
		return false;

	// �C�e���[�^�̃R���|�[�l���g������
	childComponents_.erase(it);
	return true;
}

Component* CreateComponent(ComponentType _type, StageObject* _holder)
{
	Component* comp = nullptr;

	// �^�C�v(���ʔԍ��ɂ��������ăR���|�[�l���g���쐬)
	switch (_type)
	{
	case RotationY:comp = new RotationYComponent(_holder);break;
	}
	return comp;
}
