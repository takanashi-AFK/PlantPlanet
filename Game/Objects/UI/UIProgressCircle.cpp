#include "UIProgressCircle.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"

namespace {
	const string DEFAULT_IMAGE = "Images/defaults/default_ProgressCircle.png";
	const float START_ANGLE = 0.0f;
}

UIProgressCircle::UIProgressCircle(std::string _name, UIObject* parent, int _layerNum)
	: UIObject(_name,UIType::UI_PROGRESSCIRCLE, parent, _layerNum), max_(1), now_(0),endAngle_(360)
{
}

void UIProgressCircle::Initialize()
{
	// �摜��ǂݍ���
	SetImage(DEFAULT_IMAGE);

	// �F��ݒ�
	color_ = { 0.0f,0.0f,1.0f };

	// ���ݒl���ő�l�ŏ�����
	now_ = max_;
}

void UIProgressCircle::Update()
{
	// ���ݒl�ƍő�l����ɐi�s�x���v�Z
	float progress = now_ / max_;

	// �i�s�x����ɏI���p�x���v�Z
	endAngle_ = START_ANGLE + (progress * 360.0f);
}

void UIProgressCircle::Draw()
{
	Image::SetTransform(imageHandle_,transform_);
	Image::Draw(imageHandle_, START_ANGLE, endAngle_, color_);
}

void UIProgressCircle::Release()
{
}

void UIProgressCircle::Save(json& saveObj)
{
	// �摜�t�@�C���p�X�̕ۑ�
	saveObj["imageFilePath"] = imageFilePath_;

	// �F�̕ۑ�
	saveObj["color"] = { color_.x,color_.y,color_.z };

}

void UIProgressCircle::Load(json& loadObj)
{
	// �摜�t�@�C���p�X�̓ǂݍ���
	SetImage(loadObj["imageFilePath"]);

	// �F�̓ǂݍ���
	SetColor({ loadObj["color"][0],loadObj["color"][1],loadObj["color"][2] });
}

void UIProgressCircle::DrawData()
{
	// �ő�l�̐ݒ�
	ImGui::DragFloat("max:", &max_, 1.0f, 0.0f, FLT_MAX);

	// ���ݒl�̐ݒ�
	ImGui::DragFloat("now:", &now_, 1.0f, 0.0f, max_);

	// �摜�t�@�C���p�X�̐ݒ�
	ImGui::Text("image: %s", imageFilePath_.c_str()); ImGui::SameLine();
	if (ImGui::SmallButton("...")) SetImage(GetPNGFilePath());

	// �F�̐ݒ�
	ImGui::ColorEdit3("color:", &color_.x);
}

bool UIProgressCircle::SetImage(string _imageFilePath)
{
	// �摜�t�@�C���p�X��ݒ�
	imageFilePath_ = _imageFilePath;

	// �摜��ǂݍ���
	imageHandle_ = Image::Load(imageFilePath_);

	// �摜�̓ǂݍ��݂ɐ����������ǂ��� ��Ԃ�
	return imageHandle_ >= 0;
}


string UIProgressCircle::GetPNGFilePath()
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
			ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.fbx)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
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
			return "";
		}
	}

	// �t�@�C���p�X��Ԃ�
	return filePath;
}