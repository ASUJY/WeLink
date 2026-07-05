#include "friendmodel.h"
#include "dbmagr.h"
#include <QSqlQuery>

FriendModel::FriendModel(const QString &connName) : m_connName(connName){
    IsTableExit();
}

bool FriendModel::IsTableExit() {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS im_friend (
            userid INTEGER NOT NULL PRIMARY KEY,
            friendid INTEGER NOT NULL,
            friendname TEXT,
            account TEXT UNIQUE,
            phone TEXT UNIQUE,
            avatar TEXT
        )
    )";


    return DBMagr::Instance()->ExecQuery(query, sql);
}

bool FriendModel::AddFriend(const uint64_t id, const Friend &fri) {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery insertQuery(db);
    insertQuery.prepare("insert into im_friend(userid, friendid, friendname) values(?,?,?)");
    insertQuery.addBindValue(id);
    insertQuery.addBindValue(fri.GetUserId());
    insertQuery.addBindValue(QString::fromStdString(fri.GetUserName()));
    qDebug() << "SQL模板:" << insertQuery.lastQuery();
    qDebug() << "绑定参数:" << insertQuery.boundValues();
    if (DBMagr::Instance()->ExecQuery(insertQuery)) {
        db.commit();
        return true;
    } else {
        db.rollback();
        return false;
    }
}

bool FriendModel::IsFriendExit(const uint64_t id, const Friend &fri) {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) {
        qDebug() << "数据库连接未打开";
        return false;
    }

    QSqlQuery query(db);
    QString sql = "select 1 from im_friend where userid = ? and friendid = ? limit 1";
    query.prepare(sql);
    query.addBindValue(id);
    query.addBindValue(fri.GetUserId());
    qDebug() << "SQL模板:" << query.lastQuery();
    qDebug() << "绑定参数:" << query.boundValues();
    if (DBMagr::Instance()->ExecQuery(query) && query.next()) {
        qDebug() << "查询好友SQL成功";
        return true;
    }

    qDebug() << "查询好友SQL失败";
    return false;
}