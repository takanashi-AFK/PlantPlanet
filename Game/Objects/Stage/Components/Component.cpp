#include "Component.h"

// �C���N���[�h
#include "../../../../Engine/ImGui/imgui.h"
#include "../StageObject.h"

// �쐬�����R���|�[�l���g�̃C���N���[�h
#include "RotationComponents/Component_Rotation.h"
#include "RotationComponents/Component_RotationY.h"
#include "RotationComponents/Component_RotationX.h"
#include "RotationComponents/Component_RotationZ.h"
#include "MoveComponents/Component_MoveX.h"
#include "HealthManagerComponents/Component_HealthManager.h"

Component::Component(StageObject* _holder, string _name,ComponentType _type)
    :holder_(_holder), name_(_name),type_(_type),childComponents_()
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
		if (childComponents_.empty() == false) {
			if (ImGui::TreeNode("childComponents_")) {
				// �q�R���|�[�l���g�̏���`��
				for (auto comp : childComponents_) comp->ChildDrawData();

				ImGui::TreePop();
			}
		}
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

bool Component::FindChildComponent(ComponentType _type)
{
	for (auto comp : childComponents_) if (comp->type_ == _type)return true;
	return false;
}

Component* CreateComponent(ComponentType _type, StageObject* _holder)
{
	Component* comp = nullptr;

	// �^�C�v(���ʔԍ��ɂ��������ăR���|�[�l���g���쐬)
	switch (_type)
	{
	case Rotation:comp = new Component_Rotation(_holder);break;
	case RotationY:comp = new Component_RotationY(_holder); break;
	case RotationX:comp = new Component_RotationX(_holder); break;
	case RotationZ:comp = new Component_RotationZ(_holder); break;
	case MoveX:comp = new Component_MoveX(_holder); break;
	case HealthManager:comp = new Component_HealthManager(_holder); break;

	default:/* ���̑��R���|�[�l���g��ǉ����鎞�͏�L�̂悤�ɒǉ�*/ break;
	}
	return comp;
}
