#pragma once
#include "Component_Gauge.h"
class Component_Timer;
class Component_StaminaGauge :public Component_Gauge
{
	private:
	float stamina_;	//�O��̃X�^�~�i
	float shaderChangeTime_;	//�V�F�[�_�[��ύX���鎞��
	bool isLock_;	//���b�N�t���O
	bool isUseStamina_;	//�X�^�~�i���g�p������
	Component_Timer* timer;

	public:
		/// <summary> �R���X�g���N�^ </summary>
		Component_StaminaGauge(string _name, StageObject* _holder, Component* _parent);

		/// <summary> ������ </summary>
		void Initialize() override;

		/// <summary> �X�V </summary>
		void Update() override;

		/// <summary> ��� </summary>
		void Release() override;

		/// <summary>
		/// �X�^�~�i�������
		/// </summary>
		/// <param name="_useValue">���P�ʏ���邩</param>
		/// <returns>����ł�����true </returns>
		void UseStamina(float _useValue);

		bool CanUseStamina(float _useValue);

		/// <summary> �X�^�~�i���񕜂��� </summary>
		void RecoverStamina(float _recoverValue);

		/// <summary> ���b�N </summary>
		void RecoverLock() { isLock_ = true; }

		/// <summary> �A�����b�N </summary>
		void RecoverUnlock() { isLock_ = false; }


};

