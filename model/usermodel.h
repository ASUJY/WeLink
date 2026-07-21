#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"
#include <QString>

class UserModel
{
public:
    explicit UserModel(const QString &connName = "userConn");
    ~UserModel() = default;

    bool AddUser(const std::shared_ptr<User>& user);
    bool IsUserExist(const std::shared_ptr<User>& user) const;

private:
    bool IsTableExit() const;

private:
    QString m_connName;
};

#endif // USERMODEL_H
