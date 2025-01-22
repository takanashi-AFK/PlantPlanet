#include "ReturnGate.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "../Stage/Components/DetectorComponents/Component_CircleRangeDetector.h"
#include "../Stage/Stage.h"

ReturnGate::ReturnGate(GameObject* _parent)
	:StageObject("ReturnGate", "Models/DebugCollision/BoxCollider.fbx", _parent)
{
}

ReturnGate::~ReturnGate()
{
}

void ReturnGate::Initialize()
{
	transform_.position_ = { 0,0,0 };

	AddComponent(CreateComponent("CircleRange", ComponentType::CircleRangeDetector, this));
	AddComponent(CreateComponent("ReturnGate", ComponentType::Returngate, this));
	
	modelHandle_ = Model::Load("Models/DebugCollision/BoxCollider.fbx");

	auto* detector  = static_cast<Component_CircleRangeDetector*>(FindComponent("CircleRange"));

	detector->SetRadius(1.f);
	
	detector->SetTarget(static_cast<Stage*>(GetParent())->GetStageObject("00_player"));

	SetIsColliding(false);
	Enter();
}

void ReturnGate::Update()
{
	//�蓮�ł��Ȃ���component��update�����s����Ȃ�
	for (auto& comp : myComponents_) {
		comp->Update();
	}
}

void ReturnGate::Draw()
{
	SetShader(Direct3D::SHADER_RAREFLOWER);
	Model::SetTransform(modelHandle_,transform_);
	Model::Draw(modelHandle_);
}

void ReturnGate::Release()
{
}

class ReturnGate* CreateReturnGate(GameObject* parent, string name)
{
	ReturnGate* gate = new ReturnGate(parent);

	if (parent)
	{
		parent->PushBackChild(gate);
	}

	gate->Initialize();

	return gate;
}
