#pragma once
#include "UIObject.h"

class UIButton : public UIObject
{
private:
	string imageFilePath_; // �摜�t�@�C���p�X
	int imageHandle_;		// �摜�n���h��

	int16_t arrayPlaceX_;
	int16_t arrayPlaceY_;

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

	void SetArrayPlace(int16_t x, int16_t y);
	void GetArrayPlace(int16_t* x, int16_t* y) const;
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
private:
	/// <summary> �摜���W����ʍ��W�ɕϊ� </summary>
	void ConvertToImageCoordinates(XMFLOAT2& _position);
};

