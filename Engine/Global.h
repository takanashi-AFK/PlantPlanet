#pragma once
#include "DirectX/Direct3D.h"
#include <filesystem>
#include <string>
#include <vector>
#include "../Game/Plants/Plant.h"

// ＝＝ マクロ ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝ //

//安全にメモリを開放するためのマクロ
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

// XMFLOAT3型の変数をコンマ区切りで表示するマクロ
#define REFERENCE_XMFLOAT3(p) p.x,p.y,p.z 
#define REFERENCE_XMFLOAT4(p) p.x,p.y,p.z,p.w 
#define REFERENCE_XMVECTOR3(p) XMVectorGetX(p),XMVectorGetY(p),XMVectorGetZ(p)
#define REFERENCE_XMINT2(p) p.x,p.y

// オペレーターのオーバーロード
XMFLOAT3 operator+(XMFLOAT3 a, const XMFLOAT3 b);
XMFLOAT3 operator-(XMFLOAT3 a, const XMFLOAT3 b);
XMFLOAT3 operator*(XMFLOAT3 a, const float b);
XMFLOAT3 operator*(XMFLOAT3 a, const XMFLOAT3 b);

// ＝＝ 定数 ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝ //

constexpr float FPS = 60.0f;	// フレームレート

// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝ //

namespace ScoreManager {
/*
valeable :*/
	extern int time;		// タイム
	extern int playerHp;	// プレイヤーのHP
	extern bool isClear;	// クリアしたかどうか
	extern string userName;	// ユーザー名
}

// ファイル管理namespace
namespace FileManager {
	// 文字列内の全ての "\\" を "/" に置換する関数
	void ReplaceBackslashes(string& str);

	// カレントディレクトリからの相対パスを取得する関数
	std::string GetAssetsRelativePath(const std::string& absolutePath);

	// 関数: パスからファイル名を取得する
	std::string GetFileNameFromPath(const std::string& path);

	// 関数: パスから拡張子なしのファイル名を取得する
	std::string GetFileNameWithoutExtension(const std::string& path);
}

namespace Light
{
	extern XMFLOAT4 sunLight;
	extern XMFLOAT4 sunLightDir;
	extern XMFLOAT4 ambientLight;
}

/// 臨時のグローバル変数 fix: 本来はここに置くべきではない 
extern string g_selectedStage;	// 選択されたステージ
extern int g_selectedGameMode;	// 選択されたゲームモード
extern std::vector<PlantData> g_playerPlantData;	// 植物データ
extern std::vector<PlantData> g_thisPlayGetPlantData;

extern int g_playTime;

// エフェクトデータ
// fix: 本来はここに置くべきではない
struct EffectData {
	std::string name;
	std::string path;
	XMFLOAT3 scale;
};
