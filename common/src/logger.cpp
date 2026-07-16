#include "logger.h"
#include <iostream>

QMutex Logger::m_mutex;
QFile Logger::m_file;

void Logger::initLog(const QString &filePath)
{
    m_file.setFileName(filePath);
    m_file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text);
    qInstallMessageHandler(Logger::logHandler);
}

void Logger::unInitLog()
{
    qInstallMessageHandler(nullptr);
    if(m_file.isOpen())
        m_file.close();
}

void Logger::logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker lock(&m_mutex);
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString level;
    switch (type) {
        case QtDebugMsg:    level = "DEBUG"; break;
        case QtInfoMsg:     level = "INFO";  break;
        case QtWarningMsg:  level = "WARN";  break;
        case QtCriticalMsg: level = "ERROR"; break;
        case QtFatalMsg:    level = "FATAL"; break;
        default: level = "UNKNOWN";
    }
        QString logText = QString("[%1] [%2] %3:%4 - %5\n")
                              .arg(time)
                              .arg(level)
                              .arg(context.file ? context.file : "unknown")
                              .arg(context.line)
                              .arg(msg);
    m_file.write(logText.toUtf8());
    m_file.flush();
    std::cout << logText.toLocal8Bit().constData();
    if(type == QtFatalMsg) abort();
}
