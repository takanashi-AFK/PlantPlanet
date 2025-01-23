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
	
	//外部から呼び出される関数
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

	//シーンを変える
	void SceneChange();

	void (Component_ReturnGate::* nowFunc_)();

private:

	/// <summary>
	/// 使用可能かどうか
	/// </summary>
	bool isUsable_ : 1;

	/// <summary>
	/// 見えるかどうか
	/// </summary>
	bool isVisible_ : 1;
};

