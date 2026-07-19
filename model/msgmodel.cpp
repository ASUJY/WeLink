#include "msgmodel.h"
#include "dbmagr.h"
#include <QSqlError>

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

bool MsgModel::AddMsg(const int64_t id, const int64_t friId, const Message& msg) {
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

std::vector<Message> MsgModel::FindMsg(const int64_t id, const int64_t friId) {
    std::vector<Message> msgVec;
    QSqlDatabase db = DBMagr::Instance()->GetConnection(m_connName);
    if (!db.isOpen()) {
        qDebug() << "数据库连接未打开";
        return msgVec;
    }
    QSqlQuery query(db);
    QString sql = "select * from im_message where userid = ? and friendid = ?";
    query.prepare(sql);
    query.addBindValue(id);
    query.addBindValue(friId);

    if (!DBMagr::Instance()->ExecQuery(query)) {
        qDebug() << "查询失败：" << query.lastError().text();
        return msgVec;
    }
    while (query.next()) {
        Message msg;
        msg.SetContent(query.value("content").toString());
        if (query.value("msgType").toInt() == 1) {
            msg.SetMsgType(MsgType::Sender);
        } else if (query.value("msgType").toInt() == 2){
            msg.SetMsgType(MsgType::Receive);
        }
        msg.SetTime(query.value("createTime").toString());
        msgVec.push_back(msg);
    }
    return msgVec;
}