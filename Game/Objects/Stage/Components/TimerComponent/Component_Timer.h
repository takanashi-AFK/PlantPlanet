#pragma once

// �C���N���[�h
#include "../Component.h"

// �쐬��-����
class Component_Timer : public Component
{
private:

	float time_;
	float nowTime_;
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
	/// ImGui�\��
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

	/// <returns>���݂̎���</returns>
	float GetNowTime();

	/// <summary>
	/// �^�C�}�[���I��������
	/// </summary>	/// <returns>�I��������true</returns>
	bool GetEnd();

	/// <param name="_time">�J�E���g����b��</param>
	void SetTime(int _time);

	/// <summary>
	/// �^�C�}�[�����Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// �w��̎��ԂɂȂ�����ʒm
	/// </summary>
	/// <param name="_time"></param>
	/// <returns></returns>
	bool IsOnTime(float _time);

	/// <summary>
	/// ��莞�Ԃ��Ƃɒʒm
	/// </summary>
	/// <param name="_time"></param>
	/// <returns></returns>
	float IsIntervalTime(float _time);

};

