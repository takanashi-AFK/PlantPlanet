#pragma once
#include "../Component.h"
#include<list>
#include<functional>
#include<tuple>

class Component_RunnigAwayEnemyBehavior : public Component
{
public:

	Component_RunnigAwayEnemyBehavior(string _name, StageObject* _holder, Component* _parent);
	~Component_RunnigAwayEnemyBehavior();
	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;
	void OnCollision(GameObject* _target, Collider* _collider) override;

public:

private:

	void RunawayProcess();
	void DeadProcess();

	void CheckDestination();
	void MoveTo(XMFLOAT3 vec,float amount);
	std::tuple<float,XMFLOAT3> GetLengthAndVectorToDestination();

	bool IsDead();

private:
	std::list<XMFLOAT3> points_;
	std::list<XMFLOAT3>::iterator destinationPointIterator_;

	float moveAmount_;

	std::function<void(void)> nowProcess_;

	bool isFlowerSpawned_;
	string dropFlowerName_;
};

