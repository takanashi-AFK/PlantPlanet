#include "UIText.h"
#include "../../../Engine/ResourceManager/Text.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/Global.h"

UIText::UIText(string _name, UIObject* parent,int _layerNum)
	: UIObject(_name, UI_TEXT, parent, _layerNum), pText_(nullptr), drawText_(""), size_(1.f), intNum_(nullptr)
{
}

void UIText::Initialize()
{
	// �e�L�X�g�I�u�W�F�N�g�𐶐���������
	pText_ = new Text;
	pText_->Initialize();

	// �t�H���g�f�[�^��������
	fontData_.filePath = pText_->GetFileName();
	fontData_.charSize = { (int)pText_->GetWidth(),(int)pText_->GetHeight()};
	fontData_.rowLength = pText_->GetRowLength();

	//�e�L�X�g�̕ό`�����b�N
	LockScale(); /* & */ LockRotate();
}

void UIText::Update()
{
#ifdef _DEBUG

	// �t�H���g�ύX�E�B���h�E���J��
	if(isOpenChangeFontWindow_)ChangeFontWindow();

#endif // _DEBUG
}

void UIText::Draw()
{
	// �\���T�C�Y���v�Z
	pText_->SetScale(size_);
	auto t = GetCalcTransform();

	// �\���ʒu���v�Z
	float drawX = (t.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);		
	float drawY = (t.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);
	
	// �e�L�X�g��`��
	if (intNum_ == nullptr)pText_->Draw(drawX, drawY, drawText_.c_str(),alpha_);
	else pText_->Draw(drawX, drawY,*intNum_,alpha_);
}

void UIText::Release()
{
	// �e�L�X�g�I�u�W�F�N�g�̉��
	pText_->Release();
}

void UIText::Save(json& saveObj)
{
	// �������ۑ�
	saveObj["text"] = drawText_;

	// �T�C�Y��ۑ�
	saveObj["size"] = size_;

	// �t�H���g�f�[�^��ۑ�
	saveObj["FontData"]["filePath"] = fontData_.filePath;
	saveObj["FontData"]["charSize"] = { REFERENCE_XMINT2(fontData_.charSize) };
	saveObj["FontData"]["rowLength"] = fontData_.rowLength;
}

void UIText::Load(json& loadObj)
{
	// �������ǂݍ���
	if (loadObj.contains("text"))drawText_ = loadObj["text"].get<string>();
	
	// �T�C�Y��ǂݍ���
	if (loadObj.contains("size"))size_ = loadObj["size"].get<float>();
	
	// �t�H���g�f�[�^��ǂݍ���
	if (loadObj.contains("FontData")) {
		if(loadObj["FontData"].contains("filePath"))fontData_.filePath = loadObj["FontData"]["filePath"].get<string>();
		if (loadObj["FontData"].contains("charSize"))fontData_.charSize = { loadObj["FontData"]["charSize"][0].get<int>(),loadObj["FontData"]["charSize"][1].get<int>(), };
		if (loadObj["FontData"].contains("rowLength"))fontData_.rowLength = loadObj["FontData"]["rowLength"].get<int>();

		// �t�H���g�f�[�^��ݒ�
		pText_->Initialize(fontData_.filePath.c_str(), fontData_.charSize.x, fontData_.charSize.y, fontData_.rowLength);
	}
}

void UIText::DrawData()
{
	if (ImGui::TreeNode("draw"))
	{
		// �e�L�X�g�̕`��T�C�Y��ύX
		ImGui::DragFloat("size", &size_, 0.01f);

		// �t�H���g��ύX
		ImGui::Text("fontFilePath : %s", fontData_.filePath.c_str());
		ImGui::SameLine();

		// �t�H���g��I��
		if (ImGui::SmallButton("..."))isOpenChangeFontWindow_ = true;

		// �t�H���g�f�[�^��\��
		ImGui::Text("charSize : %d, %d", fontData_.charSize.x, fontData_.charSize.y);
		ImGui::Text("rowLength : %d", fontData_.rowLength);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("value")) {

		// �e�L�X�g�̕`����e��ύX
		ImGui::Text("Current Text : %s", drawText_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New Text...", buffer, IM_ARRAYSIZE(buffer)))this->drawText_ = buffer;

		ImGui::TreePop();
	}
}

void UIText::ChangeFontWindow()
{
	ImGui::Begin("ChangeFont",&isOpenChangeFontWindow_); {

		// �t�H���g�t�@�C����\��
		static bool isFileExist = false;
		static string filePath = "none";
		
		// �t�@�C�����Ȃ��ꍇ�͐ԕ����ŕ\��
		if (isFileExist == false)ImGui::TextColored(ImVec4(1, 0, 0, 1), "File Not Found");
		else ImGui::Text("filePath : %s", filePath.c_str());
		ImGui::SameLine();

		// �t�H���g�t�@�C����I��
		if (ImGui::Button("select"))isFileExist = (GetFontFilePathFromExplorer(filePath));

		// �t�H���g�̃T�C�Y��ύX
		ImGui::DragInt2("charSize", &fontData_.charSize.x, 1.0f, 1, 100);
		ImGui::DragInt("rowLength", &fontData_.rowLength, 1.0f, 1, 100);

		// �t�@�C�������݂���ꍇ
		if (isFileExist) if (ImGui::Button("confirm")) {

			// �t�H���g�f�[�^��ݒ�
			fontData_.filePath = filePath;
			pText_->Initialize(fontData_.filePath.c_str(), fontData_.charSize.x, fontData_.charSize.y, fontData_.rowLength);

			// �t�@�C�������݂���ꍇ�̓E�B���h�E�����
			isOpenChangeFontWindow_ = false;

			// �ÓI�ϐ���������
			isFileExist = false;
			filePath = "none";
		}
	}
	ImGui::End();
}

bool UIText::GetFontFilePathFromExplorer(string& _filePath) const
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
