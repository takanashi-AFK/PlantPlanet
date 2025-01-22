#include "Component_ReturnGate.h"
#include "../../StageObject.h"
#include "../../../../../Engine/Global.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../../../Engine/SceneManager.h"

Component_ReturnGate::Component_ReturnGate(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, Returngate, _parent),nowFunc_(&Component_ReturnGate::SceneChange)
{
}

Component_ReturnGate::~Component_ReturnGate()
{
}

void Component_ReturnGate::Initialize()
{
}

void Component_ReturnGate::Update()
{
	if (!static_cast<Component_CircleRangeDetector*>(holder_->FindComponent("CircleRange"))->IsContains()) return;

	(this->*nowFunc_)();
}

void Component_ReturnGate::Release()
{
}

void Component_ReturnGate::EmptyWork()
{
}

void Component_ReturnGate::SceneChange()
{
	SceneManager* sm = static_cast<SceneManager*>(holder_->FindObject("SceneManager"));
	sm->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT, 1.0f);

	//連続してシーンチェンジ命令を出さないように関数を変える
	nowFunc_ = &Component_ReturnGate::EmptyWork;
}
