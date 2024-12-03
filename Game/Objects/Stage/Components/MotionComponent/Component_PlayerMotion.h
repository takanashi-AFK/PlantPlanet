#pragma once
#include "Component_Motion.h"
#include <vector>
#include "../../Components/BehaviorComponents/Component_PlayerBehavior.h"

class Component_PlayerMotion : public Component_Motion
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_PlayerMotion(string _name, StageObject* _holder, Component* _parent);
	
	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> ��� </summary>
	void Release() override;

	/// <summary> ImGui�p�l���\�� </summary>
	void DrawData() override;
/*
getter :*/
	/// <summary> ���݂̃t���[�����擾 </summary>
	int GetNowFrame();
/*
predicate :*/
	/// <summary> �A�j���[�V�������I�������� </summary>
	bool IsEnd();

	//��������̃A�j���[�V��������
	static const int shotWalkTime_ = 90;
};

