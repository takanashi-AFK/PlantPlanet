#pragma once
#include "UIObject.h"
#include "../../../Engine/Global.h"

class Text;

enum TIMER_DRAW_TYPE {
	TEXT,
	CIRCLE_GAUGE,
};

struct CircleGauge
{
/*
value :*/
	int imageHandle = -1;	// �摜�n���h��
	string imageFilePath;	// �摜�t�@�C���p�X
	float startAngle = 90;	// �J�n�p�x
	float endAngle = 90;	// �I���p�x
/*
function :*/
	/// <summary> �Ǎ� </summary>
	bool Load(string imageFilePath);

	/// <summary> �X�V </summary>
	void Update(float progress);

	/// <summary> �`�� </summary>
	void Draw(Transform _transform);
};

class UITimer : public UIObject
{
private:
	float count_;				// ���ݒl
	float max_;					// �ő�l

	bool isActive_;				// �^�C�}�[�������Ă��邩
	
	TIMER_DRAW_TYPE drawType_;	// �`��^�C�v
	Text* pText_;				// �e�L�X�g�I�u�W�F�N�g
	float size_;				// �e�L�X�g�̃T�C�Y
	
	CircleGauge circleGauge_;	// �Q�[�W


public:
	/// <summary> �R���X�g���N�^ </summary>
	UITimer(string _name, UIObject* parent, int _layerNum);

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

	/// <summary> �^�C�}�[�̊J�n </summary>
	void StartTimer() { isActive_ = true; }

	/// <summary> �^�C�}�[�̒�~ </summary>
	void StopTimer() { isActive_ = false; }

	/// <summary> �^�C�}�[�̏I�� </summary>
	void EndTimer() { count_ = 0; isActive_ = false; }

/*
setter :*/
	/// <summary> �^�C�}�[�̐ݒ� ���b </summary>
	void SetTimer(float _sec) { max_ = _sec * FPS; count_ = max_; }

	/// <summary> �^�C�}�[�̃��Z�b�g </summary>
	void ResetTimer() { count_ = max_; isActive_ = false; }

	/// <summary> �^�C�}�[�̃��X�^�[�g </summary>
	void RestartTimer() { count_ = max_; isActive_ = true; }

/*
getter :*/
	/// <summary> �^�C�}�[�̎擾 </summary>
	float GetTimer() const { return count_; }

	/// <summary> �b���̎擾 </summary>
	float GetSeconds() const { return count_ / FPS; }

	/// <summary> �ő�l�̎擾 </summary>
	float GetMaxTimer() const { return max_; }

/*
predicate :*/
	/// <summary> �^�C�}�[��0�ɂȂ����� </summary>
	bool IsEnd() const { return count_ <= 0; }

	/// <summary> �^�C�}�[���ő�l�� </summary>
	bool IsMax() const { return count_ >= max_; }

private:
	/// <summary> �e�L�X�g�̕`�� </summary>
	void DrawTextTimer();

	/// <summary> �Q�[�W�̕`�� </summary>
	void DrawCircleGaugeTimer();

	/// <summary> �t�@�C���p�X����摜���擾 </summary>
	bool GetImageFilePathFromExplorer(string& _filePath) const;
};

