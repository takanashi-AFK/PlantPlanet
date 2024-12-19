#pragma once
#include <string>
#include<functional>

using std::string;

class Component_PlayerBehavior;

// 植物構造体
struct PlantData {
	int id_;				// 識別番号
	int rarity_;			// 稀少度
	string name_;			// 名前
	int areaId_;			// 出現エリア
	bool isSpawn_;
	string modelFilePath_;	// モデルファイルパス
	string imageFilePath_;	// 画像ファイルパス

	bool Effect(Component_PlayerBehavior* pb);
	static std::function<bool(Component_PlayerBehavior*)> GetFunction(int id);
};
