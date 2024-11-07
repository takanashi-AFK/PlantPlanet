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

	// �Q�[�W�̉摜��ǂݍ���
	circleGauge_.Load("Images/defaults/Timer_Gauge.png");
}

void UITimer::Update()
{
	if (isActive_)count_--;  // �J�E���g�_�E��

	// �J�E���^�[��0�ɂȂ�����I��
	if (count_ <= 0)EndTimer();

	// �J�E���g�_�E���ɉ����ăQ�[�W�̐i�s�x���v�Z
	float progress = (count_ > 0) ? (count_ / max_) : 0.0f;  // �i�s�������v�Z
	circleGauge_.Update(progress);  // �i�s��������ɃQ�[�W���X�V
}

void UITimer::Draw()
{
	switch (drawType_)
	{
	case TEXT:DrawTextTimer(); break;
	case CIRCLE_GAUGE:DrawCircleGaugeTimer(); break;
	}
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

	// �`����@�̕ۑ�
	saveObj["drawType"] = drawType_;

	// Circlegauge�̃t�@�C���p�X��ۑ�
	saveObj["circleGaugeImage"] = circleGauge_.imageFilePath;
}

void UITimer::Load(json& loadObj)
{
	// �ő�l�̓ǂݍ���
	if(loadObj.contains("max"))max_ = loadObj["max"];
	count_ = max_;

	// �e�L�X�g�T�C�Y�̓ǂݍ���
	if(loadObj.contains("size"))size_ = loadObj["size"];

	// �`����@�̓ǂݍ���
	if(loadObj.contains("drawType"))drawType_ = (TIMER_DRAW_TYPE)loadObj["drawType"];

	// Circlegauge�̃t�@�C���p�X��ǂݍ���
	if(loadObj.contains("circleGaugeImage"))circleGauge_.Load(loadObj["circleGaugeImage"]);
}

void UITimer::DrawData()
{
	// �`��̐ݒ�
	if (ImGui::TreeNode("draw")) {

		// �`��^�C�v�̐ݒ�
		ImGui::Text("draw type");
		ImGui::RadioButton("text", (int*)&drawType_, TEXT);
		ImGui::SameLine();
		ImGui::RadioButton("circle gauge", (int*)&drawType_, CIRCLE_GAUGE);

		switch (drawType_)
		{
		case TEXT:
			// �e�L�X�g�̐ݒ�
			ImGui::DragFloat("size", &size_, 0.01f, 0.1f, 10.f);
			break;
		case CIRCLE_GAUGE:
			// �Q�[�W�摜�̕ύX
			ImGui::Text("image : %s", circleGauge_.imageFilePath.c_str()); ImGui::SameLine();
			if (ImGui::SmallButton("...")) {

				// �摜�t�@�C����I�����ēǂݍ���
				string inputFilePath{};
				if (GetImageFilePathFromExplorer(inputFilePath)) circleGauge_.Load(inputFilePath);
			}
			break;
		}

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

void UITimer::DrawTextTimer()
{
	// �\���ʒu���v�Z
	auto&& t = GetCalcTransform();

	float drawX = (t.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);
	float drawY = (t.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);

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
	pText_->Draw(drawX, drawY, str.c_str());
}

void UITimer::DrawCircleGaugeTimer()
{
	// �摜��`��
	circleGauge_.Draw(transform_);
}

bool UITimer::GetImageFilePathFromExplorer(string& _filePath) const
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
	string filePath{}; {
		// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.png)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = FileManager::GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			FileManager::ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
			return false;
		}
	}

	_filePath = filePath;
	return true;
}

bool CircleGauge::Load(string _imageFilePath)
{
	// �摜�t�@�C���p�X��ݒ�
	imageFilePath = _imageFilePath;

	// �Q�[�W�̉摜��ǂݍ���
	imageHandle = Image::Load(_imageFilePath);

	// �ǂݍ��݂ɐ���������
	return imageHandle >= 0;
}

void CircleGauge::Update(float progress)
{
	// Progress��0.0 ~ 1.0�œn����A�Q�[�W�̊p�x�ɔ��f�����
	startAngle = 0;  // �Œ�ŊJ�n�p�x��ݒ�
	endAngle = startAngle + (progress * 360.0f);  // Progress�Ɋ�Â��I���p�x
}

void CircleGauge::Draw(Transform _transform)
{
	Image::SetTransform(imageHandle, _transform);
	Image::Draw(imageHandle, startAngle, endAngle);
}
