#include "msgmodel.h"
#include "dbmagr.h"


MsgModel::MsgModel(const QString &connName) : m_connName(connName) {
    IsTableExit();
}

bool MsgModel::IsTableExit() {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS im_message (
            id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            userid INTEGER NOT NULL,
            friendid INTEGER NOT NULL,
            content TEXT NOT NULL,
            msgType INTEGER,
            createTime TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP
        )
    )";


    return DBMagr::Instance()->ExecQuery(query, sql);
}

bool MsgModel::AddMsg(const uint64_t id, const uint64_t friId, const Message& msg) {
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) return false;

    QSqlQuery insertQuery(db);
    insertQuery.prepare("insert into im_message(userid, friendid, content, msgType, createTime) values(?,?,?,?,?)");
    insertQuery.addBindValue(id);
    insertQuery.addBindValue(friId);
    insertQuery.addBindValue(msg.GetContent());
    insertQuery.addBindValue(msg.GetMsgType());
    insertQuery.addBindValue(msg.GetTime());

    if (DBMagr::Instance()->ExecQuery(insertQuery)) {
        db.commit();
        return true;
    } else {
        db.rollback();
        return false;
    }
}