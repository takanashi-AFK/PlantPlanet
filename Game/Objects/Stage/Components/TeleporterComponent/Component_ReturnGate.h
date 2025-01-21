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

private:

	void EmptyWork();
	void SceneChange();

	void (Component_ReturnGate::* nowFunc_)();
};

