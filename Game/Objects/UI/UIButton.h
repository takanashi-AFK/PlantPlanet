#pragma once
#include "../../Otheres/UIObject.h"

class UIButton : public UIObject
{
private:

	//UI�̉摜
	int UIButtonPict_;
	//�{�^���������ꂽ���ǂ���
	bool isButtonPushed_;
	//�摜�̃T�C�Y
	XMFLOAT3 size_;
	//�}�E�X�̈ʒu
	XMFLOAT3 mouseCenter_;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_name">UI�I�u�W�F�N�g�̖��O</param>
	/// <param name="_parent">�e�̃I�u�W�F�N�g</param>
	UIButton(string _name, GameObject* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ���
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveUIobj"></param>
	void Save(json& _saveUIobj) override;

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="_loadUIobj"></param>
	void Load(json& _loadUIobj) override;

	//Imgui��ŃC�[�W���O�֐��̎d�g�݂��m�F�������

	/// <summary>
	/// GUI��ŕ\��
	/// </summary>
	void DrawGUI();

	/// <summary>
	/// �摜��ݒ�
	/// </summary>
	void SetImage();

	/// <summary>
	/// �}�E�X�{�^���̒��ɓ����Ă��邩
	/// </summary>
	/// <param name="mousePos">�}�E�X�̌��ݒn</param>
	/// <returns></returns>
	bool MouseInArea(XMFLOAT3 mousePos);

	/// <summary>
	/// �����ꂽ��
	/// </summary>
	/// <param name="pushed">�{�^���������ꂽ��</param>
	void ClickButton(bool pushed);
};

