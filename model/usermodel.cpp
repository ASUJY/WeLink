#include "usermodel.h"
#include "dbmagr.h"
#include <QSqlQuery>

UserModel::UserModel(const QString &connName) : m_connName(connName) {
    IsTableExit();
}

bool UserModel::IsTableExit() {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS im_user (
            userid INTEGER NOT NULL PRIMARY KEY,
            account TEXT UNIQUE,
            phone TEXT UNIQUE,
            nickname TEXT,
            avatar TEXT,
            password TEXT
        )
    )";


    return DBMagr::Instance()->ExecQuery(query, sql);
}

bool UserModel::AddUser(std::shared_ptr<User> user) {

    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery insertQuery(db);
    insertQuery.prepare("insert into im_user(userid, phone, nickname, avatar) values(?,?,?,?)");
    insertQuery.addBindValue(user->GetUserId());
    // insertQuery.addBindValue(QString::fromStdString(user.GetUserAccount()));
    insertQuery.addBindValue(QString::fromStdString(user->GetUserPhone()));
    insertQuery.addBindValue(QString::fromStdString(user->GetUserName()));
    insertQuery.addBindValue(QString::fromStdString(user->GetUserAvatar()));

    return DBMagr::Instance()->ExecQuery(insertQuery);
}

bool UserModel::IsUserExit(std::shared_ptr<User>user) {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("select * from im_user where userid = ?");
    query.addBindValue(user->GetUserId());

    if (DBMagr::Instance()->ExecQuery(query) && query.next()) {
        return true;
    }

    return false;
}