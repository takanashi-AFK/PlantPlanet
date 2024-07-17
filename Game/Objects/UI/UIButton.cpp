#include "UIButton.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/Global.h"

using namespace FileManager;


UIButton::UIButton(string _name, UIObject* parent , int _layerNum)
	: UIObject(_name, UIType::UI_BUTTON, parent, _layerNum), imageHandle_(-1), imageFilePath_()

{
}

void UIButton::Initialize()
{
    LockRotate();

}

void UIButton::Update()
{
    if (imageHandle_ < 0)return;

    // マウスの座標を取得
    XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

    // マウスの座標を画像の座標に変換
    ConvertToImageCoordinates(mousePos);

	if (IsMouseOver(mousePos))
        Image::SetAlpha(imageHandle_, 192);
	else 
        Image::SetAlpha(imageHandle_, 256);
}

void UIButton::Draw()
{
    // 画像が読み込まれていない場合は処理を行わない
    if (imageHandle_ < 0)return;

	Image::SetTransform(imageHandle_, transform_);
	Image::Draw(imageHandle_);
}

void UIButton::Release()
{
}

void UIButton::Save(json& saveObj)
{
	saveObj["imageFilePath_"] = imageFilePath_;
}

void UIButton::Load(json& loadObj)
{
    if (loadObj.contains("imageFilePath_")) {
        imageFilePath_ = loadObj["imageFilePath_"].get<string>();
        SetImage(imageFilePath_);
    }
}

void UIButton::DrawData()
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
	
	if (ImGui::TreeNode("Data"))
	{
        ImGui::Text("OnClick: %s", OnClick() ? "true" : "false");

        XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };
        ConvertToImageCoordinates(mousePos);
        if (imageHandle_ >= 0)ImGui::Text("IsMouseOver: %s", IsMouseOver(mousePos) ? "true" : "false");

		ImGui::TreePop();
	}
}

void UIButton::SetImage(string _imageFilePath)
{
	imageFilePath_ = _imageFilePath;
	imageHandle_ = Image::Load(_imageFilePath);
}

bool UIButton::OnClick()
{
    // マウスの座標を取得
    XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

    // マウスの座標を画像の座標に変換
    ConvertToImageCoordinates(mousePos);

    //マウスのポジションが画像の領域に入っているかつ左クリックが押されたら
    return IsMouseOver(mousePos) && Input::IsMouseButtonDown(0);
}

bool UIButton::IsMouseOver(XMFLOAT2 _mousePosition)
{
    // 画像が読み込まれていない場合は処理を行わない
    if (imageHandle_ < 0)return false;

	// マウスカーソルの座標を取得
    XMFLOAT2 imageSize = Image::GetSize(imageHandle_); {
        // 画像のサイズを画面サイズに合わせる
        imageSize.x = imageSize.x / Direct3D::screenWidth_;
        imageSize.y = imageSize.y / Direct3D::screenHeight_;
    }

    // 画像の半分のサイズを取得
    float imageHelfWidth = imageSize.x /2.f;
    float imageHelfHeight = imageSize.y /2.f;

    // 画像の中心座標を取得
    XMFLOAT2 center = { transform_.position_.x,transform_.position_.y };

    // 画像の拡大率を取得
    XMFLOAT2 scale = { transform_.scale_.x,transform_.scale_.y };

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

#ifdef _DEBUG
    scWidth = scWidth * 0.7;
    scHeight = scHeight * 0.7;
#endif // _DEBUG

    // マウスの座標を画像の座標に変換
    _position.x = (float)(_position.x * 2.0f) / (float)scWidth - 1.0f;
    _position.y = 1.0f - (float)(_position.y * 2.0f) / (float)scHeight;
}
