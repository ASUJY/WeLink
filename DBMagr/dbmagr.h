#ifndef DBMAGR_H
#define DBMAGR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>

class DBMagr
{

public:
    ~DBMagr();
    static DBMagr* Instance();

    bool Init();
    bool OpenConnection(const QString &connName, const QString &dbPath);
    void CloseConnection(const QString &connName);

    QSqlDatabase GetConnection(const QString &connName) const;
    bool ExecQuery(QSqlQuery &query) const;
    bool ExecQuery(QSqlQuery &query, QString sql) const;

private:
    DBMagr();

private:
    QMap<QString, QSqlDatabase> m_connections;

};

#endif // DBMAGR_H
