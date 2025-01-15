#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// ユーザー情報 構造体
struct UserInfo {
    string          userName;       // ユーザー名
    vector<bool>    libraryStatus;  // 図鑑の完成状況
    int             playTotalTime;  // プレイ時間(アドベンチャーモードのみ)
    int             bestScore;	    // ベストスコア(スコアアタックモードのみ)

    UserInfo() : userName(""), libraryStatus(10, false), playTotalTime(0), bestScore(0) {}
    UserInfo(const string& _userName) : userName(_userName), libraryStatus(10, false), playTotalTime(0), bestScore(0) {}
};

class UserManager
{
private:
    unordered_map<string,UserInfo*> registeredUsers_;   // 登録ユーザー
    UserInfo* loggedInUser_;                            // ログイン中のユーザー

public:
    /// <summary> シングルトンインスタンスを取得 </summary>
    static UserManager& GetInstance();

    /// <summary> ユーザーを登録 </summary>
    bool RegisterUser(const string& _userName);

    /// <summary> ユーザーを削除 </summary>
    bool DeleteUser(const string& _userName);

    /// <summary> ユーザーをログイン </summary>
    bool LoginUser(const string& _userName);

    /// <summary> ユーザーをログアウト </summary>
    void LogoutUser();

    /// <summary> ユーザーを保存 </summary>
    bool SaveUser(const string& _filePath);

    /// <summary> ユーザーを読込 </summary>
    bool LoadUser(const string& _filePath);

/*
update :*/
    /// <summary> ユーザーの図鑑を更新 </summary>
    void UpdateLibraryStatus(const string& _userName, int _index);

    /// <summary> ユーザーのプレイ時間を更新 </summary>
    void UpdatePlayTotalTime(const string& _userName, int _time);

    /// <summary> ユーザーのベストスコアを更新 </summary>
    void UpdateBestScore(const string& _userName, int _score);

/*
getter :*/
    /// <summary> 特定のユーザーを取得 </summary>
    UserInfo* GetUser(const string& _userName) { return registeredUsers_[_userName];}

    /// <summary> ログイン中のユーザーを取得 </summary>
    UserInfo* GetLoggedInUser(){ return loggedInUser_;}

    /// <summary> 全てのユーザーを取得 </summary>
    unordered_map<string,UserInfo*> GetAllUsers() const{ return registeredUsers_;}

/*
predicate :*/
    /// <summary> ユーザーが登録されているかどうかを判定 </summary>
	bool isUserRegistered(const string& _userName);

    /// <summary> ユーザーがログイン中かどうかを判定 </summary>
    bool isUserLoggedIn() { return loggedInUser_ != nullptr; }

private:
    // コンストラクタはプライベート
	UserManager() :loggedInUser_(nullptr), registeredUsers_(unordered_map<string, UserInfo*>()) {}

    // コピーコンストラクタ・代入演算子を削除
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;
};

