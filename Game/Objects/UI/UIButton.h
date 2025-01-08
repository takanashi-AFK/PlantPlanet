#pragma once
#include "UIObject.h"
#include"../../../Engine/DirectX/Direct3D.h"

class UIButton : public UIObject
{
private:
	string imageFilePath_; // �摜�t�@�C���p�X
	int imageHandle_;		// �摜�n���h��
	Direct3D::SHADER_TYPE shaderType_;

	int arrayPlaceX_;
	int arrayPlaceY_;
	bool isSetShader_;

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

	void SetArrayPlace(int x, int y);
	void GetArrayPlace(int* x, int* y) const;

	string GetImageFilePath() const { return imageFilePath_; }

	void SetShader(Direct3D::SHADER_TYPE type);
	Direct3D::SHADER_TYPE  GetShader()const;

	bool GetIsMouseOverThisButton();
/*
setter :*/
	/// <summary> �摜�t�@�C���p�X��ݒ� </summary>
	void SetImage(string _imageFilePath);

/*
predicate :*/
	/// <summary> �{�^�����N���b�N���ꂽ�� </summary>
	bool OnClick();

	/// <summary> �}�E�X���{�^���̏�ɂ��邩 </summary>
	bool IsMouseOver(XMFLOAT2 _position);

	static constexpr int ALPHA_MAX = 256;
	static constexpr int ALPHA_MIDDLE = 192;

private:
	/// <summary> �摜���W����ʍ��W�ɕϊ� </summary>
	void ConvertToImageCoordinates(XMFLOAT2& _position);
};

