#pragma once

#include "UIObject.h"

class Text;

class UIInputString : public UIObject
{
private:
	string currentInput_;				// ���͂��ꂽ������
	bool isComplete_;					// ���͊����t���O
	std::map<int, char> keyToCharMap_;	// �X�L�����R�[�h�ƑΉ����镶��

	Text* pText_;						// �e�L�X�g�I�u�W�F�N�g
	float size_;						// �e�L�X�g�̃T�C�Y
	bool isOpenChangeFontWindow_;		// �t�H���g�ύX�E�B���h�E���J�����ǂ���

	struct FontData { string filePath; XMINT2 charSize; int rowLength; } fontData_;	// �t�H���g�f�[�^

public:
	/// <summary> �R���X�g���N�^ </summary>
	UIInputString(string _name, UIObject* parent, int _layerNum);

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
	/// <summary> �e�L�X�g�̃T�C�Y��ݒ� </summary>
	void SetSize(float size) { size_ = size; }

	/// <summary> ���͊����t���O��ݒ� </summary>
	void SetComplete(bool isComplete) { this->isComplete_ = isComplete; }

	/// <summary> ���͂��ꂽ�������ݒ� </summary>
	void SetInputString(string input) { currentInput_ = input; }

/*
getter :*/
	/// <summary> �e�L�X�g�̃T�C�Y���擾 </summary>
	float GetSize() const { return size_; }

	/// <summary> ���͂��ꂽ��������擾 </summary>
	string GetInputString() const { return currentInput_; }

/*
predicate :*/
	/// <summary> ���͊����t���O���擾 </summary>
	bool IsComplete() const{ return isComplete_; }

private:
	/// <summary> �t�H���g�ύX�E�B���h�E </summary>
	void ChangeFontWindow();

	/// <summary> �t�H���g�t�@�C���p�X���G�N�X�v���[������擾 </summary>
	bool GetFontFilePathFromExplorer(string& _filePath) const;

	/// <summary> �L�[�}�b�s���O�̏����� </summary>
	void InitializeKeyMapping();
};

