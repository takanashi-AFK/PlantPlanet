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
	// テキストオブジェクトを生成＆初期化
	pText_ = new Text;
	pText_->Initialize();

	// フォントデータを初期化
	fontData_.filePath = pText_->GetFileName();
	fontData_.charSize = { (int)pText_->GetWidth(),(int)pText_->GetHeight()};
	fontData_.rowLength = pText_->GetRowLength();

	//テキストの変形をロック
	LockScale(); /* & */ LockRotate();
}

void UIText::Update()
{
#ifdef _DEBUG

	// フォント変更ウィンドウを開く
	if(isOpenChangeFontWindow_)ChangeFontWindow();

#endif // _DEBUG
}

void UIText::Draw()
{
	// 表示サイズを計算
	pText_->SetScale(size_);
	auto t = GetCalcTransform();

	// 表示位置を計算
	float drawX = (t.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);		
	float drawY = (t.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);
	
	// テキストを描画
	if (intNum_ == nullptr)pText_->Draw(drawX, drawY, drawText_.c_str(),alpha_);
	else pText_->Draw(drawX, drawY,*intNum_,alpha_);
}

void UIText::Release()
{
	// テキストオブジェクトの解放
	pText_->Release();
}

void UIText::Save(json& saveObj)
{
	// 文字列を保存
	saveObj["text"] = drawText_;

	// サイズを保存
	saveObj["size"] = size_;

	// フォントデータを保存
	saveObj["FontData"]["filePath"] = fontData_.filePath;
	saveObj["FontData"]["charSize"] = { REFERENCE_XMINT2(fontData_.charSize) };
	saveObj["FontData"]["rowLength"] = fontData_.rowLength;
}

void UIText::Load(json& loadObj)
{
	// 文字列を読み込み
	if (loadObj.contains("text"))drawText_ = loadObj["text"].get<string>();
	
	// サイズを読み込み
	if (loadObj.contains("size"))size_ = loadObj["size"].get<float>();
	
	// フォントデータを読み込み
	if (loadObj.contains("FontData")) {
		if(loadObj["FontData"].contains("filePath"))fontData_.filePath = loadObj["FontData"]["filePath"].get<string>();
		if (loadObj["FontData"].contains("charSize"))fontData_.charSize = { loadObj["FontData"]["charSize"][0].get<int>(),loadObj["FontData"]["charSize"][1].get<int>(), };
		if (loadObj["FontData"].contains("rowLength"))fontData_.rowLength = loadObj["FontData"]["rowLength"].get<int>();

		// フォントデータを設定
		pText_->Initialize(fontData_.filePath.c_str(), fontData_.charSize.x, fontData_.charSize.y, fontData_.rowLength);
	}
}

void UIText::DrawData()
{
	if (ImGui::TreeNode("draw"))
	{
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

	if (ImGui::TreeNode("value")) {

		// テキストの描画内容を変更
		ImGui::Text("Current Text : %s", drawText_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New Text...", buffer, IM_ARRAYSIZE(buffer)))this->drawText_ = buffer;

		ImGui::TreePop();
	}
}

void UIText::ChangeFontWindow()
{
	ImGui::Begin("ChangeFont",&isOpenChangeFontWindow_); {

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

bool UIText::GetFontFilePathFromExplorer(string& _filePath) const
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
