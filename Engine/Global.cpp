#include "Global.h"

void FileManager::ReplaceBackslashes(string& str)
{
	size_t found = str.find("\\");
	while (found != string::npos) {
		str.replace(found, 1, "/"); // "\\" �� "/" �ɒu������
		found = str.find("\\", found + 1); // ���� "\\" ����������
	}
}

std::string FileManager::GetAssetsRelativePath(const std::string& absolutePath)
{
	const std::string assetsFolder = "Assets\\";
	// "Assets\\" �̈ʒu������
	size_t assetsPos = absolutePath.find(assetsFolder);
	if (assetsPos != std::string::npos) {
		// "Assets\\" �̕������������A����ȍ~�̕������擾
		std::string relativePath = absolutePath.substr(assetsPos + assetsFolder.size());
		return relativePath;
	}
	else {
		// "Assets\\" ��������Ȃ��ꍇ�́A���̃t�@�C���p�X��Ԃ�
		return absolutePath;
	}
}