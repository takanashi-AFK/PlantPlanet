#include "UIInputString.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/ResourceManager/Text.h"

UIInputString::UIInputString(string _name, UIObject* parent, int _layerNum)
	:UIObject(_name, UI_INPUTSTRING, parent, _layerNum), currentInput(""), isComplete(false)
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
    for (const auto& pair : keyToCharMap) {
        if (Input::IsKeyDown(pair.first)) {
            currentInput += pair.second;
        }
    }

    // バックスペースキー処理
    if (Input::IsKeyDown(DIK_BACK) && !currentInput.empty()) {
        currentInput.pop_back();
    }

    // Enterキーが押されたら処理を終了
    if (Input::IsKeyDown(DIK_RETURN)) {
        isComplete = true;
    }
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
    pText_->Draw(drawX, drawY, currentInput.c_str(),fade_);
}

void UIInputString::Release()
{
}

void UIInputString::Save(json& saveObj)
{
    // 文字列を保存
    saveObj["text"] = currentInput;

    // サイズを保存
    saveObj["size"] = size_;
}

void UIInputString::Load(json& loadObj)
{
    // 文字列を読み込み
    if (loadObj.contains("text")) currentInput = loadObj["text"].get<string>();

    // サイズを読み込み
	if (loadObj.contains("size")) size_ = loadObj["size"].get<float>();
}

void UIInputString::DrawData()
{
	if (ImGui::TreeNode("value")) {

		// 入力された文字列を表示
		ImGui::Text("currentInput : %s", currentInput.c_str());

		// 入力完了フラグを表示
		ImGui::Text("isComplete : %s", isComplete ? "true" : "false");ImGui::SameLine();
		ImGui::Checkbox("##isComplete", &isComplete);

		ImGui::TreePop();
	}

    if (ImGui::TreeNode("draw")) {

		// テキストの描画サイズを変更
		ImGui::DragFloat("size", &size_, 0.01f);

		ImGui::TreePop();
    }
}

void UIInputString::InitializeKeyMapping()
{
    keyToCharMap[DIK_A] = 'a';
    keyToCharMap[DIK_B] = 'b';
    keyToCharMap[DIK_C] = 'c';
    keyToCharMap[DIK_D] = 'd';
    keyToCharMap[DIK_E] = 'e';
    keyToCharMap[DIK_F] = 'f';
    keyToCharMap[DIK_G] = 'g';
    keyToCharMap[DIK_H] = 'h';
    keyToCharMap[DIK_I] = 'i';
    keyToCharMap[DIK_J] = 'j';
    keyToCharMap[DIK_K] = 'k';
    keyToCharMap[DIK_L] = 'l';
    keyToCharMap[DIK_M] = 'm';
    keyToCharMap[DIK_N] = 'n';
    keyToCharMap[DIK_O] = 'o';
    keyToCharMap[DIK_P] = 'p';
    keyToCharMap[DIK_Q] = 'q';
    keyToCharMap[DIK_R] = 'r';
    keyToCharMap[DIK_S] = 's';
    keyToCharMap[DIK_T] = 't';
    keyToCharMap[DIK_U] = 'u';
    keyToCharMap[DIK_V] = 'v';
    keyToCharMap[DIK_W] = 'w';
    keyToCharMap[DIK_X] = 'x';
    keyToCharMap[DIK_Y] = 'y';
    keyToCharMap[DIK_Z] = 'z';
    keyToCharMap[DIK_0] = '0';
    keyToCharMap[DIK_1] = '1';
    keyToCharMap[DIK_2] = '2';
    keyToCharMap[DIK_3] = '3';
    keyToCharMap[DIK_4] = '4';
    keyToCharMap[DIK_5] = '5';
    keyToCharMap[DIK_6] = '6';
    keyToCharMap[DIK_7] = '7';
    keyToCharMap[DIK_8] = '8';
    keyToCharMap[DIK_9] = '9';
    keyToCharMap[DIK_SPACE] = ' ';
}
