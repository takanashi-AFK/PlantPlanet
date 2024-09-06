#pragma once
//�C���N���[�h
#include"Component_Gauge.h"

/// <summary>
/// �̗͂̑������Ǘ�����R���|�[�l���g
/// </summary>
class Component_HealthGauge :public Component_Gauge
{
private:
	float prev_;	//�O��̗̑�
	float shaderChangeTime_;	//�V�F�[�_�[��ύX���鎞��
public:
	Component_HealthGauge(string _name, StageObject* _holder, Component* _parent);

	void Initialize() override;
	void Update() override;
	void Release() override;

	void TakeDamage(float _damageValue);
	void Heal(float _healValue);
	bool IsDead() const { return now_ <= 0; }
};