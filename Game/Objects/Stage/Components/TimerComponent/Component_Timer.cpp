#include "Component_Timer.h"
#include "../../../../../Engine/ImGui/imgui.h"
namespace {
	const int FPS = 60;
}

Component_Timer::Component_Timer(StageObject* _holder)
	:Component(_holder, "Component_Timer", Timer)
	, maxTime_(0), nowTime_(0),tempMax_(0), isEnd_(false), countNow_(false), isInfinity_(true)
{
}

void Component_Timer::Initialize()
{
}

void Component_Timer::Update()
{
	if (nowTime_ >= maxTime_ && !isInfinity_) Stop();
	// �J�E���g���łȂ��ꍇ,�J�E���g��i�߂�
	if (!countNow_) return;
	nowTime_++;
}

void Component_Timer::Release()
{
}

void Component_Timer::Save(json& _saveObj)
{
	_saveObj["time_"] = maxTime_;
	_saveObj["nowTime_"] = nowTime_;
	_saveObj["isEnd_"] = isEnd_;
	_saveObj["countNow_"] = countNow_;
}

void Component_Timer::Load(json& _loadObj)
{
	if (_loadObj.find("time_") != _loadObj.end()) 	maxTime_ = _loadObj["time_"];
	if (_loadObj.find("nowTime_") != _loadObj.end()) 	nowTime_ = _loadObj["nowTime_"];
	if (_loadObj.find("isEnd_") != _loadObj.end())   isEnd_ = _loadObj["isEnd_"];
	if (_loadObj.find("countNow_") != _loadObj.end())   countNow_ = _loadObj["countNow_"];
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
	countNow_ = true;
	isEnd_ = false;
}

void Component_Timer::Stop()
{
	//countNow_��false�ɂ��A�^�C�}�[���~
	countNow_ = false;
	isEnd_ = true;
}

float Component_Timer::GetNowTime()
{
	return nowTime_ / FPS;
}

bool Component_Timer::GetIsEnd()
{
	return isEnd_;
}

void Component_Timer::SetTime(int _time)
{
	maxTime_ = _time * FPS;
	isInfinity_ = false;
}

void Component_Timer::Reset()
{
	countNow_ = false;
	nowTime_ = 0;
	isEnd_ = false;
}

bool Component_Timer::IsOnTime(float _time)
{
	//���̎��Ԃ��w�肳�ꂽ�b�̎�true��Ԃ�
	if (nowTime_ >= _time*FPS)return true;
	return false;
}

bool Component_Timer::IsIntervalTime(float _time)
{
	//���̎��Ԃ��w�肳�ꂽ�b�̔{���̎�true��Ԃ�
	if (static_cast<int>(nowTime_) % (static_cast<int>(_time) * FPS) == 0)
		return true;
	return false;
}
