#pragma once
#include "../Component.h"
#include<functional>

class Component_RangeEnemyBehavior : public Component
{
private:
	StageObject* target_;
	string targetName_;
	bool isFire_;
	int shotAmount_;
	int shotInterval_;

public:
	Component_RangeEnemyBehavior(string _name, StageObject* _holder, Component* _parent);
	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;
	void OnCollision(GameObject* _target, Collider* _collider) override;

	enum class STATE
	{
		SLEEP,
		COMBAT,
		DEAD
	};

protected:
	
	bool isSerchTargetSuccess();
	bool isDetectTarget();
	bool isDead();

	float GetLengthTo(XMFLOAT3 tgt);

	void WalkTo(XMFLOAT3 dir);
	void Attack();
	void FaceToTarget();

protected:

	std::function<void(void)> currentProcess_;

	void SleepProcess();
	void CombatProcess();
	void DeadProcess();
};
