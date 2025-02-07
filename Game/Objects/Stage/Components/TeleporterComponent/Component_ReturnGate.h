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
	
	[[deprecated("Sceneの派生クラスでのみシーンチェンジを行うと決まったため、この関数を使ったシーンチェンジは非推奨です")]]
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

	//シーンを変える
	void SceneChange();

	void BranchUpdateFunc(State state);

	void FadeInPopUp();
	void KeepPopUp();
	void FadeOutPopUp();

	void EasingUI(string uiName);

	//Work()内で使う関数ポインタ
	void (Component_ReturnGate::* workFunc_)();

	//Update内で使う関数ポインタ
	void (Component_ReturnGate::* updateFunc_)();

private:

	/// <summary>
	/// 使用可能かどうか
	/// </summary>
	bool isUsable_ : 1;

	/// <summary>
	/// 見えるかどうか
	/// </summary>
	bool isVisible_ : 1;

	/// <summary>
	/// 現在の状態
	/// </summary>
	State state_;
	float fade_;
	float keepingTime_;

	static constexpr float KeepPopUpTime = 1.5 * FPS;
};

