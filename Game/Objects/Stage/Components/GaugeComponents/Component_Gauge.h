#pragma once
//�C���N���[�h
#include"../Component.h"


/// <summary>
/// �Q�[�W���Ǘ�����R���|�[�l���g
/// </summary>
class Component_Gauge :public Component
{
private:

	float maxGauge_; //�Q�[�W�̏��
	float nowGauge_;  //���݂̃Q�[�W

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Gauge(StageObject* _holder, string _name, ComponentType _type, Component* _parent);
	/// <summary>
	/// HP�̒l���擾
	/// </summary>
	/// <returns></returns>
	float GetNow() const { return nowGauge_; }

	/// <summary>
	/// HP�̒l��ݒ�
	/// </summary>
	/// <param name="_newHP">�^����ꂽHP</param>
	void SetNow(float _newNow) { nowGauge_ = _newNow; }

	/// <returns>�̗͂̍ő�l</returns>
	float GetMax() const { return maxGauge_; }

	/// <summary>
	/// �̗͂̍ő�l��ݒ�
	/// </summary>
	void SetMax(float _newMax) { maxGauge_ = _newMax; }

};
