#pragma once
#include "../UI/UIObject.h"

class UIButton : public UIObject
{
private:

	int UIButtonPict_;
	XMFLOAT3 size_;//�摜�̑傫��

	struct Rect {
		float left;
		float top;
		float right;
		float bottom;
	};

public:
	/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_parent"></param>
	UIButton(GameObject* _parent);

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
	/// <param name="_saveUIobj">�ۑ��������{�^���̏��</param>
	void Save(json& _saveUIobj) override;

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadUIobj">�Ǎ��������{�^���̏��</param>
	void Load(json& _loadUIobj) override;

	//Imgui��ł��[���񂮊֐��̎d�g�݌��邽�߂̂��
	void DrawGui();

	//�}�E�X���{�^���̒��ɓ����Ă邩�ǂ���
	bool MouseInArea(XMFLOAT3 mousePos);

	//�����ꂽ���ǂ���
	bool ClickButton();

	//�摜�̍��W�h�ɕϊ�
	void ConvertToImageCoordinates(XMFLOAT3& _position);

};

