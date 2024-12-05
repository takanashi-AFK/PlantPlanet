#pragma once
#include <string>
using std::string;

// 植物構造体
struct PlantData {
	int id_;				// 識別番号
	int rarity_;			// 稀少度
	string name_;			// 名前
	int areaId_;			// 出現エリア
	bool isSpawn_;
	string modelFilePath_;	// モデルファイルパス
	string imageFilePath_;	// 画像ファイルパス
};
