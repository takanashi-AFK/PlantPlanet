#pragma once
#include "../Component.h"
#include<list>
#include<functional>
#include<tuple>

class Component_RunningAwayEnemyBehavior : public Component
{
public:

	Component_RunningAwayEnemyBehavior(string _name, StageObject* _holder, Component* _parent);
	~Component_RunningAwayEnemyBehavior();
	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;
	void OnCollision(GameObject* _target, Collider* _collider) override;

	enum class STATE
	{
		RUNNING,
		DEATH,

		AMOUNT
	};

public:

	STATE GetState() { return state_; }
	void SetState(STATE state) { state_ = state; }

private:

	//��������Ă���Ƃ��̋���
	void RunawayProcess();

	//���S���̋���
	void DeadProcess();

	//�ړI�n�ƌ��ݒn���������ǂ����m�F�B���Ȃ��ł���Ύ��̖ړI�n�̐ݒ�
	void CheckDestination();

	//�w�肳�ꂽ�����Ɨʂ����ړ�
	void MoveTo(XMFLOAT3 vec,float amount);

	//�����ƕ�����Ԃ�
	std::tuple<float,XMFLOAT3> GetLengthAndVectorToDestination();

	//HP��0�ȉ����ǂ���
	bool IsDead();

	void ExchangeRotate(XMFLOAT3 vec);

private:
	std::list<XMFLOAT3> points_;
	std::list<XMFLOAT3>::iterator destinationPointIterator_;

	float moveAmount_;

	std::function<void(void)> nowProcess_;

	bool isFlowerSpawned_;
	string dropFlowerName_;

	STATE state_;
};

