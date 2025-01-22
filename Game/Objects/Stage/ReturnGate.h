#pragma once
#include "StageObject.h"

class ReturnGate : public StageObject
{
private:


public:

	ReturnGate(GameObject* _parent);
	~ReturnGate();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void Intaract();
};

ReturnGate* CreateReturnGate(GameObject* parent , string name = "ReturnGate");