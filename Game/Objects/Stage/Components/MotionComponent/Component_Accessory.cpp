#include "Component_Accessory.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"

Component_Accessory::Component_Accessory(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, ComponentType::Accessory, _parent), originalPosition_{}, originalRotate_{}, originalScale_{1,1,1},
	accessory_{}, accessoryModelHandle_{},bone_{ "mixamorig:LeftHand" }
{
	accessory_ = CreateStageObject(name_, holder_->GetModelFilePath(), holder_);
}

Component_Accessory::~Component_Accessory()
{
	accessory_->KillMe();
}

void Component_Accessory::Initialize()
{
}

void Component_Accessory::Update()
{
	auto playerHandle = holder_->GetModelHandle();
	auto transform = holder_->GetTransform();
	Model::SetTransform(playerHandle, transform);

	auto bonePos = Model::GetBonePosition(playerHandle, bone_);
	auto boneRot = Model::GetBoneRotation(playerHandle, bone_);
	auto boneScale = Model::GetBoneScale(playerHandle, bone_);

	accessory_->SetPosition(bonePos + originalPosition_);
	accessory_->SetRotate(boneRot + originalRotate_);
	accessory_->SetScale({ boneScale.x * originalScale_.x ,boneScale.y * originalScale_.y,boneScale.z * originalScale_.z });
}

void Component_Accessory::Release()
{
}

void Component_Accessory::DrawData()
{
	ImGui::DragFloat3("Original Position", &originalPosition_.x, .01f);
	ImGui::DragFloat3("Original Rotation", &originalRotate_.x, 1.f);
	ImGui::DragFloat3("Original Scale", &originalScale_.x, .01f);
	
	if (ImGui::Button("Load Model")) {
		//現在のカレントディレクトリを覚えておく
		char defaultCurrentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, defaultCurrentDir);
		{
			OPENFILENAME ofn; {
				TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
				ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
				ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
				ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
				ofn.lpstrFile[0] = '\0'; // 初期化
				ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
				ofn.lpstrFilter = TEXT("FBXファイル(*.fbx)\0*.fbx\0すべてのファイル(*.*)\0*.*\0"); 
				ofn.nFilterIndex = 1; // 初期選択するフィルター
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
				ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
			}

			// ファイルを選択するダイアログの表示
			if (GetOpenFileName(&ofn) == TRUE) {
				// ファイルパスを取得
				string path = ofn.lpstrFile;

				// カレントディレクトリからの相対パスを取得
				path = FileManager::GetAssetsRelativePath(path);

				// 文字列内の"\\"を"/"に置換
				FileManager::ReplaceBackslashes(path);

				// ディレクトリを戻す
				SetCurrentDirectory(defaultCurrentDir);

				ExchangeModel(path);
			}
			else {
				return;
			}
		}
	}

	ImGui::SameLine();
	ImGui::Text(Model::GetModelName(accessoryModelHandle_).c_str());
	ImGui::InputText(": Bone Name", bone_, sizeof(bone_));
}

void Component_Accessory::ExchangeModel(std::string filePath)
{
	accessoryModelHandle_ = Model::Load(filePath);

	modelPath_ = filePath;
	accessory_->SetModelHandle(accessoryModelHandle_);
}


void Component_Accessory::Load(json& _loadObj)
{
	string path{};

	if (_loadObj.contains("Accessory ModelPath")) accessoryModelHandle_ = Model::Load(_loadObj["Accessory ModelPath"].get<std::string>());
	else
	{

		char defaultCurrentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, defaultCurrentDir);
		{
			OPENFILENAME ofn; {
				TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
				ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
				ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
				ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
				ofn.lpstrFile[0] = '\0'; // 初期化
				ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
				ofn.lpstrFilter = TEXT("FBXファイル(*.fbx)\0*.fbx\0すべてのファイル(*.*)\0*.*\0");
				ofn.nFilterIndex = 1; // 初期選択するフィルター
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
				ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
			}

			// ファイルを選択するダイアログの表示
			if (GetOpenFileName(&ofn) == TRUE) {
				// ファイルパスを取得
				path = ofn.lpstrFile;

				// カレントディレクトリからの相対パスを取得
				path = FileManager::GetAssetsRelativePath(path);

				// 文字列内の"\\"を"/"に置換
				FileManager::ReplaceBackslashes(path);

				// ディレクトリを戻す
				SetCurrentDirectory(defaultCurrentDir);



			}
		}
	}

	if (_loadObj.contains("Accessory Original Position")) originalPosition_
		={ _loadObj["Accessory Original Position"][0].get<float>(),_loadObj["Accessory Original Position"][1].get<float>(),_loadObj["Accessory Original Position"][2].get<float>()};
	if (_loadObj.contains("Accessory Original Rotate")) originalRotate_
		= { _loadObj["Accessory Original Rotate"][0].get<float>(),_loadObj["Accessory Original Rotate"][1].get<float>(),_loadObj["Accessory Original Rotate"][2].get<float>() };
	if (_loadObj.contains("Accessory Original Scale"))originalScale_
		= { _loadObj["Accessory Original Scale"][0].get<float>(),_loadObj["Accessory Original Scale"][1].get<float>(),_loadObj["Accessory Original Scale"][2].get<float>() };

	if (_loadObj.contains("Accessory ModelPath"))
	{
		modelPath_ = _loadObj["Accessory ModelPath"].get<string>();
		ExchangeModel(modelPath_);
	}
}

void Component_Accessory::Save(json& _saveObj)
{
	_saveObj["Accessory ModelPath"] = modelPath_;

	_saveObj["Accessory Original Position"] = { originalPosition_.x,originalPosition_.y,originalPosition_.z };
	_saveObj["Accessory Original Rotate"] = { originalRotate_.x,originalRotate_.y,originalRotate_.z };
	_saveObj["Accessory Original Scale"] = { originalScale_.x,originalScale_.y,originalScale_.z };
}
