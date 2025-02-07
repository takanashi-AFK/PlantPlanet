#pragma once
#include "../Component.h"
#include "../../Stage.h"
#include "../../../Engine/Global.h"

class Component_ReturnGate : public Component
{

public:
	enum class State
	{
		IDLE,
		MESSAGE,

		AMOUNT
	};

public:
	Component_ReturnGate(string _name, StageObject* _holder, Component* _parent);
	~Component_ReturnGate();
	void Initialize() override;
	void Update() override;
	void Release() override;
	
	[[deprecated("Scene�̔h���N���X�ł̂݃V�[���`�F���W���s���ƌ��܂������߁A���̊֐����g�����V�[���`�F���W�͔񐄏��ł�")]]
	void Work();

	void SetVisible(bool b);
	void SetUsable(bool b);
	void SetState(State state);

	bool IsVisible();
	bool IsUsable();

	void DrawData()override;

	State GetState()const;

	void Save(json& _saveObj);
	void Load(json& _loadObj);

private:

	void EmptyWork();

	//�V�[����ς���
	void SceneChange();

	void BranchUpdateFunc(State state);

	void FadeInPopUp();
	void KeepPopUp();
	void FadeOutPopUp();

	void EasingUI(string uiName);

	//Work()���Ŏg���֐��|�C���^
	void (Component_ReturnGate::* workFunc_)();

	//Update���Ŏg���֐��|�C���^
	void (Component_ReturnGate::* updateFunc_)();

private:

	/// <summary>
	/// �g�p�\���ǂ���
	/// </summary>
	bool isUsable_ : 1;

	/// <summary>
	/// �����邩�ǂ���
	/// </summary>
	bool isVisible_ : 1;

	/// <summary>
	/// ���݂̏��
	/// </summary>
	State state_;
	float fade_;
	float keepingTime_;

	static constexpr float KeepPopUpTime = 1.5 * FPS;
};

