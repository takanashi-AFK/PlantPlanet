#pragma once
#include <string>

//-----------------------------------------------------------
//�T�E���h���Ǘ�����iXAudio�g�p�j
//-----------------------------------------------------------
namespace Audio
{
	/// <summary> ������ </summary>
	void Initialize();

	//���łɓ������O�̃t�@�C�������[�h�ς݂̏ꍇ�́A�����̃f�[�^�̔ԍ���Ԃ�
	/// <summary> �Ǎ� </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="isLoop">���[�v�Đ����������Ȃ�true</param>
	/// <param name="svNum">�����ɖ炷�ő吔�i�ȗ��j</param>
	/// <returns>���̃f�[�^�Ɋ��蓖�Ă�ꂽ�ԍ�</returns>
	int Load(std::string fileName, bool isLoop = false, int svNum = 1);

	/// <summary> �Đ� </summary>
	void Play(int ID);
	void Play(int ID,float volume);

	/// <summary> ��~ </summary>
	void Stop(int ID);

	/// <summary> ��� </summary>
	void Release();

	/// <summary> �S�Ẳ�� </summary>
	void AllRelease();

	/// <summary> �{�����[���ݒ� </summary>
	void SetVolume(int ID, float volume);

	/// <summary> �Đ����x�ݒ� </summary>
	void SetPlaybackSpeed(int ID, float speed);
};