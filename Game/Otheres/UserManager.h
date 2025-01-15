#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// ���[�U�[��� �\����
struct UserInfo {
    string          userName;       // ���[�U�[��
    vector<bool>    libraryStatus;  // �}�ӂ̊�����
    int             playTotalTime;  // �v���C����(�A�h�x���`���[���[�h�̂�)
    int             bestScore;	    // �x�X�g�X�R�A(�X�R�A�A�^�b�N���[�h�̂�)

    UserInfo() : userName(""), libraryStatus(10, false), playTotalTime(0), bestScore(0) {}
    UserInfo(const string& _userName) : userName(_userName), libraryStatus(10, false), playTotalTime(0), bestScore(0) {}
};

class UserManager
{
private:
    unordered_map<string,UserInfo*> registeredUsers_;   // �o�^���[�U�[
    UserInfo* loggedInUser_;                            // ���O�C�����̃��[�U�[

public:
    /// <summary> �V���O���g���C���X�^���X���擾 </summary>
    static UserManager& GetInstance();

    /// <summary> ���[�U�[��o�^ </summary>
    bool RegisterUser(const string& _userName);

    /// <summary> ���[�U�[���폜 </summary>
    bool DeleteUser(const string& _userName);

    /// <summary> ���[�U�[�����O�C�� </summary>
    bool LoginUser(const string& _userName);

    /// <summary> ���[�U�[�����O�A�E�g </summary>
    void LogoutUser();

    /// <summary> ���[�U�[��ۑ� </summary>
    bool SaveUser(const string& _filePath);

    /// <summary> ���[�U�[��Ǎ� </summary>
    bool LoadUser(const string& _filePath);

/*
update :*/
    /// <summary> ���[�U�[�̐}�ӂ��X�V </summary>
    void UpdateLibraryStatus(const string& _userName, int _index);

    /// <summary> ���[�U�[�̃v���C���Ԃ��X�V </summary>
    void UpdatePlayTotalTime(const string& _userName, int _time);

    /// <summary> ���[�U�[�̃x�X�g�X�R�A���X�V </summary>
    void UpdateBestScore(const string& _userName, int _score);

/*
getter :*/
    /// <summary> ����̃��[�U�[���擾 </summary>
    UserInfo* GetUser(const string& _userName) { return registeredUsers_[_userName];}

    /// <summary> ���O�C�����̃��[�U�[���擾 </summary>
    UserInfo* GetLoggedInUser(){ return loggedInUser_;}

    /// <summary> �S�Ẵ��[�U�[���擾 </summary>
    unordered_map<string,UserInfo*> GetAllUsers() const{ return registeredUsers_;}

/*
predicate :*/
    /// <summary> ���[�U�[���o�^����Ă��邩�ǂ����𔻒� </summary>
	bool isUserRegistered(const string& _userName);

    /// <summary> ���[�U�[�����O�C�������ǂ����𔻒� </summary>
    bool isUserLoggedIn() { return loggedInUser_ != nullptr; }

private:
    // �R���X�g���N�^�̓v���C�x�[�g
	UserManager() :loggedInUser_(nullptr), registeredUsers_(unordered_map<string, UserInfo*>()) {}

    // �R�s�[�R���X�g���N�^�E������Z�q���폜
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;
};

