#pragma once
#include "../../../Engine/Json/JsonReader.h"
#include <vector>
#include "UIObject.h"


using std::vector;
class UIButton;


class UIPanel : public UIObject
{
private:
	static UIPanel* instance_;	//�V���O���g���C���X�^���X

public:
	/// <summary> �C���X�^���X�̎擾 </summary>
	static UIPanel* GetInstance();
	
	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;

	/// <summary> �ۑ� </summary>
	void Save(json& _saveObj)override;

	/// <summary> �Ǎ� </summary>
	void Load(json& _loadObj)override;
	
	/// <summary> ImGui�\�� </summary>
	void DrawData()override;

	/// <summary> �I�u�W�F�N�g�̍폜 </summary>
	void DeleteUIObject(UIObject* _object);

	/// <summary> �S�I�u�W�F�N�g�̍폜 </summary>
	void DeleteAllUIObject();

	/// <summary> �I�u�W�F�N�g�̃\�[�g </summary>
	void SortUIObject();


/*
getter :*/
	/// <summary> �I�u�W�F�N�g�̎擾 </summary>
	UIObject* GetUIObject(string _name);

	/// <summary> �I�u�W�F�N�g�̎擾 </summary>
	vector<UIObject*> GetUIObject(UIType _type);

	/// <summary> �I�u�W�F�N�g���X�g�̎擾 </summary>
	vector<UIObject*> GetUIObjects() { return childList_; };

/*
setter :*/
	/// <summary> �I�u�W�F�N�g�̉�����ݒ� </summary>
	void SetVisible(string _name, bool _visible);
private:
	/// <summary> �R���X�g���N�^ </summary>
	UIPanel();

	/// <summary> �R�s�[�R���X�g���N�^�̏��O </summary>
	UIPanel(const UIPanel&) = delete;

	/// <summary> ������Z�q�̏��O </summary>
	UIPanel& operator=(const UIPanel&) = delete;

//---------------------------------�}�E�X�ȊO�ō��W���擾����

private:

	std::list<UIButton*>  arrayButton_;
	int16_t buttonIndexX_;
	int16_t buttonIndexY_;
	UIButton* selectingButton_;

public:

	enum class SELECTOR_MOVE_TO
	{
		UP=0,
		LEFT,
		RIGHT,
		BOTTOM,

		AMOUNT
	};

	bool SetButtonArrayIndex(int16_t x, int16_t y);
	void GetButtonIndex(int16_t* x, int16_t* y);
	UIButton* GetSelectingButton();
	void SetCursorToSelectingButton();
	void SelectorMove(SELECTOR_MOVE_TO way);

	void PushButtonToArray(UIButton* b);
	void RemoveButtonFromArray(UIButton* b);
	void ResetArrayOfButton();

	void CheckSelectingButton();

	// �I������{�^����array�������I�Ɍ���
	void SetSelectingButton(int16_t x, int16_t y);

private:

	std::vector<UIButton*> GetSelectorMovable(SELECTOR_MOVE_TO way);
	
};

