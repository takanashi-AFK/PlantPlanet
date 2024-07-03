#pragma once
//�C���N���[�h
#include"../Component.h"


/// <summary>
/// �̗͂̑������Ǘ�����R���|�[�l���g
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
	Component_Gauge(string _name, StageObject* _holder, ComponentType _type, Component* _parent);

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
	/// �Q�[�W�̒l���擾
	/// </summary>
	/// <returns></returns>
	float GetGauge() const;

	/// <summary>
	/// �Q�[�W�̒l��ݒ�
	/// </summary>
	/// <param name="_newHP">�^����ꂽHP</param>
	void SetGauge(float _newGauge);

	/// <returns>�Q�[�W�̍ő�l</returns>
	float GetMax() const;

	/// <summary>
	/// �Q�[�W�̍ő�l��ݒ�
	/// </summary>
	void SetMax(float _newMaxGauge);

	/// <summary>
	/// �Q�[�W�̃��Z�b�g(�ő�l�ɖ߂�)
	/// </summary>
	void Reset();
};
