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

	bool Effect(Component_PlayerBehavior* pb);
	static std::function<bool(Component_PlayerBehavior*)> GetFunction(int id);
};
