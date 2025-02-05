#pragma once
#include "../Component.h"
#include "../../../../../EffekseeLib/EffekseerVFX.h"

class StageObject;
class Component_MeleeEnemyBehavior :
	public Component
{
private:
	enum EnemyState
	{
		E_IDLE = 1,
		E_MOVE,
		E_ATTACK,
		E_DEAD,
		E_MAX
	}enemyState_;

	StageObject* playerObjects_;
	float speed_;
	float deltaDistance_;
	float power_;
	float attackCoolTime_;
	float attackDelayTime_;
	bool isAttacked_;
	bool isFlowerSpawned_;

	string dropFlowerName_;
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform;

public:
	Component_MeleeEnemyBehavior(string _name, StageObject* _holder, Component* _parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;
	void DrawData() override;


	void Idle();
	void Move();
	void Attack();
	void Dead();

	void SetState(EnemyState _state) { enemyState_ = _state; }

};

