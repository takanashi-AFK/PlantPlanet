#include "Component_ReturnGate.h"
#include "../../StageObject.h"
#include "../../../../../Engine/Global.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../../../Engine/SceneManager.h"
#include "../../../../../Engine/ImGui/imgui.h"

Component_ReturnGate::Component_ReturnGate(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, ReturnGate, _parent),nowFunc_(&Component_ReturnGate::SceneChange),isUsable_(false),isVisible_(false)
{
}

Component_ReturnGate::~Component_ReturnGate()
{
}

void Component_ReturnGate::Initialize()
{
	//�ђʂ���悤��
	holder_->SetIsColliding(false);
}

void Component_ReturnGate::Update()
{
	//�A�҃Q�[�g�̃I�u�W�F�N�g��`�悷�邩�ǂ���
	holder_->SetVisible(this->isVisible_);
}

void Component_ReturnGate::Release()
{
}

void Component_ReturnGate::Work()
{
	//isUsable��true�łȂ���Ύg���Ȃ�
	if (!isUsable_) return;
	//�ŏ��̈��ڂ̂ݏ�������
	(this->*nowFunc_)();
}

void Component_ReturnGate::SetVisible(bool b)
{
	isVisible_ = b;
}

void Component_ReturnGate::SetUsable(bool b)
{
	isUsable_ = b;
}

bool Component_ReturnGate::IsVisible()
{
	return isVisible_;
}

bool Component_ReturnGate::IsUsable()
{
	return isUsable_;
}

void Component_ReturnGate::DrawData()
{
	bool isUsable = isUsable_;
	bool isVisible = isVisible_;

	ImGui::Checkbox("Is Usable?  ", &isUsable);
	ImGui::Checkbox("Is Visible? ", &isVisible);

	isUsable_ = isUsable;
	isVisible_ = isVisible;
}

void Component_ReturnGate::Save(json& _saveObj)
{
	_saveObj["IsVisible"] = (bool)isVisible_;
	_saveObj["IsUsable"] = (bool)isUsable_;
}

void Component_ReturnGate::Load(json& _loadObj)
{
	if(_loadObj.contains("IsVisible")) isVisible_ = _loadObj["IsVisible"];
	if(_loadObj.contains("IsUsable"))isUsable_ = _loadObj["IsUsable"];
}

void Component_ReturnGate::EmptyWork()
{
	//�����Ȃ�
}

void Component_ReturnGate::SceneChange()
{
	SceneManager* sm = static_cast<SceneManager*>(holder_->FindObject("SceneManager"));
	sm->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT, 1.0f);

	//�A�����ăV�[���`�F���W���߂��o���Ȃ��悤�Ɋ֐���ς���
	nowFunc_ = &Component_ReturnGate::EmptyWork;
}
