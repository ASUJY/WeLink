#include "friendmodel.h"
#include "dbmagr.h"
#include <QSqlQuery>
#include <QSqlError>

FriendModel::FriendModel(const QString &connName) : m_connName(connName){
    IsTableExit();
}

bool FriendModel::IsTableExit() {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS im_friend (
            userid INTEGER NOT NULL,
            friendid INTEGER NOT NULL,
            friendname TEXT,
            account TEXT UNIQUE,
            phone TEXT UNIQUE,
            avatar TEXT,
            PRIMARY KEY (userid, friendid)
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

std::vector<Friend> FriendModel::FindFriends(const uint64_t id) {
    qDebug() << "FriendModel::FindFriends";
    std::vector<Friend> friendsVec;
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) {
        qDebug() << "数据库连接未打开";
        return friendsVec;
    }
    QSqlQuery query(db);
    QString sql = "select * from im_friend where userid = ?";
    query.prepare(sql);
    query.addBindValue(id);
    qDebug() << "SQL模板:" << query.lastQuery();
    qDebug() << "绑定参数:" << query.boundValues();

    if (!DBMagr::Instance()->ExecQuery(query)) {
        qDebug() << "查询失败：" << query.lastError().text();
        return friendsVec;
    }
    while (query.next()) {
        Friend fri;
        fri.SetUserId(query.value("friendid").toLongLong());
        fri.SetUserName(query.value("friendname").toString().toStdString());
        friendsVec.push_back(fri);
    }
    return friendsVec;
}