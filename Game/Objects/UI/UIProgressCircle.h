#pragma once
#include "UIObject.h"

class UIProgressCircle : public UIObject
{
private:
	float max_;                    // �ő�l
	float now_;	                // ���ݒl

	float startAngle_;              // �J�n�p�x
	float endAngle_;                // �I���p�x

	string imageFilePath_;          // �摜�t�@�C���p�X
	int imageHandle_;               // �摜�n���h��
	XMFLOAT3 color_;                // �F

public:
	/// <summary> �R���X�g���N�^ </summary>
	UIProgressCircle(std::string _name, UIObject* parent, int _layerNum);

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
	/// <summary> �ő�l��ݒ� </summary>
	void SetMax(float _max) { max_ = _max; }

	/// <summary> ���ݒl��ݒ� </summary>
	void SetNow(float _now) { now_ = _now; }

	/// <summary> �ő�l�ƌ��ݒl��ݒ� </summary>
	void SetProgress(float* _now, float* _max) { now_ = _now; max_ = _max; }

	/// <summary> �J�n�p�x��ݒ� </summary>
	void SetStartAngle(float _angle) { startAngle_ = _angle; }

	/// <summary> �I���p�x��ݒ� </summary>
	void SetEndAngle(float _angle) { endAngle_ = _angle; }

	/// <summary> �F��ݒ� </summary>
	void SetColor(XMFLOAT3 _color) { color_ = _color; }

	/// <summary> �g�p����摜��ݒ� </summary>
	bool SetImage(string _imageFilePath);

/*
getter :*/
	/// <summary> �ő�l���擾 </summary>
	float GetMax() const { return max_; }

	/// <summary> ���ݒl���擾 </summary>
	float GetNow() const { return now_; }

	/// <summary> �J�n�p�x���擾 </summary>
	float GetStartAngle() const { return startAngle_; }

	/// <summary> �I���p�x���擾 </summary>
	float GetEndAngle() const { return endAngle_; }

	/// <summary> �F���擾 </summary>
	XMFLOAT3 GetColor() const { return color_; }

	/// <summary> �摜�t�@�C���p�X���擾 </summary>
	string GetImageFilePath() { return imageFilePath_; }

	/// <summary> �摜�n���h�����擾 </summary>
	int GetImageHandle() { return imageHandle_; }

private:
	/// <summary> �摜�t�@�C���p�X���G�N�X�v���[���[����擾 </summary>
	string GetPNGFilePath();

};

