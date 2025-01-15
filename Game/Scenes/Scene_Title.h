#pragma once

#include "../../Engine/GameObject/GameObject.h"

using std::string;
class UIPanel;

class Scene_Title:public GameObject
{
private:
	bool isFirstSelectButton_;
	int status_;	// ���݂̏�Ԃ�ێ�����ϐ�

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
};

