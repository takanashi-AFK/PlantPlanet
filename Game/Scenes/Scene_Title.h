#pragma once

#include "../../Engine/GameObject/GameObject.h"

#include <string>
#include <vector>

using std::string;
using std::vector;
class UIPanel;

class Scene_Title:public GameObject
{
private:
	bool	isFirstSelectButton_;	// �ŏ��Ƀ{�^����I�����Ă���ǂ�����ێ�����ϐ�
	int		status_;				// ���݂̏�Ԃ�ێ�����ϐ�

public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Title(GameObject* parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;

private:
	/// <summary> UI���͏��� </summary>
	void HandleUIInput(UIPanel* _uiPanel, bool& _isFirstSelectButton);

	/// <summary> �{�^���A�N�V�������� </summary>
	void ProcessButtonAction(UIPanel* _uiPanel,string _buttonName, string _userName);

/*
uiControl: */
	/// <summary> UI�I�u�W�F�N�g�̕\��/��\����ݒ� </summary>
	void SetUIVisible(UIPanel* _uiPanel, vector<string> _uiObjectNames, bool _visible);

	/// <summary> �|�b�v�A�b�v����� </summary>
	void ClosePopup(UIPanel* _uiPanel);

};
