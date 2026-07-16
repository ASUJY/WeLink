#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <QMessageLogContext>

class Logger
{
public:
    static void initLog(const QString& filePath = "app.log");
    static void unInitLog();

private:
    static void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QMutex m_mutex;
    static QFile m_file;
};

#endif // LOGGER_H
