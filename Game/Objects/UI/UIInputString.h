#pragma once

#include "UIObject.h"

class Text;

class UIInputString : public UIObject
{
private:
	string currentInput;				// ���͂��ꂽ������
	bool isComplete = false;			// ���͊����t���O
	std::map<int, char> keyToCharMap;	// �X�L�����R�[�h�ƑΉ����镶��

	Text* pText_;						// �e�L�X�g�I�u�W�F�N�g
	float size_;						// �e�L�X�g�̃T�C�Y

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
	void SetComplete(bool isComplete) { this->isComplete = isComplete; }

	/// <summary> ���͂��ꂽ�������ݒ� </summary>
	void SetInputString(string input) { currentInput = input; }

/*
getter :*/
	/// <summary> �e�L�X�g�̃T�C�Y���擾 </summary>
	float GetSize() const { return size_; }

	/// <summary> ���͂��ꂽ��������擾 </summary>
	string GetInputString() const { return currentInput; }

/*
predicate :*/
	/// <summary> ���͊����t���O���擾 </summary>
	bool IsComplete() const{ return isComplete; }

private:
	/// <summary> �L�[�}�b�s���O�̏����� </summary>
	void InitializeKeyMapping();
};

