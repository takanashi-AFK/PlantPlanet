#pragma once
#include "../../../Engine/Json/JsonReader.h"
#include <vector>
#include "UIObject.h"


using std::vector;


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

private:
	/// <summary> �R���X�g���N�^ </summary>
	UIPanel();

	/// <summary> �R�s�[�R���X�g���N�^�̏��O </summary>
	UIPanel(const UIPanel&) = delete;

	/// <summary> ������Z�q�̏��O </summary>
	UIPanel& operator=(const UIPanel&) = delete;

};

