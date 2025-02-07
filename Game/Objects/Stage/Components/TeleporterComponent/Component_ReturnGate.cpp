#include "Component_ReturnGate.h"
#include "../../StageObject.h"
#include "../../../../../Engine/Global.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../../../Engine/SceneManager.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../../UI/UIImage.h"
#include "../../../UI/UIPanel.h"

Component_ReturnGate::Component_ReturnGate(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, ReturnGate, _parent),workFunc_(&Component_ReturnGate::SceneChange),updateFunc_(&Component_ReturnGate::EmptyWork)
	,isUsable_(false),isVisible_(false),state_(Component_ReturnGate::State::IDLE),fade_(.0f)
{
}

Component_ReturnGate::~Component_ReturnGate()
{
}

void Component_ReturnGate::Initialize()
{
	//貫通するように
	holder_->SetIsColliding(false);
}

void Component_ReturnGate::Update()
{
	//帰還ゲートのオブジェクトを描画するかどうか
	holder_->SetVisible(this->isVisible_);

	(this->*updateFunc_)();
}

void Component_ReturnGate::Release()
{
}

void Component_ReturnGate::Work()
{
	//isUsableがtrueでなければ使えない
	if (!isUsable_) return;
	//最初の一回目のみ処理あり
	(this->*workFunc_)();
}

void Component_ReturnGate::SetVisible(bool b)
{
	isVisible_ = b;
}

void Component_ReturnGate::SetUsable(bool b)
{
	isUsable_ = b;
}

void Component_ReturnGate::SetState(State state)
{
	BranchUpdateFunc(state);
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

Component_ReturnGate::State Component_ReturnGate::GetState() const
{
	return state_;
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
	//処理なし
}

void Component_ReturnGate::SceneChange()
{
	SceneManager* sm = static_cast<SceneManager*>(holder_->FindObject("SceneManager"));
	sm->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT, 1.0f);

	//連続してシーンチェンジ命令を出さないように関数を変える
	workFunc_ = &Component_ReturnGate::EmptyWork;
}

void Component_ReturnGate::BranchUpdateFunc(State state)
{
	if (state_ == state)	return;

	state_ = state;

	switch (state_)
	{
	case State::IDLE:updateFunc_ = &Component_ReturnGate::EmptyWork; return;

	case State::MESSAGE:updateFunc_ = &Component_ReturnGate::FadeInPopUp; return;

	default:return;
	}
}

void Component_ReturnGate::FadeInPopUp()
{
	EasingUI("ReturnGate_PopUp_Text");
	EasingUI("ReturnGate_PopUp_BackGround");

	fade_ += 0.05f;

	if (fade_ < 1.0f)return;
	
	fade_ = 1.0f;
	updateFunc_ = &Component_ReturnGate::KeepPopUp;
}

void Component_ReturnGate::KeepPopUp()
{
	EasingUI("ReturnGate_PopUp_Text");
	EasingUI("ReturnGate_PopUp_BackGround");

	fade_ = 1.0f;

	++keepingTime_;
	if (keepingTime_ < KeepPopUpTime)return;

	keepingTime_ = 0.f;
	updateFunc_ = &Component_ReturnGate::FadeOutPopUp;
}

void Component_ReturnGate::FadeOutPopUp()
{
	EasingUI("ReturnGate_PopUp_Text");
	EasingUI("ReturnGate_PopUp_BackGround");

	fade_ -= 0.05f;

	if (fade_ > .0f)return;

	fade_ = .0f;
	
	UIPanel::GetInstance()->GetUIObject("ReturnGate_PopUp_Text")->SetVisible(false);
	UIPanel::GetInstance()->GetUIObject("ReturnGate_PopUp_BackGround")->SetVisible(false);

	BranchUpdateFunc(State::IDLE);
}

void Component_ReturnGate::EasingUI(string uiName)
{
	UIObject* ui = UIPanel::GetInstance()->GetUIObject(uiName);
	if (!ui)return;
	ui->SetVisible(true);
	ui->SetFade(fade_);
}
