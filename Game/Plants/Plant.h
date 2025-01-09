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

	struct FuncValue
	{
		bool isUsable;		 //when false then delete Function
		std::string filePath;//image path
		int id;    //plant id
		int time;  //left time of effect
		int amount;//amount of effect
		string specialText;//use this when special func
	};

	FuncValue Effect(Component_PlayerBehavior* pb);
	static std::function<FuncValue(Component_PlayerBehavior*)> GetFunction(int id);
};
