#include "Global.h"

void FileManager::ReplaceBackslashes(string& str)
{
	size_t found = str.find("\\");
	while (found != string::npos) {
		str.replace(found, 1, "/"); // "\\" を "/" に置換する
		found = str.find("\\", found + 1); // 次の "\\" を検索する
	}
}

std::string FileManager::GetAssetsRelativePath(const std::string& absolutePath)
{
	const std::string assetsFolder = "Assets\\";
	// "Assets\\" の位置を検索
	size_t assetsPos = absolutePath.find(assetsFolder);
	if (assetsPos != std::string::npos) {
		// "Assets\\" の部分を除いた、それ以降の部分を取得
		std::string relativePath = absolutePath.substr(assetsPos + assetsFolder.size());
		return relativePath;
	}
	else {
		// "Assets\\" が見つからない場合は、元のファイルパスを返す
		return absolutePath;
	}
}

std::string FileManager::GetFileNameFromPath(const std::string& path)
{
	return std::filesystem::path(path).filename().string();
}


// 関数: パスから拡張子なしのファイル名を取得する
std::string FileManager::GetFileNameWithoutExtension(const std::string& path) {
	return std::filesystem::path(path).stem().string();
}
XMFLOAT3 operator+(XMFLOAT3 a, const XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

XMFLOAT3 operator-(XMFLOAT3 a, const XMFLOAT3 b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

XMFLOAT3 operator*(XMFLOAT3 a, const float b)
{
	return XMFLOAT3(a.x * b, a.y * b, a.z * b);
}

XMFLOAT3 operator*(XMFLOAT3 a, const XMFLOAT3 b)
{
	return XMFLOAT3(a.x * b.x, a.y * b.y, a.z * b.z);
}

// ゲームのスコア
namespace ScoreManager {

	// NOTE: シーン間で共有する変数はここに記述する
/*
valeable :*/
	int time;			// タイム
	int playerHp;		// プレイヤーのHP
	int justAvoidance;	// ジャスト回避した回数
	int recieveDMG;	// 受けたダメージ
	int dealtDMG;		// 与ダメージ
	bool isClear;		// クリアしたかどうか
	string userName;	// ユーザー名
}

namespace Light
{
	XMFLOAT4 sunLight;
	XMFLOAT4 sunLightDir = { 1, -1, 1, 0 };
	XMFLOAT4 ambientLight = { 0,0,0,0 };
}

string g_selectedStage = "";
int g_selectedGameMode = 0;
std::vector<PlantData> g_playerPlantData;
std::vector<PlantData> g_thisPlayGetPlantData;
GameMode g_gameMode;
int g_playTime;

