#include "usermodel.h"
#include "dbmagr.h"
#include <QSqlQuery>
#include <QSqlError>

UserModel::UserModel(const QString &connName) : m_connName(connName) {
    IsTableExit();
}

bool UserModel::IsTableExit() const{
    QSqlDatabase db = DBMagr::Instance().GetConnection(m_connName);
    if (!db.isOpen()) {
        qCritical() << "UserModel 数据库连接失效，conn:" << m_connName;
        return false;
    }

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


    return DBMagr::Instance().ExecQuery(query, sql);
}

bool UserModel::AddUser(const std::shared_ptr<User>& user) {

    if (!user) {
        qWarning() << "AddUser 传入空 User 智能指针";
        return false;
    }

    QSqlDatabase db = DBMagr::Instance().GetConnection(m_connName);
    if (!db.isOpen()) {
        qCritical() << "AddUser 数据库未打开 conn:" << m_connName;
        return false;
    }

    if (!db.transaction())
    {
        qCritical() << "AddUser 开启事务失败:" << db.lastError().text();
        return false;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare("insert into im_user(userid, phone, nickname, avatar) values(?,?,?,?)");
    insertQuery.addBindValue(user->GetUserId());
    // insertQuery.addBindValue(QString::fromStdString(user.GetUserAccount()));
    insertQuery.addBindValue(QString::fromStdString(user->GetUserPhone()));
    insertQuery.addBindValue(QString::fromStdString(user->GetUserName()));
    insertQuery.addBindValue(QString::fromStdString(user->GetUserAvatar()));

    if (DBMagr::Instance().ExecQuery(insertQuery)) {
        db.commit();
        return true;
    }
    db.rollback();
    qCritical() << "AddUser 插入用户数据失败，事务已回滚";
    return false;
}

bool UserModel::IsUserExist(const std::shared_ptr<User>& user) const {
    if (!user) {
        qWarning() << "IsUserExist 传入空 User 智能指针";
        return false;
    }

    QSqlDatabase db = DBMagr::Instance().GetConnection(m_connName);
    if (!db.isOpen()) {
        qCritical() << "IsUserExist 数据库连接未打开 conn:" << m_connName;
        return false;
    }

    QSqlQuery query(db);
    query.prepare("select 1 from im_user where userid = ? LIMIT 1");
    query.addBindValue(user->GetUserId());

    if (DBMagr::Instance().ExecQuery(query) && query.next()) {
        return true;
    }

    return false;
}