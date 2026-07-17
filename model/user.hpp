#ifndef USER_H
#define USER_H

#include <string>
#include <QObject>
#include <QString>

class User {
public:
    User(int id = -1, std::string name = "", std::string avatar = "", std::string phone = "", std::string account = "",
        std::string pwd = "",  std::string state = "offline") {
        this->m_userid = id;
        this->m_userphone = phone;
        this->m_account = account;
        this->m_username = name;
        this->m_avatar = avatar;
        this->m_password = pwd;
        this->m_userstate = state;
    }
    ~User() = default;

    void SetUserId(int id) {this->m_userid = id;}
    void SetUserPhone(const std::string phone) {this->m_userphone = phone;}
    void SetUserAccount(const std::string account) {this->m_account = account;}
    void SetUserName(const std::string name) {this->m_username = name;}
    void SetUserAvatar(const std::string avatar) {this->m_avatar = avatar;}
    void SetPwd(const std::string pwd) {this->m_password = pwd;}
    void SetUserState(const std::string state) {this->m_userstate = state;}

    int GetUserId() const {return m_userid;}
    std::string GetUserPhone() const {return m_userphone;}
    std::string GetUserAccount() const {return m_account;}
    std::string GetUserName() const {return m_username;}
    std::string GetUserAvatar() const {return m_avatar;}
    std::string GetPwd() const {return m_password;}
    std::string GetUserState() const {return m_userstate;}

private:
    int m_userid;
    std::string m_userphone;
    std::string m_account;
    std::string m_username;
    std::string m_avatar;
    std::string m_password;
    std::string m_userstate;
};

#endif // USER_H
