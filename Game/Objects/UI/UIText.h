#pragma once
#include "UIObject.h"

class Text;

struct FontData {
	string filePath;
	XMINT2 charSize;
	int rowLength;
};

class UIText : public UIObject
{
private:
	Text* pText_;	// �e�L�X�g�C���X�^���X
	string drawText_;	// �`�悷�镶����
	int* intNum_;		// �`�悷�鐔��
	
	float size_;		// �e�L�X�g�T�C�Y
	FontData fontData_;	// �t�H���g�f�[�^

	bool isOpenChangeFontWindow_;	// �t�H���g�ύX�E�B���h�E���J�����ǂ���

public:
	/// <summary> �R���X�g���N�^ </summary>
	UIText(string _name, UIObject* parent, int _layerNum);

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
	/// <summary> �������ݒ� </summary>
	void SetText(string _text) { drawText_ = _text; }
	
	/// <summary> ������ݒ� </summary>
	void SetText(int* _num) { intNum_ = _num; }

	/// <summary> �e�L�X�g�T�C�Y��ݒ� </summary>
	void SetSize(float _size) { size_ = _size; }

/*
getter :*/
	/// <summary> ��������擾 </summary>
	string GetText() const { return drawText_; }

	/// <summary> �������擾 </summary>
	int* GetNum() const { return intNum_; }

	/// <summary> �e�L�X�g�T�C�Y���擾 </summary>
	float GetSize() const { return size_; }

private:
	void ChangeFontWindow();

	bool GetFontFilePathFromExplorer(string& _filePath) const;
};

