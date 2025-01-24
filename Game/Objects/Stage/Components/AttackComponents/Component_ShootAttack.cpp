#include "Component_ShootAttack.h"

// インクルード
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Bullet.h"
#include "../../Stage.h"
#include "../../../../../Engine/Global.h"
using namespace FileManager;

Component_ShootAttack::Component_ShootAttack(string _name, StageObject* _holder, Component* _parent):
	Component_Attack(_holder, _name, ShootAttack,_parent),
	shootingSpeed_(0),
	shootingDirection_({0,0,0,0}),
	shootingPosition_({0,0,0}),
	isShootPositionSet_(false),
	bulletLifeTime_(0),
	effectData({}),
	bulletColliderRadius_(0)
{
}

void Component_ShootAttack::Initialize()
{
}

void Component_ShootAttack::Update()
{	
	if (isActive_ == false)return;

	// 撃ち放つプレハブを生成
	Bullet* bulletPrefab = CreateBullet(holder_->GetParent(), effectData,bulletColliderRadius_); {

		// 弾を撃ち放ったオブジェクトを設定
		bulletPrefab->SetShooter(holder_);

		// 撃ち放つ速度を設定
		bulletPrefab->SetSpeed(shootingSpeed_);

		// 撃ち放つ方向を設定
		bulletPrefab->SetDirection(shootingDirection_);

		// 弾の生存時間を設定
		bulletPrefab->SetLifeTime(bulletLifeTime_);

		// 攻撃力を設定
		bulletPrefab->SetPower(power_);

		// 撃ち放つ位置を設定
		if (isShootPositionSet_ == true)bulletPrefab->SetPosition(shootingPosition_);
		else bulletPrefab->SetPosition(holder_->GetPosition());
	}
	
	// 撃ち放つ
	bulletPrefab->Execute();
	
	// 攻撃を停止
	Stop();
}

void Component_ShootAttack::Release()
{
}

void Component_ShootAttack::Save(json& _saveObj)
{
	// 攻撃力の保存
	_saveObj["power_"] = power_;

	// 速度と方向の保存
	_saveObj["shootingSpeed_"] = shootingSpeed_;
	_saveObj["shootingDirection_"] = { REFERENCE_XMVECTOR3(shootingDirection_)};

	// 弾の生存時間の保存
	_saveObj["bulletLifeTime_"] = bulletLifeTime_;

	// 弾の大きさの保存
	_saveObj["bulletColliderRadius_"] = bulletColliderRadius_;

	// エフェクトデータの保存
	_saveObj["EffectData"] = {
		{"name",effectData.name},
		{"path",effectData.path},
	};

	// エフェクトのスケールの保存
	_saveObj["EffectData"]["scale"] = { REFERENCE_XMFLOAT3(effectData.scale)};
}

void Component_ShootAttack::Load(json& _loadObj)
{
	// 攻撃力の読み込み
	if (_loadObj.contains("power_"))power_ = _loadObj["power_"];

	// 速度と方向の読み込み
	if (_loadObj.contains("shootingSpeed_"))shootingSpeed_ = _loadObj["shootingSpeed_"];
	if (_loadObj.contains("shootingDirection_"))shootingDirection_ = XMVectorSet(_loadObj["shootingDirection_"][0], _loadObj["shootingDirection_"][1],_loadObj["shootingDirection_"][2],0);

	// 弾の大きさの読み込み
	if (_loadObj.contains("bulletColliderRadius_"))bulletColliderRadius_ = _loadObj["bulletColliderRadius_"];

	// 弾の生存時間の読み込み
	if (_loadObj.contains("bulletLifeTime_"))bulletLifeTime_ = _loadObj["bulletLifeTime_"];

	// エフェクトデータの読み込み
	if (_loadObj.contains("EffectData")) {
		if(_loadObj["EffectData"].contains("name")) effectData.name = _loadObj["EffectData"]["name"];
		if(_loadObj["EffectData"].contains("path")) effectData.path = _loadObj["EffectData"]["path"];
		if(_loadObj["EffectData"].contains("scale"))effectData.scale = { _loadObj["EffectData"]["scale"][0].get<float>(),_loadObj["EffectData"]["scale"][1].get<float>(), _loadObj["EffectData"]["scale"][2].get<float>() };
	}
}

void Component_ShootAttack::DrawData()
{
	// 攻撃力の設定
	ImGui::DragInt("Power", &power_);

	// 速度の設定
	ImGui::DragFloat("Speed", &shootingSpeed_, 0.1f,0.f);
	
	// 方向の設定
	ImGui::DragFloat3("Direction", (float*)&shootingDirection_, 0.1f);

	// 弾を生存期間を設定
	ImGui::DragFloat("bulletLifeTime_", &bulletLifeTime_, 0.1f, 0.f);

	// 弾の大きさを設定
	ImGui::DragFloat("bulletColliderRadius_", &bulletColliderRadius_, 0.1f, 0.f);

	// 攻撃ボタン
	if (ImGui::Button("Execute"))this->Execute();
	
	// エフェクトデータの表示
	if(ImGui::TreeNode("Effect Data")) {

		// エフェクトの名前
		ImGui::Text("Name: %s", effectData.name.c_str());
		// エフェクトのパス
		ImGui::Text("Path: %s", effectData.path.c_str());
		ImGui::SameLine();

		// エフェクトの設定
		if (ImGui::Button("Set Effect")){
			//現在のカレントディレクトリを覚えておく
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// エフェクトのファイルパスを設定
			{
				// 「ファイルを開く」ダイアログの設定用構造体を設定
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
					ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
					ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
					ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
					ofn.lpstrFile[0] = '\0'; // 初期化
					ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
					ofn.lpstrFilter = TEXT("FBXファイル(*.efk)\0*.efk\0すべてのファイル(*.*)\0*.*\0"); // フィルター（efkファイルのみ表示）
					ofn.nFilterIndex = 1; // 初期選択するフィルター
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
					ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
				}

				// ファイルを選択するダイアログの表示
				if (GetOpenFileName(&ofn) == TRUE) {
					// ファイルパスを取得
					effectData.path = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					effectData.path = GetAssetsRelativePath(effectData.path);

					// 文字列内の"\\"を"/"に置換
					ReplaceBackslashes(effectData.path);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					return;
				}
			}

			// エフェクトの名前を設定
			effectData.name = GetFileNameWithoutExtension(effectData.path);
		}

		// エフェクトのスケール
		ImGui::DragFloat3("Scale", (float*)&effectData.scale, 0.1f);

		ImGui::TreePop();
	}
}
