#include "Component_BreakableWall.h"
#include "../../../Engine/Global.h"
#include "../DetectorComponents/Component_RectangleDetector.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "../../../Engine/ImGui/imgui.h"
namespace 
{
	float interactiveRange_x = 4.f;
	float interactiveRange_y = 2.f;
}

Component_BreakableWall::Component_BreakableWall(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, BreakableWall, _parent)
{
}

void Component_BreakableWall::Initialize()
{
	if (FindChildComponent("RectangleDetector") == false)AddChildComponent(CreateComponent("RectangleDetector", RectangleDetector, holder_, this));
	// 移動コンポーネントの取得 & 有無の確認
	Component_RectangleDetector* detector = (Component_RectangleDetector*)(GetChildComponent("RectangleDetector"));

	Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
	if (pStage == nullptr)return;

	for (StageObject* object : pStage->GetStageObjects()) {
		// Playerの属性をもったオブジェクトを取得
		if (object->GetObjectType() == StageObject::TYPE_PLAYER) {
			playerObjects_ = object;
		}
	}
	holder_->SetObjectType(StageObject::TYPE_WALL);
}

void Component_BreakableWall::Update()
{
	Component_RectangleDetector* detector = (Component_RectangleDetector*)(GetChildComponent("RectangleDetector"));
	if (detector == nullptr)return;

	XMFLOAT2 length = detector->GetLength();
	
	detector->SetLength(length.x,length.y);

	detector->SetTarget(playerObjects_);

	if (detector->IsContains() == true)isPlayerContains = true;
	else isPlayerContains = false;
}

void Component_BreakableWall::Release()
{
}

void Component_BreakableWall::OnCollision(GameObject* _target, Collider* _collider)
{
}

void Component_BreakableWall::Save(json& _saveObj)
{
}

void Component_BreakableWall::Load(json& _loadObj)
{
}

void Component_BreakableWall::DrawData()
{
}
