#pragma once

//�C���N���[�h
#include"../Component.h"

/// <summary>
/// �U���R���|�[�l���g�̊��N���X
/// </summary>
class Component_Attack :public Component
{
protected:
	bool isActive_; 
	int  power_;	//�U����

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	/// <param name="_name">���g�̃R���|�[�l���g��</param>
	/// <param name="_type">���g�̃R���|�[�l���g�^�C�v</param>
	Component_Attack(StageObject* _holder, string _name, ComponentType _type,Component* _parent);

	void Execute() { isActive_ = true; }
	void Stop() { isActive_ = false; }
	void SetPower(int _power) { power_ = _power; }
	int GetPower() { return power_; }
};
