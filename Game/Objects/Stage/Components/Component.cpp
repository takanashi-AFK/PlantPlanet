#include "Component.h"

#include "../../../../Engine/ImGui/imgui.h"
#include "../StageObject.h"

#include "RotationComponents/RotationComponent.h"
#include "RotationComponents/RotationYComponent.h"
#include "RotationComponents/RotationXComponent.h"
#include "RotationComponents/RotationZComponent.h"
#include "MoveComponents/MoveXComponent.h"


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
	if (ImGui::TreeNode(this->name_.c_str())) {
		
		// ���g�̏���`��
		this->DrawData();

		// �q�R���|�[�l���g�̏���`��
		for (auto comp : childComponents_) comp->ChildDrawData();
		
		ImGui::TreePop();
	}
}

void Component::ChildSave(json& _saveObj)
{
	// ���g�̏���ۑ�
	_saveObj["type_"] = type_;
	this->Save(_saveObj);

	// �q�R���|�[�l���g�̏���ۑ�
	for (auto comp : childComponents_)comp->ChildSave(_saveObj["childComponents_"][comp->GetName()]);
}

void Component::ChildLoad(json& _loadObj)
{
	// ���g�̏���ۑ�
	this->Load(_loadObj);

	// �q�R���|�[�l���g�̏���Ǎ�
	for (auto comp : childComponents_)comp->ChildLoad(_loadObj["childComponents_"][comp->GetName()]);
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
	case Rotation:comp = new RotationComponent(_holder);break;
	case RotationY:comp = new RotationYComponent(_holder); break;
	case RotationX:comp = new RotationXComponent(_holder); break;
	case RotationZ:comp = new RotationZComponent(_holder); break;
	case MoveX:comp = new MoveXComponent(_holder); break;

	default:/* ���̑��R���|�[�l���g��ǉ����鎞�͏�L�̂悤�ɒǉ�*/ break;
	}
	return comp;
}
