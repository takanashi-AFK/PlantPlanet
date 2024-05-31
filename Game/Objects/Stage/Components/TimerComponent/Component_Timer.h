#pragma once

// �C���N���[�h
#include "../Component.h"

/// <summary>
/// �^�C�}�[���Ǘ�����R���|�[�l���g
/// </summary>
class Component_Timer : public Component
{
private:
	float maxTime_;		// �ő厞��
	float nowTime_;		// ���݂̎���
	float tempMax_;		// �ꎞ�I�ȍő厞��
	bool isEnd_;		// �I��������
	bool isCountNow_;	// �J�E���g����
	bool isInfinity_;	// �����ɃJ�E���g���邩

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Timer(string _name,StageObject* _holder, Component* _parent);

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
	/// <param name="_saveObj">�ۑ��f�[�^</param>
	void Save(json& _saveObj) override;

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadObj">�Ǎ��f�[�^</param>
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

	/// <returns>
	/// ���݂̎���(�b)
	/// </returns>
	float GetNowTime();

	/// <returns>
	/// �^�C�}�[���I��������
	/// </returns>
	bool GetIsEnd();

	/// <param name="_time">
	/// �J�E���g���鎞��(�b)
	/// </param>
	void SetTime(int _time);

	/// <summary>
	/// �^�C�}�[�����Z�b�g
	/// </summary>
	void Reset();

	/// <returns>
	/// �w��̎��ԂɂȂ�����true(��~�͂��Ȃ�)
	/// </returns>
	/// <param name="_time">�ʒm���Ăق�������(�b)</param>
	bool IsOnTime(float _time);

	/// <returns>
	/// ��莞�Ԃ��Ƃ�true
	/// </returns>
	/// <param name="_time">���b���Ƃɒʒm���邩(�b)</param>
	bool IsIntervalTime(float _time);

	/// <summary>
	/// �����ɃJ�E���g���邩�ǂ���
	/// </summary>
	/// <param name="_isInfinity">�����ɂ��邩</param>
	void SetInfinity(bool _isInfinity) { isInfinity_ = _isInfinity; }
};

