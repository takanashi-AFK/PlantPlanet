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
	
};
