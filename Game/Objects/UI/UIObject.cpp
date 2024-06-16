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
	_saveObj["objectName_"] = objectName_;

	// ���g�̕ό`�s�����ۑ�
	_saveObj["position_"] = { REFERENCE_XMFLOAT3(transform_.position_) };
	_saveObj["rotate_"] = { REFERENCE_XMFLOAT3(transform_.rotate_) };
	_saveObj["scale_"] = { REFERENCE_XMFLOAT3(transform_.scale_) };

	// ���g�̌ŗL����ۑ�
	this->Save(_saveObj);
}

void UIObject::ChildLoad(json& _loadObj)
{
	// �^�C�v��Ǎ�
	if(_loadObj.contains("type_"))type_ = (UIType)_loadObj["type_"].get<int>();

	// �I�u�W�F�N�g����Ǎ�
	if (_loadObj.contains("objectName_"))objectName_ = _loadObj["objectName_"];

	// �ό`�s�����Ǎ�
	if (_loadObj.contains("position_"))transform_.position_ = { _loadObj["position_"][0].get<float>(),_loadObj["position_"][1].get<float>(), _loadObj["position_"][2].get<float>() };
	if (_loadObj.contains("rotate_"))transform_.rotate_ = { _loadObj["rotate_"][0].get<float>(),_loadObj["rotate_"][1].get<float>(), _loadObj["rotate_"][2].get<float>() };
	if (_loadObj.contains("scale_"))transform_.scale_ = { _loadObj["scale_"][0].get<float>(),_loadObj["scale_"][1].get<float>(), _loadObj["scale_"][2].get<float>() };

	// �ŗL����Ǎ�
	this->Load(_loadObj);
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
