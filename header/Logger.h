#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <QThread>
#include <QDir>
#include <QStandardPaths>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(lcNet)

namespace Logger {

inline QMutex g_mutex;
inline QFile g_file;
inline QtMessageHandler g_prevHandler = nullptr;
constexpr qint64 kMaxSize = 2 * 1024 * 1024;

inline void rotateIfNeeded()
{
    if (g_file.size() < kMaxSize) return;
    const QString path = g_file.fileName();
    g_file.close();
    QFile::remove(path + ".1");
    QFile::rename(path, path + ".1");
    g_file.setFileName(path);
    g_file.open(QIODevice::Append | QIODevice::Text);
}

inline void handler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    const char *level = "INFO";
    switch (type) {
    case QtDebugMsg:    level = "DEBUG"; break;
    case QtInfoMsg:     level = "INFO";  break;
    case QtWarningMsg:  level = "WARN";  break;
    case QtCriticalMsg: level = "ERROR"; break;
    case QtFatalMsg:    level = "FATAL"; break;
    }

    const QString line = QString("%1 [%2] [thr %3] %4: %5\n")
                             .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                             .arg(level)
                             .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()))
                             .arg(ctx.category ? ctx.category : "default")
                             .arg(msg);

    {
        QMutexLocker lock(&g_mutex);
        if (g_file.isOpen()) {
            rotateIfNeeded();
            g_file.write(line.toUtf8());
            g_file.flush();
        }
    }


    if (g_prevHandler) g_prevHandler(type, ctx, msg);
}

inline void init()
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(dir);
    g_file.setFileName(dir + "/logs/app.log");
    g_file.open(QIODevice::Append | QIODevice::Text);

    g_prevHandler = qInstallMessageHandler(handler);
}

}

#endif // LOGGER_H
