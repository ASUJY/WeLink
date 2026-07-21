#ifndef USER_H
#define USER_H

#include <string>
#include <QObject>
#include <QString>
#include "common.h"

class User {
public:
    User() = default;
    User(int64_t id, std::string name, std::string avatar, std::string phone = "", std::string account = "",
         std::string pwd = "",  std::string state = "") : m_userid(id), m_username(std::move(name)), m_avatar(std::move(avatar)),
        m_userphone(std::move(phone)), m_account(std::move(account)), m_password(std::move(pwd)), m_userstate(std::move(state)){}
    User(User&& other) noexcept = default;
    User& operator=(User&& other) noexcept = default;
    User(const User&) = delete;
    User& operator=(const User&) = delete;
    ~User() = default;

    void SetUserId(int64_t id) {this->m_userid = id;}
    void SetUserPhone(const std::string& phone) {this->m_userphone = phone;}
    void SetUserPhone(std::string&& phone) noexcept { m_userphone = std::move(phone);}
    void SetUserAccount(const std::string& account) {this->m_account = account;}
    void SetUserAccount(std::string&& account) noexcept { m_account = std::move(account);}
    void SetUserName(const std::string& name) {this->m_username = name;}
    void SetUserName(std::string&& name) noexcept {m_username = std::move(name);}
    void SetUserAvatar(const std::string& avatar) {this->m_avatar = avatar;}
    void SetUserAvatar(std::string&& avatar) noexcept { m_avatar = std::move(avatar);}
    void SetPwd(const std::string& pwd) {this->m_password = pwd;}
    void SetPwd(std::string&& pwd) noexcept {m_password = std::move(pwd);}
    void SetUserState(const std::string &state) {this->m_userstate = state;}
    void SetUserState(std::string&& state) noexcept {m_userstate = std::move(state);}

    int64_t GetUserId() const {return m_userid;}
    const std::string& GetUserPhone() const {return m_userphone;}
    const std::string& GetUserAccount() const {return m_account;}
    const std::string& GetUserName() const {return m_username;}
    const std::string& GetUserAvatar() const {return m_avatar;}
    const std::string& GetPwd() const {return m_password;}
    const std::string& GetUserState() const {return m_userstate;}

private:
    int64_t m_userid = -1;
    std::string m_userphone;
    std::string m_account;
    std::string m_username;
    std::string m_avatar;
    std::string m_password;
    std::string m_userstate = "offline";
};

#endif // USER_H
