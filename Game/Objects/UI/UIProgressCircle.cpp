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
	// 画像を読み込む
	SetImage(DEFAULT_IMAGE);

	// 色を設定
	color_ = { 0.0f,0.0f,1.0f };

	// 現在値を最大値で初期化
	now_ = max_;
}

void UIProgressCircle::Update()
{
	// 現在値と最大値を基に進行度を計算
	float progress = now_ / max_;

	// 進行度を基に終了角度を計算
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
	// 画像ファイルパスの保存
	saveObj["imageFilePath"] = imageFilePath_;

	// 色の保存
	saveObj["color"] = { color_.x,color_.y,color_.z };

}

void UIProgressCircle::Load(json& loadObj)
{
	// 画像ファイルパスの読み込み
	SetImage(loadObj["imageFilePath"]);

	// 色の読み込み
	SetColor({ loadObj["color"][0],loadObj["color"][1],loadObj["color"][2] });
}

void UIProgressCircle::DrawData()
{
	// 最大値の設定
	ImGui::DragFloat("max:", &max_, 1.0f, 0.0f, FLT_MAX);

	// 現在値の設定
	ImGui::DragFloat("now:", &now_, 1.0f, 0.0f, max_);

	// 画像ファイルパスの設定
	ImGui::Text("image: %s", imageFilePath_.c_str()); ImGui::SameLine();
	if (ImGui::SmallButton("...")) SetImage(GetPNGFilePath());

	// 色の設定
	ImGui::ColorEdit3("color:", &color_.x);
}

bool UIProgressCircle::SetImage(string _imageFilePath)
{
	// 画像ファイルパスを設定
	imageFilePath_ = _imageFilePath;

	// 画像を読み込む
	imageHandle_ = Image::Load(imageFilePath_);

	// 画像の読み込みに成功したかどうか を返す
	return imageHandle_ >= 0;
}


string UIProgressCircle::GetPNGFilePath()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 追加するオブジェクトのモデルファイルパスを設定
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("PNGファイル(*.fbx)\0*.png\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = FileManager::GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			FileManager::ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return "";
		}
	}

	// ファイルパスを返す
	return filePath;
}