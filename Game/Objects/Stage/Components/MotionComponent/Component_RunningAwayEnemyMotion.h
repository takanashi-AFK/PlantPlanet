#pragma once
#include <vector>
#include "../../Components/BehaviorComponents/Component_RunningAwayEnemyBehavior.h"
#include <map>
#include"Component_Motion.h"
struct MotioData;

class Component_RunningAwayEnemyMotion :public Component
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_RunningAwayEnemyMotion(string _name, StageObject* _holder, Component* _parent);

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
	static constexpr int endRunningFrame_ = 60;
	static constexpr int endDyingFrame = 20;
private:

	std::map<Component_RunningAwayEnemyBehavior::STATE, MotionData>modelHandleList_;
};