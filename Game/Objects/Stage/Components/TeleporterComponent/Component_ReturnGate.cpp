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
	//貫通するように
	holder_->SetIsColliding(false);
}

void Component_ReturnGate::Update()
{
	//帰還ゲートのオブジェクトを描画するかどうか
	holder_->SetVisible(this->isVisible_);
}

void Component_ReturnGate::Release()
{
}

void Component_ReturnGate::Work()
{
	//isUsableがtrueでなければ使えない
	if (!isUsable_) return;
	//最初の一回目のみ処理あり
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
	//処理なし
}

void Component_ReturnGate::SceneChange()
{
	SceneManager* sm = static_cast<SceneManager*>(holder_->FindObject("SceneManager"));
	sm->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT, 1.0f);

	//連続してシーンチェンジ命令を出さないように関数を変える
	nowFunc_ = &Component_ReturnGate::EmptyWork;
}
