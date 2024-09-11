#include "UITimer.h"

// �C���N���[�h
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Text.h"


UITimer::UITimer(string _name, UIObject* parent, int _layerNum)
	: UIObject(_name, UI_TIMER, parent, _layerNum), max_(0), count_(0), isActive_(false), pText_(nullptr), size_(1.f)
{
}

void UITimer::Initialize()
{
	// �e�L�X�g�I�u�W�F�N�g�𐶐���������
	pText_ = new Text();
	pText_->Initialize();
}

void UITimer::Update()
{
	// �J�E���^�[���X�V
	if(isActive_)count_--;

	// �J�E���^�[��0�ɂȂ�����I��
	if (count_ <= 0) EndTimer();
}

void UITimer::Draw()
{

	// �\���ʒu���v�Z
	float drawX = (transform_.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);
	float drawY = (transform_.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);
	
	// �\���T�C�Y��ݒ�
	pText_->SetScale(size_);

	// �������ݒ�
	string str{}; {
		int totalSeconds = count_ / FPS;
		int minutes = totalSeconds / 60;
		int seconds = totalSeconds % 60;

		str = std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
	}

	// �e�L�X�g��`��
	pText_->Draw(drawX, drawY,str.c_str());
}

void UITimer::Release()
{
}

void UITimer::Save(json& saveObj)
{
	// �ő�l�̕ۑ�
	saveObj["max"] = max_;

	// �e�L�X�g�T�C�Y�̕ۑ�
	saveObj["size"] = size_;
}

void UITimer::Load(json& loadObj)
{
	// �ő�l�̓ǂݍ���
	if(loadObj.contains("max"))max_ = loadObj["max"];
	count_ = max_;

	// �e�L�X�g�T�C�Y�̓ǂݍ���
	if(loadObj.contains("size"))size_ = loadObj["size"];
}

void UITimer::DrawData()
{
	// �`��̐ݒ�
	if (ImGui::TreeNode("draw")) {
		
		// �e�L�X�g�̐ݒ�
		ImGui::DragFloat("size", &size_, 0.01f, 0.1f, 10.f);

		
		ImGui::TreePop();
	}

	// �l�̐ݒ�
	if (ImGui::TreeNode("value")) {

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
