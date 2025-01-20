#include "UIButton.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/Global.h"
#include "UIPanel.h"

using namespace FileManager;


UIButton::UIButton(string _name, UIObject* parent , int _layerNum)
	: UIObject(_name, UIType::UI_BUTTON, parent, _layerNum), imageHandle_(-1), 
	imageFilePath_(), arrayPlaceX_(1), arrayPlaceY_(1),isSetShader_(false)
{
}

UIButton::~UIButton()
{
	UIPanel::GetInstance()->RemoveButtonFromArray(this);
}

void UIButton::Initialize()
{
	LockRotate();
}

void UIButton::Update()
{
	if (imageHandle_ < 0)return;

	if (isSetShader_)
	{
		isSetShader_ = false;
		return;
	}

	static UIPanel* panel = UIPanel::GetInstance();
	// �}�E�X�̍��W���擾
	XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

	// �}�E�X�̍��W���摜�̍��W�ɕϊ�
	ConvertToImageCoordinates(mousePos);

	if (isChangeImage_) {
		SetShader(Direct3D::SHADER_BUTTON_NOTSELECT);
		if (IsMouseOver(mousePos) || (panel->GetSelectingButton() != nullptr && panel->GetSelectingButton() == this)) {
			imageHandle_ = selectImageHandle_;
		}
		else {
			imageHandle_ = defaultImageHandle_;
		}
	}
	else {
		shaderType_ = IsMouseOver(mousePos) ? Direct3D::SHADER_BUTTON_SELECT : Direct3D::SHADER_BUTTON_NOTSELECT;

		if (panel->GetSelectingButton() != nullptr)
			if (panel->GetSelectingButton() == this) {
				shaderType_ = Direct3D::SHADER_BUTTON_SELECT;
			}
	}


	Image::SetAlpha(imageHandle_, 256);
}

void UIButton::Draw()
{
	// �摜���ǂݍ��܂�Ă��Ȃ��ꍇ�͏������s��Ȃ�
	if (imageHandle_ < 0)return;

	auto t = this->GetCalcTransform();

	Image::SetTransform(imageHandle_,t);
	Image::Draw(imageHandle_,shaderType_);
}

void UIButton::Release()
{
}

void UIButton::Save(json& saveObj)
{
	saveObj["imageFilePath_"] = imageFilePath_;
	saveObj["defaultImageFilePath_"] = defaultImageFilePath_;
	saveObj["selectImageFilePath_"] = selectImageFilePath_;
	saveObj["ArrayPlace_X"] = arrayPlaceX_;
	saveObj["ArrayPlace_Y"] = arrayPlaceY_;
}

void UIButton::Load(json& loadObj)
{
	if (loadObj.contains("imageFilePath_")) {
		if (loadObj["imageFilePath_"].get<string>() != "") {
			imageFilePath_ = loadObj["imageFilePath_"].get<string>();
			SetImage(imageFilePath_);
		}
	}
	if(loadObj.contains("defaultImageFilePath_")) {
		if (loadObj["defaultImageFilePath_"].get<string>() != "") {
			defaultImageFilePath_ = loadObj["defaultImageFilePath_"].get<string>();
			SetDefaultImage(defaultImageFilePath_);
		}
	}
	if (loadObj.contains("selectImageFilePath_")) {
		if (loadObj["selectImageFilePath_"].get<string>() != "") {
			selectImageFilePath_ = loadObj["selectImageFilePath_"].get<string>();
			SetSelectImage(selectImageFilePath_);
		}
	}
	if (loadObj.contains("ArrayPlace_X")) arrayPlaceX_ = loadObj["ArrayPlace_X"].get<int>();
	if (loadObj.contains("ArrayPlace_Y")) arrayPlaceY_ = loadObj["ArrayPlace_Y"].get<int>();
}

void UIButton::DrawData()
{

	if (ImGui::TreeNode("imageFilePath_")) {

		ImGui::Text("default_:%s", defaultImageFilePath_.c_str());
		ImGui::Text("handle :%d", defaultImageHandle_);

		ImGui::SameLine();
		if (ImGui::SmallButton("default")) {
			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
			{
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
					defaultImageFilePath_ = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					defaultImageFilePath_ = FileManager::GetAssetsRelativePath(defaultImageFilePath_);

					// ���������"\\"��"/"�ɒu��
					FileManager::ReplaceBackslashes(defaultImageFilePath_);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);

					// �摜��ǂݍ���
					SetDefaultImage(defaultImageFilePath_);
				}
				else {
					return;
				}
			}
		}
		
		ImGui::Text("selectImageHandle_:%s", selectImageFilePath_.c_str());
		ImGui::Text("handle :%d", selectImageHandle_);
		ImGui::SameLine();
		if (ImGui::SmallButton("select")) {
			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
			{
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
					selectImageFilePath_ = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					selectImageFilePath_ = FileManager::GetAssetsRelativePath(selectImageFilePath_);

					// ���������"\\"��"/"�ɒu��
					FileManager::ReplaceBackslashes(selectImageFilePath_);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);

					// �摜��ǂݍ���
					SetSelectImage(selectImageFilePath_);
				}
				else {
					return;
				}
			}
		}
		ImGui::TreePop();
	}

	
	if (ImGui::TreeNode("Data"))
	{
		ImGui::Text("OnClick: %s", OnClick() ? "true" : "false");

		XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };
		ConvertToImageCoordinates(mousePos);
		if (imageHandle_ >= 0)ImGui::Text("IsMouseOver: %s", IsMouseOver(mousePos) ? "true" : "false");

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("ButtonArray"))
	{
		int tempX = arrayPlaceX_;
		int tempY = arrayPlaceY_;

		ImGui::DragInt(" X ", &tempX, 1,
			(std::numeric_limits<decltype(arrayPlaceX_)>::min)(), (std::numeric_limits<decltype(arrayPlaceX_)>::max)() );

		ImGui::DragInt(" Y ", &tempY, 1,
			(std::numeric_limits<decltype(arrayPlaceY_)>::min)(), (std::numeric_limits<decltype(arrayPlaceY_)>::max)() );

		SetArrayPlace(tempX, tempY);

		ImGui::TreePop();
	}
}

void UIButton::SetArrayPlace(int x, int y)
{
	arrayPlaceX_ = x;
	arrayPlaceY_ = y;
}

void UIButton::GetArrayPlace(int* x, int* y) const
{
	*x = arrayPlaceX_;
	*y = arrayPlaceY_;
}

void UIButton::SetShader(Direct3D::SHADER_TYPE type)
{
	shaderType_ = type;

	isSetShader_ = true;
}

Direct3D::SHADER_TYPE UIButton::GetShader() const
{
	return shaderType_;
}

bool UIButton::GetIsMouseOverThisButton()
{
	// �}�E�X�̍��W���擾
	XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

	// �}�E�X�̍��W���摜�̍��W�ɕϊ�
	ConvertToImageCoordinates(mousePos);
	return IsMouseOver(mousePos);
}

void UIButton::SetImage(string _imageFilePath)
{
	imageFilePath_ = _imageFilePath;
	imageHandle_ = Image::Load(_imageFilePath);
}

void UIButton::SetSelectImage(string _imageFilePath)
{
	selectImageFilePath_ = _imageFilePath;
	selectImageHandle_ = Image::Load(_imageFilePath);
	isChangeImage_ = true;
}

void UIButton::SetDefaultImage(string _imageFilePath)
{
	defaultImageFilePath_ = _imageFilePath;
	defaultImageHandle_ = Image::Load(_imageFilePath);
}

bool UIButton::OnClick()
{
	// �}�E�X�̍��W���擾
	XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

	// �}�E�X�̍��W���摜�̍��W�ɕϊ�
	ConvertToImageCoordinates(mousePos);

	//�}�E�X�̃|�W�V�������摜�̗̈�ɓ����Ă��邩���N���b�N�������ꂽ��
	return IsMouseOver(mousePos) && Input::IsMouseButtonDown(0) && this->isVisible_;
}

bool UIButton::IsMouseOver(XMFLOAT2 _mousePosition)
{
	// �摜���ǂݍ��܂�Ă��Ȃ��ꍇ�͏������s��Ȃ�
	if (imageHandle_ < 0)return false;

	// ��ʂ̃T�C�Y���擾
	int scWidth = Direct3D::screenWidth_;
	int scHeight = Direct3D::screenHeight_;

	// �}�E�X�J�[�\���̍��W���擾
	XMFLOAT2 imageSize = Image::GetSize(imageHandle_); {
		// �摜�̃T�C�Y����ʃT�C�Y�ɍ��킹��

		imageSize.x = imageSize.x / scWidth;
		imageSize.y = imageSize.y / scHeight;
	}

	// �摜�̔����̃T�C�Y���擾
	float imageHelfWidth = imageSize.x /2.f;
	float imageHelfHeight = imageSize.y /2.f;

	// �摜�̒��S���W���擾
	auto&& calcform = GetCalcTransform();

	XMFLOAT2 center = { calcform.position_.x,calcform.position_.y };

	// �摜�̊g�嗦���擾
	XMFLOAT2 scale = { calcform.scale_.x,calcform.scale_.y };

	// ����p�͈̔͂̒l���擾
	float top = center.y - (imageHelfHeight * 2.f * scale.y);       // �摜�̒��S�����ɉ摜�̍����̔����̋���
	float bottom = center.y + (imageHelfHeight * 2.f * scale.y);    // �摜�̒��S���牺�ɉ摜�̍����̔����̋���
	float left = center.x - (imageHelfWidth * 2.f * scale.x);       // �摜�̒��S���獶�ɉ摜�̕��̔����̋���
	float right = center.x + (imageHelfWidth * 2.f * scale.x);      // �摜�̒��S����E�ɉ摜�̕��̔����̋���
	
	// ����͈͓��Ƀ}�E�X�J�[�\���������Ă��邩�ǂ�����Ԃ�
	return (_mousePosition.x >= left && _mousePosition.x <= right && _mousePosition.y >= top && _mousePosition.y <= bottom);
}

void UIButton::ConvertToImageCoordinates(XMFLOAT2& _position)
{
	int scWidth = Direct3D::screenWidth_;
	int scHeight = Direct3D::screenHeight_;

	if (Direct3D::isFullScreen_)
		Direct3D::GetFullScreenSize(scWidth, scHeight);

#ifdef _DEBUG
	scWidth = scWidth * 0.7;
	scHeight = scHeight * 0.7;
#endif // _DEBUG

	// �}�E�X�̍��W���摜�̍��W�ɕϊ�
	_position.x = (float)(_position.x * 2.0f) / (float)scWidth - 1.0f;
	_position.y = 1.0f - (float)(_position.y * 2.0f) / (float)scHeight;
}
