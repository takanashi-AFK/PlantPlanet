#pragma once
#include "StageObject.h"

class MenuCharacter :public StageObject
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	MenuCharacter(GameObject* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override {};

};

