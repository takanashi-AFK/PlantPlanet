#pragma once
#include <string>
using std::string;

// �A���\����
struct PlantData {
	int id_;				// ���ʔԍ�
	int rarity_;			// �H���x
	string name_;			// ���O
	int areaId_;			// �o���G���A
	bool isSpawn_;
	string modelFilePath_;	// ���f���t�@�C���p�X
	string imageFilePath_;	// �摜�t�@�C���p�X
};
