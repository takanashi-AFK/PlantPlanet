
#include "UIProgressBar.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../Stage/Stage.h"
#include "../../../Game/Objects/Stage/StageObject.h"
#include "../../../Game/Objects/Stage/Components/Component.h"

namespace {
    const string DEFAULT_FRAME_IMAGE = "Images/defaults/Bar_Frame.png";
    const string DEFAULT_GAUGE_IMAGE = "Images/defaults/Bar_Gauge.png";
}

UIProgressBar::UIProgressBar(string _name, UIObject* parent,int _layerNum)
    :UIObject(_name, UIType::UI_PROGRESSBAR, parent, _layerNum)
{
}

void UIProgressBar::Initialize()
{
    // フレームを設定
    {
        // フレーム画像を読み込む
        frameImage_.Load(DEFAULT_FRAME_IMAGE);

        // フレームのtransformを設定 ※デフォルトの位置、回転、拡大縮小
        frameImage_.transform_ = transform_;
    }

    // ゲージを設定
    {
        // ゲージ画像を読み込む
        gaugeImage_.Load(DEFAULT_GAUGE_IMAGE);

        // ゲージのtransformを設定 ※デフォルトの位置、回転、拡大縮小
        gaugeImage_.transform_ = transform_;

        // ゲージの色を設定
        gaugeImage_.color_ = { 1.0f,1.0f,1.0f };
    }
}

void UIProgressBar::Update()
{
    // 描画用transformをオブジェクトのtransformで更新
    frameImage_.transform_ = GetCalcTransform();
    gaugeImage_.transform_ = GetCalcTransform();

    // ゲージのスケールを参照した値を基に設定
    {
		// ゲージのスケールを設定 ※現在値/最大値
        gaugeImage_.transform_.scale_.x = frameImage_.transform_.scale_.x * (now_ / max_);

		// ゲージのスケールが0以下の場合は、0に設定
		if (gaugeImage_.transform_.scale_.x <= 0)gaugeImage_.transform_.scale_.x = 0;
	}
}

void UIProgressBar::Draw()
{
    // ゲージ画像が読み込まれている場合
  
    if (gaugeImage_.IsAvailable()) {

        Transform LoaclTransform = gaugeImage_.transform_;
        auto t = GetCalcTransform(LoaclTransform);

		// ゲージの画像を描画
        Image::SetTransform(gaugeImage_.handle_, t);
        Image::Draw(gaugeImage_.handle_, Direct3D::SHADER_BAR, gaugeImage_.color_);
    }

    // フレーム画像が読み込まれている場合
    if (frameImage_.IsAvailable()) {

        Transform LoaclTransform = frameImage_.transform_;
        auto t = GetCalcTransform(LoaclTransform);

        // フレームの画像を描画
		Image::SetTransform(frameImage_.handle_, t);
		Image::Draw(frameImage_.handle_);
    }
}

void UIProgressBar::Release()
{

}

void UIProgressBar::Save(json& saveObj)
{
    // ゲージ画像の情報を保存
    saveObj["gaugeImage_"]["filePath_"] = gaugeImage_.filePath_;
    saveObj["gaugeImage_"]["color_"] = { REFERENCE_XMFLOAT3(gaugeImage_.color_)};
    
    // フレーム画像の情報を保存
    saveObj["frameImage_"]["filePath_"] = frameImage_.filePath_;
}

void UIProgressBar::Load(json& loadObj)
{
    // ゲージ画像の情報を読込
    if (loadObj.contains("gaugeImage_")) {
		
        // ゲージ画像を読み込む
        if (loadObj["gaugeImage_"].contains("filePath_")) gaugeImage_.Load(loadObj["gaugeImage_"]["filePath_"].get<string>());

        // ゲージの色を取得
        if (loadObj["gaugeImage_"].contains("color_"))gaugeImage_.color_ = { loadObj["gaugeImage_"]["color_"][0].get<float>(),loadObj["gaugeImage_"]["color_"][1].get<float>(), loadObj["gaugeImage_"]["color_"][2].get<float>() };
	}   

    // フレーム画像の情報を読込
    if (loadObj.contains("frameImage_")) {

        // フレーム画像を読み込む
        if (loadObj["frameImage_"].contains("filePath_"))frameImage_.Load(loadObj["frameImage_"]["filePath_"].get<string>());
	}
}

void UIProgressBar::DrawData()
{
    // 描画情報の設定
    if (ImGui::TreeNode("draw")) {
        
        // フレームの設定
        if(ImGui::TreeNode("frame")){

			// フレーム画像の変更
			ImGui::Text("image : %s", frameImage_.filePath_.c_str()); ImGui::SameLine();
			if (ImGui::SmallButton("...")) {

                // 画像ファイルを選択して読み込む
				string inputFilePath{};
				if (GetImageFilePathFromExplorer(inputFilePath)) frameImage_.Load(inputFilePath);
			}

			ImGui::TreePop();
		}

        // ゲージの設定
        if (ImGui::TreeNode("gauge")) {

			// ゲージ画像の変更
			ImGui::Text("image : %s", gaugeImage_.filePath_.c_str()); ImGui::SameLine();
			if (ImGui::SmallButton("...")) {

                // 画像ファイルを選択して読み込む
                string inputFilePath{};
                if (GetImageFilePathFromExplorer(inputFilePath)) gaugeImage_.Load(inputFilePath);
			}

			// ゲージの色を設定
			ImGui::ColorEdit3("color", &gaugeImage_.color_.x);

			ImGui::TreePop();
		}
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("value")) {
        
		// 最大値と現在値を設定
		ImGui::DragFloat("max", &max_);
		ImGui::DragFloat("now", &now_);

		ImGui::TreePop();
    }
}

bool UIProgressBar::GetImageFilePathFromExplorer(string& _filePath) const
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
            ofn.lpstrFilter = TEXT("PNGファイル(*.png)\0*.png\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
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
            // ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
			return false;
        }
    }

    _filePath = filePath;
    return true;

}

bool ProgressBarImage::Load(string _filepath)
{
    // ファイルパスを設定
    filePath_ = _filepath;

    // 画像の読み込み
    handle_ = Image::Load(_filepath);

    // 画像の読み込みに成功したかどうかを返す
    return handle_ >= 0;
}

bool ProgressBarImage::IsAvailable() const
{
    // 画像のハンドルが有効かどうかを返す
	return handle_ >= 0;
}
