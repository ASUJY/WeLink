#include "friendrequestmodel.h"
#include "dbmagr.h"
#include <QSqlQuery>
#include <QSqlError>

FriendRequestModel:: FriendRequestModel(const QString &connName) : m_connName(connName) {
    IsTableExit();
}


bool FriendRequestModel::IsTableExit() const{
    QSqlDatabase db = DBMagr::Instance().GetConnection(m_connName);
    if (!db.isOpen()) {
        qCritical() << "FriendRequestModel 连接失效 conn:" << m_connName;
        return false;
    }

    QSqlQuery query(db);
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS im_friendrequest (
            userid INTEGER NOT NULL,
            friendid INTEGER NOT NULL,
            friendname TEXT,
            account TEXT UNIQUE,
            phone TEXT UNIQUE,
            avatar TEXT,
            status INTEGER NOT NULL,
            PRIMARY KEY (userid, friendid)
        )
    )";
    qDebug() << "FriendRequestModel::IsTableExit()";

    return DBMagr::Instance().ExecQuery(query, sql);
}

bool FriendRequestModel::AddItem(const int64_t id, const User &fri, FriendState status) {
    QSqlDatabase db = DBMagr::Instance().GetConnection(m_connName);
    if (!db.isOpen()) {
        qCritical() << "AddItem 数据库未打开 conn:" << m_connName;
        return false;
    }

    if (!db.transaction()) {
        qCritical() << "AddItem 开启事务失败:" << db.lastError().text();
        return false;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare("insert into im_friendrequest(userid, friendid, friendname, phone, status) values(?,?,?,?,?)");
    insertQuery.addBindValue(id);
    insertQuery.addBindValue(fri.GetUserId());
    insertQuery.addBindValue(QString::fromStdString(fri.GetUserName()));
    insertQuery.addBindValue(QString::fromStdString(fri.GetUserPhone()));
    insertQuery.addBindValue(static_cast<int>(status));
    qDebug() << "SQL模板:" << insertQuery.lastQuery();
    qDebug() << "绑定参数:" << insertQuery.boundValues();
    if (DBMagr::Instance().ExecQuery(insertQuery)) {
        db.commit();
        return true;
    } else {
        db.rollback();
        qCritical() << "AddItem 新增好友申请失败，事务回滚";
        return false;
    }
}

bool FriendRequestModel::UpdateItemStatus(const int64_t id, const int64_t friendid, FriendState status) {
    QSqlDatabase db = DBMagr::Instance().GetConnection(m_connName);
    if (!db.isOpen()) {
        qCritical() << "UpdateItemStatus 数据库未打开 conn:" << m_connName;
        return false;
    }

    if (!db.transaction()) {
        qCritical() << "UpdateItemStatus 开启事务失败";
        return false;
    }

    QSqlQuery updateQuery(db);
    const QString sql = R"(
        UPDATE im_friendrequest
        SET status = :status
        WHERE userid = :uid AND friendid = :fid
    )";

    updateQuery.prepare(sql);
    // 绑定参数
    updateQuery.bindValue(":status", static_cast<int>(status));
    updateQuery.bindValue(":uid", id);
    updateQuery.bindValue(":fid", friendid);
    qDebug() << "SQL模板:" << updateQuery.lastQuery();
    qDebug() << "绑定参数:" << updateQuery.boundValues();
    if (DBMagr::Instance().ExecQuery(updateQuery)) {
        db.commit();
        return true;
    } else {
        db.rollback();
        qCritical() << "UpdateItemStatus 更新申请状态失败，事务回滚";
        return false;
    }
}