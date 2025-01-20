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
	// マウスの座標を取得
	XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

	// マウスの座標を画像の座標に変換
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
	// 画像が読み込まれていない場合は処理を行わない
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
					defaultImageFilePath_ = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					defaultImageFilePath_ = FileManager::GetAssetsRelativePath(defaultImageFilePath_);

					// 文字列内の"\\"を"/"に置換
					FileManager::ReplaceBackslashes(defaultImageFilePath_);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);

					// 画像を読み込む
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
					selectImageFilePath_ = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					selectImageFilePath_ = FileManager::GetAssetsRelativePath(selectImageFilePath_);

					// 文字列内の"\\"を"/"に置換
					FileManager::ReplaceBackslashes(selectImageFilePath_);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);

					// 画像を読み込む
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
	// マウスの座標を取得
	XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

	// マウスの座標を画像の座標に変換
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
	// マウスの座標を取得
	XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

	// マウスの座標を画像の座標に変換
	ConvertToImageCoordinates(mousePos);

	//マウスのポジションが画像の領域に入っているかつ左クリックが押されたら
	return IsMouseOver(mousePos) && Input::IsMouseButtonDown(0) && this->isVisible_;
}

bool UIButton::IsMouseOver(XMFLOAT2 _mousePosition)
{
	// 画像が読み込まれていない場合は処理を行わない
	if (imageHandle_ < 0)return false;

	// 画面のサイズを取得
	int scWidth = Direct3D::screenWidth_;
	int scHeight = Direct3D::screenHeight_;

	// マウスカーソルの座標を取得
	XMFLOAT2 imageSize = Image::GetSize(imageHandle_); {
		// 画像のサイズを画面サイズに合わせる

		imageSize.x = imageSize.x / scWidth;
		imageSize.y = imageSize.y / scHeight;
	}

	// 画像の半分のサイズを取得
	float imageHelfWidth = imageSize.x /2.f;
	float imageHelfHeight = imageSize.y /2.f;

	// 画像の中心座標を取得
	auto&& calcform = GetCalcTransform();

	XMFLOAT2 center = { calcform.position_.x,calcform.position_.y };

	// 画像の拡大率を取得
	XMFLOAT2 scale = { calcform.scale_.x,calcform.scale_.y };

	// 判定用の範囲の値を取得
	float top = center.y - (imageHelfHeight * 2.f * scale.y);       // 画像の中心から上に画像の高さの半分の距離
	float bottom = center.y + (imageHelfHeight * 2.f * scale.y);    // 画像の中心から下に画像の高さの半分の距離
	float left = center.x - (imageHelfWidth * 2.f * scale.x);       // 画像の中心から左に画像の幅の半分の距離
	float right = center.x + (imageHelfWidth * 2.f * scale.x);      // 画像の中心から右に画像の幅の半分の距離
	
	// 判定範囲内にマウスカーソルが入っているかどうかを返す
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

	// マウスの座標を画像の座標に変換
	_position.x = (float)(_position.x * 2.0f) / (float)scWidth - 1.0f;
	_position.y = 1.0f - (float)(_position.y * 2.0f) / (float)scHeight;
}
