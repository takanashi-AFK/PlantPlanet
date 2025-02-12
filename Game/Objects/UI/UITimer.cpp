#include "UITimer.h"

// インクルード
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Text.h"


UITimer::UITimer(string _name, UIObject* parent, int _layerNum)
	: UIObject(_name, UI_TIMER, parent, _layerNum), max_(0), count_(0), isActive_(false), pText_(nullptr), size_(1.f), isOpenChangeFontWindow_(false)
{
}

void UITimer::Initialize()
{
	// テキストオブジェクトを生成＆初期化
	pText_ = new Text();
	pText_->Initialize();

	// ゲージの画像を読み込む
	circleGauge_.Load("Images/defaults/Timer_Gauge.png");
}

void UITimer::Update()
{
	if (isActive_)count_--;  // カウントダウン

	// カウンターが0になったら終了
	if (count_ <= 0)EndTimer();

	// カウントダウンに応じてゲージの進行度を計算
	float progress = (count_ > 0) ? (count_ / max_) : 0.0f;  // 進行割合を計算
	circleGauge_.Update(progress);  // 進行割合を基にゲージを更新
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
	// 最大値の保存
	saveObj["max"] = max_;

	// テキストサイズの保存
	saveObj["size"] = size_;

	// 描画方法の保存
	saveObj["drawType"] = drawType_;

	// Circlegaugeのファイルパスを保存
	saveObj["circleGaugeImage"] = circleGauge_.imageFilePath;

	// フォントデータを保存
	saveObj["FontData"]["filePath"] = fontData_.filePath;
	saveObj["FontData"]["charSize"] = { REFERENCE_XMINT2(fontData_.charSize) };
	saveObj["FontData"]["rowLength"] = fontData_.rowLength;
}

void UITimer::Load(json& loadObj)
{
	// 最大値の読み込み
	if(loadObj.contains("max"))max_ = loadObj["max"];
	count_ = max_;

	// テキストサイズの読み込み
	if(loadObj.contains("size"))size_ = loadObj["size"];

	// 描画方法の読み込み
	if(loadObj.contains("drawType"))drawType_ = (TIMER_DRAW_TYPE)loadObj["drawType"];

	// Circlegaugeのファイルパスを読み込み
	if(loadObj.contains("circleGaugeImage"))circleGauge_.Load(loadObj["circleGaugeImage"]);

	if (loadObj.contains("FontData")) {
		if (loadObj["FontData"].contains("filePath"))fontData_.filePath = loadObj["FontData"]["filePath"].get<string>();
		if (loadObj["FontData"].contains("charSize"))fontData_.charSize = { loadObj["FontData"]["charSize"][0].get<int>(),loadObj["FontData"]["charSize"][1].get<int>(), };
		if (loadObj["FontData"].contains("rowLength"))fontData_.rowLength = loadObj["FontData"]["rowLength"].get<int>();

		// フォントデータを設定
		pText_->Initialize(fontData_.filePath.c_str(), fontData_.charSize.x, fontData_.charSize.y, fontData_.rowLength);
	}
}

void UITimer::DrawData()
{
	// 描画の設定
	if (ImGui::TreeNode("draw")) {

		// 描画タイプの設定
		ImGui::Text("draw type");
		ImGui::RadioButton("text", (int*)&drawType_, TEXT);
		ImGui::SameLine();
		ImGui::RadioButton("circle gauge", (int*)&drawType_, CIRCLE_GAUGE);
		if(ImGui::Button("change font")) isOpenChangeFontWindow_ = true;

		switch (drawType_)
		{
		case TEXT:
			// テキストの設定
			ImGui::DragFloat("size", &size_, 0.01f, 0.1f, 10.f);
			break;
		case CIRCLE_GAUGE:
			// ゲージ画像の変更
			ImGui::Text("image : %s", circleGauge_.imageFilePath.c_str()); ImGui::SameLine();
			if (ImGui::SmallButton("...")) {

				// 画像ファイルを選択して読み込む
				string inputFilePath{};
				if (GetImageFilePathFromExplorer(inputFilePath)) circleGauge_.Load(inputFilePath);
			}
			break;
		}

		ImGui::TreePop();
	}

	// 値の設定
	if (ImGui::TreeNode("value")) {

		// 開始ボタンの表示
		if(ImGui::SmallButton("start"))StartTimer();
		ImGui::SameLine();

		// 停止ボタンの表示
		if(ImGui::SmallButton("stop"))StopTimer();
		ImGui::SameLine();

		// 終了ボタンの表示
		if(ImGui::SmallButton("end"))EndTimer();
		ImGui::SameLine();
		
		// リセットボタンの表示
		if(ImGui::SmallButton("reset"))ResetTimer();
		ImGui::SameLine();

		// リスタートボタンの表示
		if(ImGui::SmallButton("restart"))RestartTimer();

		// 最大値を秒で設定
		{
			static float sec = 0;
			ImGui::InputFloat("sec", &sec);
			ImGui::SameLine();
			if (ImGui::SmallButton("set"))SetTimer(sec);
		}

		// カウンターの表示
		ImGui::Text("count : %f", count_);
		ImGui::Text("sec   : %f", count_ / FPS);
		ImGui::TreePop();
	}

	ChangeFontWindow();
}

void UITimer::DrawTextTimer()
{
	// 表示位置を計算
	auto&& t = GetCalcTransform();

	float drawX = (t.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);
	float drawY = (t.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);

	// 表示サイズを設定
	pText_->SetScale(size_);

	// 文字列を設定
	string str{}; {
		int totalSeconds = count_ / FPS;
		int minutes = totalSeconds / 60;
		int seconds = totalSeconds % 60;

		str = std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
	}

	// テキストを描画
	pText_->Draw(drawX, drawY, str.c_str());
}

void UITimer::DrawCircleGaugeTimer()
{
	// 画像を描画
	circleGauge_.Draw(transform_);
}

bool UITimer::GetImageFilePathFromExplorer(string& _filePath) const
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


bool UITimer::GetFontFilePathFromExplorer(string& _filePath) const
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

void UITimer::ChangeFontWindow()
{
	if (!isOpenChangeFontWindow_)	return;
	ImGui::Begin("ChangeFont");
	{

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

bool CircleGauge::Load(string _imageFilePath)
{
	// 画像ファイルパスを設定
	imageFilePath = _imageFilePath;

	// ゲージの画像を読み込む
	imageHandle = Image::Load(_imageFilePath);

	// 読み込みに成功したか
	return imageHandle >= 0;
}

void CircleGauge::Update(float progress)
{
	// Progressは0.0 ~ 1.0で渡され、ゲージの角度に反映される
	startAngle = 0;  // 固定で開始角度を設定
	endAngle = startAngle + (progress * 360.0f);  // Progressに基づく終了角度
}

void CircleGauge::Draw(Transform _transform)
{
	Image::SetTransform(imageHandle, _transform);
	Image::Draw(imageHandle, NULL,startAngle, endAngle);
}


