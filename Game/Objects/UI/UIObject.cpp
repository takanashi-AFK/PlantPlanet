#include "UIObject.h"

#include "UIButton.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"
#include "UIImage.h"
#include "UIPanel.h"

UIObject::UIObject(string _name, UIType _type, GameObject* parent, int _layerNum)
	: GameObject(parent, _name), isEnable_(true), type_(_type), layerNumber_(_layerNum)
{
}

void UIObject::ChildSave(json& _saveObj)
{
	// �^�C�v��ۑ�
	_saveObj["type_"] = type_;
	
	// �I�u�W�F�N�g����ۑ�
	_saveObj["objectName_"] = objectName_;

	// ���C���[�ԍ���ۑ�
	_saveObj["layerNumber_"] = layerNumber_;

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

	// ���C���[�ԍ���Ǎ�
	if (_loadObj.contains("layerNumber_"))layerNumber_ = _loadObj["layerNumber_"].get<int>();

	// �ό`�s�����Ǎ�
	if (_loadObj.contains("position_"))transform_.position_ = { _loadObj["position_"][0].get<float>(),_loadObj["position_"][1].get<float>(), _loadObj["position_"][2].get<float>() };
	if (_loadObj.contains("rotate_"))transform_.rotate_ = { _loadObj["rotate_"][0].get<float>(),_loadObj["rotate_"][1].get<float>(), _loadObj["rotate_"][2].get<float>() };
	if (_loadObj.contains("scale_"))transform_.scale_ = { _loadObj["scale_"][0].get<float>(),_loadObj["scale_"][1].get<float>(), _loadObj["scale_"][2].get<float>() };

	// �ŗL����Ǎ�
	this->Load(_loadObj);
}

void UIObject::ChildDrawData()
{
	// ��������������������������������������������
	// �I�u�W�F�N�g����\��
	// ��������������������������������������������
	ImGui::Text(this->objectName_.c_str());
	ImGui::SameLine();

	// ��������������������������������������������
	// �I�u�W�F�N�g�̍폜�{�^��
	// ��������������������������������������������
	if (ImGui::SmallButton("delete"))((UIPanel*)FindObject("UIPanel"))->DeleteUIObject(this);
	ImGui::Separator();

	// ��������������������������������������������
	// �I�u�W�F�N�g�̖��O��ύX
	// ��������������������������������������������
	if (ImGui::TreeNode("name")) {
		ImGui::Text("Current name : %s", this->objectName_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New name...", buffer, IM_ARRAYSIZE(buffer)))
			this->objectName_ = buffer;
		ImGui::TreePop();
	}

	// ��������������������������������������������
	// ���g�̕ό`����`��
	// ��������������������������������������������
	if (ImGui::TreeNode("transform_")) {
		ImGui::DragFloat3("position_", &transform_.position_.x, 0.1f);
		ImGui::DragFloat3("rotate_", &transform_.rotate_.x, 1.f, -360.f, 360.f);
		ImGui::DragFloat3("scale_", &transform_.scale_.x, 0.1f, 0.f, LONG_MAX);
		ImGui::TreePop();
	}


	// ��������������������������������������������
	// ���C���[�ԍ���`��
	// ��������������������������������������������
	if (ImGui::TreeNode("LayerNumber"))
	{
		ImGui::Text("Image Layer Number: %d", GetLayerNumber());
		ImGui::InputInt("Layer Number", &layerNumber_);
		//layerNumbert��0�ȉ��ꍇ��1�ɂ���
		if (layerNumber_ <= 0)layerNumber_ = 1;

		ImGui::TreePop();
	}
	// �ŗL����`��
	this->DrawData();
}

bool UIObject::CompareLayerNumber(UIObject* _object1, UIObject* _object2)
{
	return _object1->GetLayerNumber() < _object2->GetLayerNumber();
}

void UIObject::SortChildren()
{
	std::sort(children_.begin(), children_.end(), UIObject::CompareLayerNumber);
	for (auto child : children_) {
		child->SortChildren();  // �ċA�I�Ɏq�I�u�W�F�N�g���\�[�g
	}

}

UIObject* CreateUIObject(string _name, UIType _type, GameObject* _parent, int _layerNum)
{
	// �C���X�^���X�𐶐�����
	UIObject* obj = nullptr;
	switch (_type)
	{
		case UI_BUTTON:obj = new UIButton(_name, _parent,_layerNum); break;
		case UI_IMAGE:obj = new UIImage(_name, _parent,_layerNum); break;
		case UI_TEXT:break;
		default:obj = new UIObject(_name, _type, _parent,_layerNum);break;
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
