#pragma once
#include "../Component.h"
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

};

