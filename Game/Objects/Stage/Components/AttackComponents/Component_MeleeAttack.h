#pragma once
#include "Component_Attack.h"
#include "../../../../../Engine/Collider/BoxCollider.h"
#include <DirectXMath.h>
using namespace DirectX;
class Component_FanRangeDetector;

class Component_MeleeAttack : public Component_Attack
{
private:
	
	enum Sequence
	{
		E_STEPFORWORD,
		E_SLASH,
		E_MAX
	}sequence_;

	XMVECTOR forward_; 
	float distance_;
	float speed_;
	bool isHit_;
	string easingType_;			// イージングの種類
	float rate_;				// イージングの割合

	bool isFirstMove_;	
	XMFLOAT3 startPosition_;
	Component_FanRangeDetector* attackRange_;

	float range_;
	float angle_;
public:
	Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
	void DrawData() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void SetForward(XMVECTOR _forward) { forward_ = _forward; }

	void StepForward();


	void Slash();

	void SetSequence(Sequence _sequence) { sequence_ = _sequence; }
};