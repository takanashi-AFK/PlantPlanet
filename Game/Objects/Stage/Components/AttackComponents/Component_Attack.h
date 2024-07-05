#pragma once

//�C���N���[�h
#include"../Component.h"

/// <summary>
/// �U���R���|�[�l���g�̊��N���X
/// </summary>
class Component_Attack :public Component
{
protected:
	int  power_;	//�U����

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	/// <param name="_name">���g�̃R���|�[�l���g��</param>
	/// <param name="_type">���g�̃R���|�[�l���g�^�C�v</param>
	Component_Attack(StageObject* _holder, string _name, ComponentType _type,Component* _parent);

	/// <summary>
	/// �U���͂�ݒ�
	/// </summary>
	/// <param name="_power"></param>
	void SetPower(int _power) { power_ = _power; }

	/// <summary>
	/// �U���͂��擾
	/// </summary>
	/// <returns></returns>
	int GetPower() { return power_; }
};
