#include "BreakableWall.h"
#include "../Stage/Components/DetectorComponents/Component_RectangleDetector.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "../Stage/Components/Component.h"

BreakableWall::BreakableWall(GameObject* _parent)
	:StageObject("BreakableWall", "Models/DebugCollision/BoxCollider.fbx", _parent)
{
}

void BreakableWall::Initialize()
{
	// ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);


}

void BreakableWall::Update()
{
}

void BreakableWall::Draw()
{
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

void BreakableWall::OnCollision(GameObject* _target, Collider* _collider)
{
}

void BreakableWall::Save(json& _saveObj)
{
}

void BreakableWall::Load(json& _loadObj)
{
}

void BreakableWall::DrawData()
{
}
