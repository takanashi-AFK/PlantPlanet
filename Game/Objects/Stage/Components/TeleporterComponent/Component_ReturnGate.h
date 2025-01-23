#pragma once
#include "../Component.h"
#include "../../Stage.h"

class Component_ReturnGate : public Component
{
public:
	Component_ReturnGate(string _name, StageObject* _holder, Component* _parent);
	~Component_ReturnGate();
	void Initialize() override;
	void Update() override;
	void Release() override;
	
	//�O������Ăяo�����֐�
	void Work();

	void SetVisible(bool b);
	void SetUsable(bool b);

	bool IsVisible();
	bool IsUsable();

	void DrawData()override;

	void Save(json& _saveObj);
	void Load(json& _loadObj);

private:

	void EmptyWork();

	//�V�[����ς���
	void SceneChange();

	void (Component_ReturnGate::* nowFunc_)();

private:

	/// <summary>
	/// �g�p�\���ǂ���
	/// </summary>
	bool isUsable_ : 1;

	/// <summary>
	/// �����邩�ǂ���
	/// </summary>
	bool isVisible_ : 1;
};

