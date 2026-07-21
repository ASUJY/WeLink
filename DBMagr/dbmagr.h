#ifndef DBMAGR_H
#define DBMAGR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>

class DBMagr
{

public:
    DBMagr(const DBMagr&) = delete;
    DBMagr& operator=(const DBMagr&) = delete;
    DBMagr(DBMagr&&) = delete;
    DBMagr& operator=(DBMagr&&) = delete;

    static DBMagr& Instance() noexcept {
        static DBMagr instance;
        return instance;
    }

    bool Init();
    bool OpenConnection(const QString &connName, const QString &dbPath);
    void CloseConnection(const QString &connName);

    QSqlDatabase GetConnection(const QString &connName) const;
    bool ExecQuery(QSqlQuery &query) const;
    bool ExecQuery(QSqlQuery &query, const QString& sql) const;

private:
    DBMagr() = default;
    ~DBMagr();
    void CloseAllConnections() noexcept;    // 关闭所有连接

private:
    mutable QMutex m_mutex;
    QMap<QString, QSqlDatabase> m_connections;

};

#endif // DBMAGR_H
