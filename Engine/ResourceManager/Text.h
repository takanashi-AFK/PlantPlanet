#pragma once
#include "Image.h"

class Text
{
private:
	int hPict_;						// �s�N�`���n���h��
	unsigned int width_, height_;	// 1�������̕��ƍ���
	char fileName_[MAX_PATH];		// �t�@�C����
	unsigned int rowLength_;		// 1�s�ɕ��ԕ�����

public:
	/// <summary> �R���X�g���N�^ </summary>
	Text();

	/// <summary> �f�X�g���N�^ </summary>
	~Text();

	/// <summary> ������ </summary>
	HRESULT Initialize();

	//�������i�I���W�i���̉摜���g���������j
	//�����FfileName	�摜�t�@�C����
	//�����FcharWidth	�摜�̂P�����̕��i�s�N�Z���j
	//�����FcharHeight	�摜�̂P�����̍����i�s�N�Z���j
	//�����FrowLength	�摜�̒��ɉ��ɉ���������ł��邩
	//�ߒl�F�����^���s
	HRESULT Initialize(const char* fileName, const unsigned int charWidth, const unsigned int charHeight, const unsigned int rowLength);

	//�`��i������j
	//�����Fx, y	�\���ʒu�i����j
	//�����Fstr		�\��������������
	void Draw(int x, int y, const char* str);

	//�`��i�����l�j
	//�����Fx, y	�\���ʒu�i����j
	//�����Fvalue	�\���������l
	void Draw(int x, int y, int value);

	///	<summary> ��� </summary>
	void Release();

/*
setter :*/
	void SetScale(float sc) { scale = sc; }
/*
getter :*/
	unsigned int GetWidth() const { return width_; }
	unsigned int GetHeight()const  { return height_; }
	string GetFileName()const { return fileName_; }
	unsigned int GetRowLength()const { return rowLength_; }

private:
	float scale;
};

