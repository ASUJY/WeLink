#include "dbmagr.h"
#include <QDebug>
#include <QSqlError>

DBMagr::DBMagr() {}

DBMagr::~DBMagr() {
    for (auto &db : m_connections) {
        if (db.isOpen()) {
            db.close();
        }
    }
    m_connections.clear();
}

bool DBMagr::OpenConnection(const QString &connName, const QString &dbPath) {
    if (m_connections.contains(connName)) {
        auto oldconn = m_connections.take(connName);
        if (oldconn.isOpen()) {
            oldconn.close();
        }
        QSqlDatabase::removeDatabase(connName); // 移除驱动中的连接
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qDebug() << "打开数据库失败:" << db.lastError().text();
        return false;
    }
    m_connections.insert(connName, db);
    qDebug() << "连接" << connName << "打开成功";
    return true;
}

void DBMagr::CloseConnection(const QString &connName) {
    if (m_connections.contains(connName)) {
        auto db = m_connections.take(connName);
        if (db.isOpen()) {
            db.close();
        }
        QSqlDatabase::removeDatabase(connName);
    }
}

DBMagr* DBMagr::Instance() {
    static DBMagr db;
    return &db;
}

QSqlDatabase DBMagr::GetConnection(const QString &connName) const {
    return m_connections.value(connName);
}

bool DBMagr::ExecQuery(QSqlQuery &query) const {
    if (!query.exec()) {
        qDebug() << "执行sql失败" << query.lastError().text();
        return false;
    }
    return true;
}

bool DBMagr::ExecQuery(QSqlQuery &query, QString sql) const {
    if (!query.exec(sql)) {
        qDebug() << "执行sql失败" << query.lastError().text();
        return false;
    }
    return true;
}