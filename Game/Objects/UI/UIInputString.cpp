#include "UIInputString.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/ResourceManager/Text.h"
#include "../../../Engine/Global.h"

UIInputString::UIInputString(string _name, UIObject* parent, int _layerNum)
	:UIObject(_name, UI_INPUTSTRING, parent, _layerNum), currentInput_(""), isComplete_(false), size_(1.f)
	, pText_(nullptr), isOpenChangeFontWindow_(false)
{
}

void UIInputString::Initialize()
{

    // テキストオブジェクトを生成＆初期化
    pText_ = new Text();
    pText_->Initialize();

    // キーマッピングの初期化
    InitializeKeyMapping();

    /// 変形をロック
    LockScale();
}

void UIInputString::Update()
{
    // キー入力処理
    for (const auto& pair : keyToCharMap_) {
        if (Input::IsKeyDown(pair.first)) {
            currentInput_ += pair.second;
        }
    }

    // バックスペースキー処理
    if (Input::IsKeyDown(DIK_BACK) && !currentInput_.empty()) {
        currentInput_.pop_back();
    }

    // Enterキーが押されたら処理を終了
    if (Input::IsKeyDown(DIK_RETURN)) {
        isComplete_ = true;
    }

	// テキストの更新
	prevInput_ = currentInput_;
}

void UIInputString::Draw()
{
    // 表示サイズを計算
    pText_->SetScale(size_);
    
    // 表示位置を計算
    auto t = GetCalcTransform();

    float drawX = (t.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);
    float drawY = (t.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);

    // テキストを描画
    pText_->Draw(drawX, drawY, currentInput_.c_str(),fade_);

#ifdef _DEBUG

    // フォント変更ウィンドウを開く
    if (isOpenChangeFontWindow_)ChangeFontWindow();

#endif // _DEBUG
}

void UIInputString::Release()
{
}

void UIInputString::Save(json& saveObj)
{
    // 文字列を保存
    saveObj["text"] = currentInput_;

    // サイズを保存
    saveObj["size"] = size_;

    // フォントデータを保存
    saveObj["FontData"]["filePath"] = fontData_.filePath;
    saveObj["FontData"]["charSize"] = { REFERENCE_XMINT2(fontData_.charSize) };
    saveObj["FontData"]["rowLength"] = fontData_.rowLength;
}

void UIInputString::Load(json& loadObj)
{
    // 文字列を読み込み
    if (loadObj.contains("text")) currentInput_ = loadObj["text"].get<string>();

    // サイズを読み込み
	if (loadObj.contains("size")) size_ = loadObj["size"].get<float>();

    // フォントデータを読み込み
    if (loadObj.contains("FontData")) {
        if (loadObj["FontData"].contains("filePath"))fontData_.filePath = loadObj["FontData"]["filePath"].get<string>();
        if (loadObj["FontData"].contains("charSize"))fontData_.charSize = { loadObj["FontData"]["charSize"][0].get<int>(),loadObj["FontData"]["charSize"][1].get<int>(), };
        if (loadObj["FontData"].contains("rowLength"))fontData_.rowLength = loadObj["FontData"]["rowLength"].get<int>();

        // フォントデータを設定
        pText_->Initialize(fontData_.filePath.c_str(), fontData_.charSize.x, fontData_.charSize.y, fontData_.rowLength);
    }
}

void UIInputString::DrawData()
{
	if (ImGui::TreeNode("value")) {

		// 入力された文字列を表示
		ImGui::Text("currentInput : %s", currentInput_.c_str());

		// 入力完了フラグを表示
		ImGui::Text("isComplete : %s", isComplete_ ? "true" : "false");ImGui::SameLine();
		ImGui::Checkbox("##isComplete", &isComplete_);

		ImGui::TreePop();
	}

    if (ImGui::TreeNode("draw")) {

		// テキストの描画サイズを変更
		ImGui::DragFloat("size", &size_, 0.01f);

        // フォントを変更
        ImGui::Text("fontFilePath : %s", fontData_.filePath.c_str());
        ImGui::SameLine();

        // フォントを選択
        if (ImGui::SmallButton("..."))isOpenChangeFontWindow_ = true;

        // フォントデータを表示
        ImGui::Text("charSize : %d, %d", fontData_.charSize.x, fontData_.charSize.y);
        ImGui::Text("rowLength : %d", fontData_.rowLength);

		ImGui::TreePop();
    }
}

void UIInputString::ChangeFontWindow()
{
    ImGui::Begin("ChangeFont", &isOpenChangeFontWindow_); {

        // フォントファイルを表示
        static bool isFileExist = false;
        static string filePath = "none";

        // ファイルがない場合は赤文字で表示
        if (isFileExist == false)ImGui::TextColored(ImVec4(1, 0, 0, 1), "File Not Found");
        else ImGui::Text("filePath : %s", filePath.c_str());
        ImGui::SameLine();

        // フォントファイルを選択
        if (ImGui::Button("select"))isFileExist = (GetFontFilePathFromExplorer(filePath));

        // フォントのサイズを変更
        ImGui::DragInt2("charSize", &fontData_.charSize.x, 1.0f, 1, 100);
        ImGui::DragInt("rowLength", &fontData_.rowLength, 1.0f, 1, 100);

        // ファイルが存在する場合
        if (isFileExist) if (ImGui::Button("confirm")) {

            // フォントデータを設定
            fontData_.filePath = filePath;
            pText_->Initialize(fontData_.filePath.c_str(), fontData_.charSize.x, fontData_.charSize.y, fontData_.rowLength);

            // ファイルが存在する場合はウィンドウを閉じる
            isOpenChangeFontWindow_ = false;

            // 静的変数を初期化
            isFileExist = false;
            filePath = "none";
        }
    }
    ImGui::End();
}

bool UIInputString::GetFontFilePathFromExplorer(string& _filePath) const
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



void UIInputString::InitializeKeyMapping()
{
    keyToCharMap_[DIK_A] = 'a';
    keyToCharMap_[DIK_B] = 'b';
    keyToCharMap_[DIK_C] = 'c';
    keyToCharMap_[DIK_D] = 'd';
    keyToCharMap_[DIK_E] = 'e';
    keyToCharMap_[DIK_F] = 'f';
    keyToCharMap_[DIK_G] = 'g';
    keyToCharMap_[DIK_H] = 'h';
    keyToCharMap_[DIK_I] = 'i';
    keyToCharMap_[DIK_J] = 'j';
    keyToCharMap_[DIK_K] = 'k';
    keyToCharMap_[DIK_L] = 'l';
    keyToCharMap_[DIK_M] = 'm';
    keyToCharMap_[DIK_N] = 'n';
    keyToCharMap_[DIK_O] = 'o';
    keyToCharMap_[DIK_P] = 'p';
    keyToCharMap_[DIK_Q] = 'q';
    keyToCharMap_[DIK_R] = 'r';
    keyToCharMap_[DIK_S] = 's';
    keyToCharMap_[DIK_T] = 't';
    keyToCharMap_[DIK_U] = 'u';
    keyToCharMap_[DIK_V] = 'v';
    keyToCharMap_[DIK_W] = 'w';
    keyToCharMap_[DIK_X] = 'x';
    keyToCharMap_[DIK_Y] = 'y';
    keyToCharMap_[DIK_Z] = 'z';
    keyToCharMap_[DIK_0] = '0';
    keyToCharMap_[DIK_1] = '1';
    keyToCharMap_[DIK_2] = '2';
    keyToCharMap_[DIK_3] = '3';
    keyToCharMap_[DIK_4] = '4';
    keyToCharMap_[DIK_5] = '5';
    keyToCharMap_[DIK_6] = '6';
    keyToCharMap_[DIK_7] = '7';
    keyToCharMap_[DIK_8] = '8';
    keyToCharMap_[DIK_9] = '9';
    keyToCharMap_[DIK_SPACE] = ' ';
}
