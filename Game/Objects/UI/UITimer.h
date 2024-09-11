#pragma once
#include "UIObject.h"

class UITimer : public UIObject
{
private:
	float count_;	// ���ݒl
	float max_;		// �ő�l

	bool isActive_;	// �^�C�}�[�������Ă��邩
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
	/// <summary> �^�C�}�[�̐ݒ� </summary>
	void SetTimer(float _max) { max_ = _max; count_ = max_; }

	/// <summary> �^�C�}�[�̃��Z�b�g </summary>
	void ResetTimer() { count_ = max_; isActive_ = false; }
/*
getter :*/
	/// <summary> �^�C�}�[�̎擾 </summary>
	float GetTimer() const { return count_; }

	/// <summary> �ő�l�̎擾 </summary>
	float GetMaxTimer() const { return max_; }

/*
predicate :*/
	/// <summary> �^�C�}�[��0�ɂȂ����� </summary>
	bool IsEnd() const { return count_ <= 0; }

	/// <summary> �^�C�}�[���ő�l�� </summary>
	bool IsMax() const { return count_ >= max_; }
};

