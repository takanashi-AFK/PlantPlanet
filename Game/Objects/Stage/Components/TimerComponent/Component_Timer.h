#pragma once

// �C���N���[�h
#include "../Component.h"

// �쐬��-����

/// <summary>
/// �^�C�}�[���Ǘ�����R���|�[�l���g
/// </summary>
class Component_Timer : public Component
{
private:

	float maxTime_;
	float nowTime_;
	float tempMax_;
	bool isEnd_;
	bool countNow_;
	bool isInfinity_;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Timer(StageObject* _holder);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �J��
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveObj">�ۑ����</param>
	void Save(json& _saveObj) override;

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadObj">�Ǎ����</param>
	void Load(json& _loadObj) override;

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData() override;

	/// <summary>
	/// �J�n
	/// </summary>
	void Start();

	/// <summary>
	/// ��~
	/// </summary>
	void Stop();

	/// <returns>���݂̎���(�b)</returns>
	float GetNowTime();

	/// <returns>�^�C�}�[���I��������</returns>
	bool GetIsEnd();

	/// <param name="_time">�J�E���g���鎞��(�b)</param>
	void SetTime(int _time);

	/// <summary>
	/// �^�C�}�[�����Z�b�g
	/// </summary>
	void Reset();

	/// <param name="_time">�ʒm���Ăق�������(�b)</param>
	/// <returns>�w��̎��ԂɂȂ�����true(��~�͂��Ȃ�)</returns>
	bool IsOnTime(float _time);


	/// <param name="_time">���b���Ƃɒʒm���邩(�b)</param>
	/// <returns>��莞�Ԃ��Ƃ�true</returns>
	bool IsIntervalTime(float _time);

	/// <summary>
	/// �����ɃJ�E���g���邩�ǂ���
	/// </summary>
	void SetInfinity(bool _isInfinity) { isInfinity_ = _isInfinity; }

};

