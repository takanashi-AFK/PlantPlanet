#include "UIObject.h"

UIObject::UIObject(string _name, UIType _type, GameObject* parent)
	: GameObject(parent, _name), isEnable_(true), type_(_type)
{
}

UIObject* CreateUIObject(string _name, UIType _type, GameObject* _parent)
{
	// �C���X�^���X�𐶐�����
	UIObject* obj = nullptr;
	switch (_type)
	{
	case UI_BUTTON:break;
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
