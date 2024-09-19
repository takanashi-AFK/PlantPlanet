#pragma once
#include "UIObject.h"

class UIImage : public UIObject
{
private:
	string imageFilePath_;	// �摜�t�@�C���p�X
	int imageHandle_;		// �摜�n���h��
	bool isDraw_;			// �`�悷�邩�ǂ���
public:
	/// <summary> �R���X�g���N�^ </summary>
	UIImage(string _name, UIObject* parent, int _layerNum);
	
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

	/// <summary> �`�悷�邩�ǂ�����ݒ� </summary>
	void SetEnable(bool _enable) { isDraw_ = _enable; }

/*
predicate :*/
	/// <summary> �`�悷�邩�ǂ������擾 </summary>
	bool IsEnable() const { return isDraw_; }
};

