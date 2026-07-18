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
            state INTEGER NOT NULL,
            PRIMARY KEY (userid, friendid)
        )
    )";


    return DBMagr::Instance()->ExecQuery(query, sql);
}

bool FriendModel::AddFriend(const uint64_t id, const User &fri, FriendState state) {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery insertQuery(db);
    insertQuery.prepare("insert into im_friend(userid, friendid, friendname, phone) values(?,?,?,?)");
    insertQuery.addBindValue(id);
    insertQuery.addBindValue(fri.GetUserId());
    insertQuery.addBindValue(QString::fromStdString(fri.GetUserName()));
    insertQuery.addBindValue(QString::fromStdString(fri.GetUserPhone()));
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
    QString sql = R"(select 1 from im_friend where userid = ? and friendid = ? limit 1)";
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

User FriendModel::IsFriendExit(const uint64_t id, const QString &name, E_ACCOUNT_TYPE type) {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) {
        qDebug() << "数据库连接未打开";
        return User{};
    }

    QSqlQuery query(db);
    QString sql = "";
    if (type == E_ACCOUNT_TYPE::NICKNAME) {
        sql = R"(select * from im_friend where userid = ? and friendname = ? limit 1)";
    } else if (type == E_ACCOUNT_TYPE::PHONE) {
        sql = R"(select * from im_friend where userid = ? and phone = ? limit 1)";
    }
    query.prepare(sql);
    query.addBindValue(id);
    query.addBindValue(name);

    if (!DBMagr::Instance()->ExecQuery(query)) {
        qDebug() << "查询好友SQL失败: " << query.lastQuery() << ", 查询参数: " << query.boundValues();
        return User{};
    }

    if (query.next()) {
        User user;
        user.SetUserId(query.value("friendid").toULongLong());
        user.SetUserName(query.value("friendname").toString().toStdString());
        user.SetUserPhone(query.value("phone").toString().toStdString());
        return user;
    }

    return User{};
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


bool FriendModel::UpdateFriendState(const uint64_t id, const uint64_t friendid, FriendState type) {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery updateQuery(db);
    const QString sql = R"(
        UPDATE im_friend
        SET state = :state
        WHERE userid = :uid AND friendid = :fid
    )";

    updateQuery.prepare(sql);
    // 绑定参数
    updateQuery.bindValue(":state", static_cast<int>(type));
    updateQuery.bindValue(":uid", id);
    updateQuery.bindValue(":fid", friendid);

    if (DBMagr::Instance()->ExecQuery(updateQuery)) {
        db.commit();
        return true;
    } else {
        db.rollback();
        return false;
    }
}