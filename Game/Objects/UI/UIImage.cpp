#include "UIImage.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"

UIImage::UIImage(string _name, UIObject* parent, int _layerNum)
	:UIObject(_name, UI_IMAGE, parent,_layerNum), imageHandle_(-1), imageFilePath_(""), isDraw_(true)
{
}

void UIImage::Initialize()
{
}

void UIImage::Update()
{
}

void UIImage::Draw()
{
	// 画像が読み込まれていない場合は処理を行わない
	if (imageHandle_ < 0 || isDraw_ == false )return;

    auto t = GetCalcTransform();

    Image::SetAlpha(imageHandle_, alpha_);
    Image::SetTransform(imageHandle_, t);
    Image::Draw(imageHandle_,fade_);
}

void UIImage::Release()
{
}

void UIImage::Save(json& saveObj)
{
	saveObj["imageFilePath_"] = imageFilePath_;
}

void UIImage::Load(json& loadObj)
{
	if (loadObj.contains("imageFilePath_")) {
		imageFilePath_ = loadObj["imageFilePath_"].get<string>();
		SetImage(imageFilePath_);
	}
}

void UIImage::DrawData()
{
    if (ImGui::TreeNode("imageFilePath_")) {

        ImGui::Text("imageFilePath_:%s", imageFilePath_.c_str());
        ImGui::SameLine();

        if (ImGui::SmallButton("...")) {
            //現在のカレントディレクトリを覚えておく
            char defaultCurrentDir[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

            // 追加するオブジェクトのモデルファイルパスを設定
            {
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
                    imageFilePath_ = ofn.lpstrFile;

                    // カレントディレクトリからの相対パスを取得
                    imageFilePath_ = FileManager::GetAssetsRelativePath(imageFilePath_);

                    // 文字列内の"\\"を"/"に置換
                    FileManager::ReplaceBackslashes(imageFilePath_);

                    // ディレクトリを戻す
                    SetCurrentDirectory(defaultCurrentDir);

                    // 画像を読み込む
                    SetImage(imageFilePath_);
                }
                else {
                    return;
                }
            }
        }
        ImGui::TreePop();
    }
}

void UIImage::SetImage(string _imageFilePath)
{
	imageFilePath_ = _imageFilePath;
	imageHandle_ = Image::Load(_imageFilePath);
	assert(imageHandle_ >= 0);
}
