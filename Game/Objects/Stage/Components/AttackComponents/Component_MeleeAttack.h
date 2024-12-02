#pragma once
#include "Component_Attack.h"
#include "../../../../../Engine/Collider/BoxCollider.h"

class Component_MeleeAttack : public Component_Attack
{
private:
	XMVECTOR forward_; 
	XMVECTOR prevFrontVec_;
	bool isHit_;

	bool isFirstMove_;
	XMFLOAT3 startPosition_;

public:
	Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent);
	void Initialize() override;
	void Update() override;
	void Release();
	void DrawData() override;

	void SetForward(XMVECTOR _forward) { forward_ = _forward; }
};