#include "UITimer.h"
#include "../../../Engine/ImGui/imgui.h"


UITimer::UITimer(string _name, UIObject* parent, int _layerNum)
	: UIObject(_name, UI_TIMER, parent, _layerNum), max_(0), count_(0), isActive_(false)
{
}

void UITimer::Initialize()
{
}

void UITimer::Update()
{
	// �J�E���^�[���X�V
	if(isActive_)count_--;

	// �J�E���^�[��0�ɂȂ������A�N�e�B�u�ɂ���
	if (count_ <= 0){
		count_ = 0;
		isActive_ = false;
	}
}

void UITimer::Draw()
{
}

void UITimer::Release()
{
}

void UITimer::Save(json& saveObj)
{
	// �ő�l�̕ۑ�
	saveObj["max"] = max_;
}

void UITimer::Load(json& loadObj)
{
	// �ő�l�̓ǂݍ���
	max_ = loadObj["max"];
	count_ = max_;
}

void UITimer::DrawData()
{
	// �J�E���^�[�̎��s�e�X�g
	if (ImGui::TreeNode("UITimer")) {

		// �J�n�{�^���̕\��
		if(ImGui::SmallButton("start"))StartTimer();
		ImGui::SameLine();

		// ��~�{�^���̕\��
		if(ImGui::SmallButton("stop"))StopTimer();
		ImGui::SameLine();

		// �I���{�^���̕\��
		if(ImGui::SmallButton("end"))EndTimer();
		ImGui::SameLine();
		
		// ���Z�b�g�{�^���̕\��
		if(ImGui::SmallButton("reset"))ResetTimer();
		ImGui::SameLine();

		// ���X�^�[�g�{�^���̕\��
		if(ImGui::SmallButton("restart"))RestartTimer();

		// �ő�l��b�Őݒ�
		{
			static float sec = 0;
			ImGui::InputFloat("sec", &sec);
			ImGui::SameLine();
			if (ImGui::SmallButton("set"))SetTimer(sec);
		}

		// �J�E���^�[�̕\��
		ImGui::Text("count : %f", count_);
		ImGui::Text("sec   : %f", count_ / FPS);
		ImGui::TreePop();
	}
}
