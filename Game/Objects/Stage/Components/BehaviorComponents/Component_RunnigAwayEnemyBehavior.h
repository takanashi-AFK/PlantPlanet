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
};

