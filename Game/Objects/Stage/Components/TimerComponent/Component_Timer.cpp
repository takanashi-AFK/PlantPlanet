#include "../../../../../Engine/ImGui/imgui.h"
#include "Component_Timer.h"

namespace
{
	const int FPS = 60;
}

Component_Timer::Component_Timer(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, Timer,_parent)
	, maxTime_(0), nowTime_(0),tempMax_(0), isEnd_(false), isCountNow_(false), isInfinity_(true)
{
}

void Component_Timer::Initialize()
{
}

void Component_Timer::Update()
{
	if (nowTime_ >= maxTime_ && !isInfinity_)
		End();

	if (!isCountNow_) return;
	nowTime_++;
}

void Component_Timer::Release()
{
}

void Component_Timer::Save(json& _saveObj)
{
	_saveObj["maxTime_"] = maxTime_;
	_saveObj["nowTime_"] = nowTime_;
}

void Component_Timer::Load(json& _loadObj)
{
	if (_loadObj.find("maxTime_") != _loadObj.end()) 	maxTime_ = _loadObj["maxTime_"];
	if (_loadObj.find("nowTime_") != _loadObj.end()) 	nowTime_ = _loadObj["nowTime_"];
}

void Component_Timer::DrawData()
{
	//���݂̎��Ԃ�\��
	ImGui::Text("%f", GetNowTime());

	//�ő厞�Ԃ�ݒ�
	ImGui::DragFloat("Time", &tempMax_, 1.f,0,100);
	if (ImGui::Button("Set"))SetTime(tempMax_);
	ImGui::SameLine();

	//�J�n
	if (ImGui::Button("Start"))Start();
	ImGui::SameLine();

	//��~
	if (ImGui::Button("Stop"))Stop();
	ImGui::SameLine();

	//���Z�b�g
	if (ImGui::Button("Reset"))Reset();

	//�I��������
	ImGui::Text("isEnd_ : %s",isEnd_ ? "true" : "false");
}

void Component_Timer::Start()
{
	//countNow_��true�ɂ��A�^�C�}�[���J�n
	isCountNow_ = true;
}

void Component_Timer::Stop()
{
	//countNow_��false�ɂ��A�^�C�}�[���~
	isCountNow_ = false;
}

float Component_Timer::GetNowTime()
{
	return nowTime_ / FPS;
}

bool Component_Timer::GetIsEnd()
{
	return isEnd_;
}

void Component_Timer::SetTime(float _time)
{
	maxTime_ = _time * FPS;
	isInfinity_ = false;
}

void Component_Timer::Reset()
{
	isCountNow_ = false;
	nowTime_ = 0;
	isEnd_ = false;
}

bool Component_Timer::IsOnTime(float _time)
{
	//���̎��Ԃ��w�肳�ꂽ�b�̎�true��Ԃ�
	return nowTime_ >= _time * FPS;
}

void Component_Timer::End()
{
	Stop(); 
	isEnd_ = true;
}

bool Component_Timer::IsIntervalTime(float _time)
{
	//���̎��Ԃ��w�肳�ꂽ�b�̔{���̎�true��Ԃ�
	return static_cast<int>(nowTime_) % (static_cast<int>(_time) * FPS) == 0;
}

bool Component_Timer::GetIsCountNow()
{
	return isCountNow_;
}
