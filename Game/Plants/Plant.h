#pragma once
#include <string>
#include<functional>

using std::string;

class Component_PlayerBehavior;

// �A���\����
struct PlantData {
	int id_;				// ���ʔԍ�
	int rarity_;			// �H���x
	string name_;			// ���O
	int areaId_;			// �o���G���A
	bool isSpawn_;
	string modelFilePath_;	// ���f���t�@�C���p�X
	string imageFilePath_;	// �摜�t�@�C���p�X

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
