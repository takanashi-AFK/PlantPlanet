#pragma once
#include<string>
using std::string;
class Plant 
{
private:
	int id_;
	int rarity_;
	string name_;
	string modelFilePath_;
	string imageFilePath_;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">親オブジェクト</param>
	Plant();

	void SetID(int _id) { id_ = _id; }
	void SetRarity(int _rarity) { rarity_ = _rarity; }
	void SetName(string _name) { name_ = _name; }
	void SetModelFilePath(string _modelFilePath) { modelFilePath_ = _modelFilePath; }
	void SetImageFilePath(string _imageFilePath) { imageFilePath_ = _imageFilePath; }

	int GetID() { return id_; }
	int GetRarity() { return rarity_; }
	string GetName() { return name_; }

	void DrawData();
};

