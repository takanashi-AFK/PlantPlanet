#include "UIObject.h"

#include "UIButton.h"
#include "../../../Engine/Global.h"

UIObject::UIObject(string _name, UIType _type, GameObject* parent)
	: GameObject(parent, _name), isEnable_(true), type_(_type)
{
}

void UIObject::ChildSave(json& _saveObj)
{
	// �^�C�v��ۑ�
	_saveObj["type_"] = type_;
	
	// �I�u�W�F�N�g����ۑ�
	_saveObj["name_"] = objectName_;

	// ���g�̕ό`�s�����ۑ�
	_saveObj["position_"] = { REFERENCE_XMFLOAT3(transform_.position_) };
	_saveObj["rotate_"] = { REFERENCE_XMFLOAT3(transform_.rotate_) };
	_saveObj["scale_"] = { REFERENCE_XMFLOAT3(transform_.scale_) };

	// ���g�̌ŗL����ۑ�
	this->Save(_saveObj);
}

void UIObject::ChildLoad(json& _loadObj)
{
}

UIObject* CreateUIObject(string _name, UIType _type, GameObject* _parent)
{
	// �C���X�^���X�𐶐�����
	UIObject* obj = nullptr;
	switch (_type)
	{
	case UI_BUTTON:obj = new UIButton(_name, _parent); break;
	case UI_IMAGE:break;
	case UI_TEXT:break;
	default:obj = new UIObject(_name, _type, _parent);break;
	}

	// �C���X�^���X�������ł��Ȃ������ꍇ��nullptr��Ԃ�
	if (obj == nullptr)return nullptr;

	// �e�I�u�W�F�N�g�̎q�I�u�W�F�N�g�Ƃ��ēo�^����
	if (_parent != nullptr)_parent->PushBackChild(obj);
	
	// ���������s��
	obj->Initialize();

	// ���������I�u�W�F�N�g��Ԃ�
	return obj;
}

string GetUITypeString(UIType _type)
{
	switch (_type)
	{
	case UI_BUTTON:return "BUTTON";
	case UI_IMAGE:return "IMAGE";
	case UI_TEXT:return "TEXT";
	default:return "UNKNOWN";
	}
}
