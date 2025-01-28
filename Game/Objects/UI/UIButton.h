#pragma once
#include "UIObject.h"
#include"../../../Engine/DirectX/Direct3D.h"

class UIButton : public UIObject
{
private:
	string imageFilePath_; // �摜�t�@�C���p�X
	int imageHandle_;		// �摜�n���h��
	Direct3D::SHADER_TYPE shaderType_;

	string defaultImageFilePath_; // �f�t�H���g�摜�t�@�C���p�X
	int defaultImageHandle_;		// �f�t�H���g�摜�n���h��

	string selectImageFilePath_; // �I���摜�t�@�C���p�X
	int selectImageHandle_;		// �I���摜�n���h��

	int arrayPlaceX_;
	int arrayPlaceY_;
	bool isSetShader_;
	bool isChangeImage_;
	bool isSelectable_;

public:
	/// <summary> �R���X�g���N�^ </summary>
	UIButton(string _name, UIObject* parent, int _layerNum);
	~UIButton() override;

	/// <summary> ������ </summary>
	void Initialize() override;
	
	/// <summary> �X�V </summary>
	void Update() override;
	
	/// <summary> �`�� </summary>
	void Draw() override;
	
	/// <summary> ��� </summary>
	void Release() override;

	/// <summary> �ۑ� </summary>
	void Save(json& saveObj) override;
	
	/// <summary> �Ǎ� </summary>
	void Load(json& loadObj) override;
	
	/// <summary> ImGui�\�� </summary>
	void DrawData() override;


/*
setter :*/
	/// <summary> �摜�t�@�C���p�X��ݒ� </summary>
	void SetImage(string _imageFilePath);
	void SetShader(Direct3D::SHADER_TYPE type);
	void SetArrayPlace(int x, int y);
	void SetSelctable(bool flag) { isSelectable_ = flag; };
/*
getter */

	//���̔z��̒��ɂ���{�^���ň�ԏォ�I������Ă���{�^�����擾
	static UIButton* GetTopSelectingUI(vector<UIObject*> list);

	void GetArrayPlace(int* x, int* y) const;

	string GetImageFilePath() const { return imageFilePath_; }

	Direct3D::SHADER_TYPE  GetShader()const;

	// ���̃{�^���Ƀ}�E�X������Ă��邩
	bool GetIsMouseOverThisButton();

	bool IsSelectable() const { return isSelectable_; };
/*
setter :*/
	/// <summary> �摜�t�@�C���p�X��ݒ� </summary>

	void SetSelectImage(string _imageFilePath);
	void SetDefaultImage(string _imageFilePath);
/*
predicate :*/
	/// <summary> �{�^�����N���b�N���ꂽ�� </summary>
	bool OnClick();

	/// <summary> �}�E�X���{�^���̏�ɂ��邩 </summary>
	bool IsMouseOver(XMFLOAT2 _position);

private:
	/// <summary> �摜���W����ʍ��W�ɕϊ� </summary>
	void ConvertToImageCoordinates(XMFLOAT2& _position);
};

