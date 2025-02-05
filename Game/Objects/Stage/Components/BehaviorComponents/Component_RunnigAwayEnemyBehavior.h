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

	//逃げ回っているときの挙動
	void RunawayProcess();

	//死亡時の挙動
	void DeadProcess();

	//目的地と現在地が同じかどうか確認。おなじであれば次の目的地の設定
	void CheckDestination();

	//指定された向きと量だけ移動
	void MoveTo(XMFLOAT3 vec,float amount);

	//距離と方向を返す
	std::tuple<float,XMFLOAT3> GetLengthAndVectorToDestination();

	//HPが0以下かどうか
	bool IsDead();

	void ExchangeRotate(XMFLOAT3 vec);

private:
	std::list<XMFLOAT3> points_;
	std::list<XMFLOAT3>::iterator destinationPointIterator_;

	float moveAmount_;

	std::function<void(void)> nowProcess_;

	bool isFlowerSpawned_;
	string dropFlowerName_;
};

