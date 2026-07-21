#include "dbmagr.h"
#include <QDebug>
#include <QSqlError>

DBMagr::~DBMagr() {
    CloseAllConnections();
}

bool DBMagr::Init() {
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qCritical() << "SQLite 驱动不可用！";
        return false;
    }
    return true;
}

bool DBMagr::OpenConnection(const QString &connName, const QString &dbPath) {
    // 连接已存在，直接返回
    if (m_connections.contains(connName)) {
        auto& oldconn = m_connections[connName];
        if (oldconn.isOpen()) {
            return true;
        }
        // 连接存在但未打开，重新打开
        oldconn.setDatabaseName(dbPath);
        if (!oldconn.open()) {
            qCritical() << "重新打开连接失败：" << connName << "，错误：" << oldconn.lastError().text();
            return false;
        }
        return true;
    }

    // 连接不存在，创建新连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qCritical() << "打开数据库失败：" << connName << "，路径：" << dbPath << "，错误：" << db.lastError().text();
        return false;
    }

    m_connections.insert(connName, db);
    qInfo() << "数据库连接成功：" << connName;
    return true;
}

void DBMagr::CloseConnection(const QString &connName) {
    QMutexLocker locker(&m_mutex);

    if (m_connections.contains(connName)) {
        auto& db = m_connections[connName];
        if (db.isOpen()) {
            db.close();
        }
        QSqlDatabase::removeDatabase(connName);
        m_connections.remove(connName);
    }
}

QSqlDatabase DBMagr::GetConnection(const QString &connName) const {
    QMutexLocker locker(&m_mutex);
    if (m_connections.contains(connName)) {
        return m_connections[connName];
    }

    // 返回无效对象（调用处可通过 isOpen() 判断）
    return QSqlDatabase();
}

bool DBMagr::ExecQuery(QSqlQuery &query) const {
    if (!query.exec()) {
        qCritical() << "SQL 执行失败：" << query.lastError().text() << "，SQL：" << query.executedQuery();
        return false;
    }
    return true;
}

bool DBMagr::ExecQuery(QSqlQuery &query, const QString& sql) const {
    if (!query.exec(sql)) {
        qCritical() << "SQL 执行失败：" << query.lastError().text() << "，SQL：" << query.executedQuery();
        return false;
    }
    return true;
}

void DBMagr::CloseAllConnections() noexcept
{
    QMutexLocker locker(&m_mutex);

    for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
        auto& db = it.value();
        if (db.isOpen()) {
            db.close();
        }
        QSqlDatabase::removeDatabase(it.key());
    }

    m_connections.clear();
}