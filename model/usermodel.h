#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"
#include <QString>

class UserModel
{
public:
    explicit UserModel(const QString &connName = "userConn");
    ~UserModel() = default;

    bool AddUser(std::shared_ptr<User>user);
    bool IsUserExit(std::shared_ptr<User>user);

private:
    bool IsTableExit();

private:
    QString m_connName;
};

#endif // USERMODEL_H
