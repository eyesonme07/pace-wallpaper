#include"../header/Backend.h"

Backend::Backend()
{
    connect(&timer,&QTimer::timeout,this,&Backend::updateWallpaperAsync);
    timer.setInterval(15min);
}

//создаем каталог в AppData/Local/eyesonme/pace-wallpaper и в Document
void Backend::createApplicationDir()
{
    QString localDataPath = QString::fromStdString(settings().getApplicationDir());

    if (localDataPath.isEmpty()) {
        qWarning() << "Не удалось определить путь для локальных данных";
        return;
    }

    QDir dir(localDataPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Не удалось создать директорию: " << localDataPath;
            std::terminate();
        }
    }

    const QStringList subDirs = { "logs", "data/images", "settings" };
    for (const QString &sub : subDirs) {
        if (!dir.mkpath(sub)) {
            qWarning() << "Не удалось создать поддиректорию:" << sub;
        }
    }


    QDir doc(QString::fromStdString(settings().getDocumentDir())+"/pace-wallpaper");

    if(!doc.exists()){
        if(!doc.mkpath(".")){
            qWarning()<<"Не удалось создать директорию";
            std::terminate();
        }
    }
}

//берем clear и добавляем в него инфы потом сохраняем под названием new
void Backend::updateWallpaper()
{

    Wallpaper w;
    PrepareImage p;

    QString imagesDir = QString::fromStdString(settings().getApplicationDir()) + "/data/images";

    QDir dir(imagesDir);
    const QStringList found = dir.entryList({"clear.*"}, QDir::Files);
    if (found.isEmpty()) {
        qWarning() << "Оригинал обоев не найден";
        return;
    }

    QString clearWallpaperPath = imagesDir + "/" + found.first();

    QString suffix = QFileInfo(clearWallpaperPath).suffix();
    QString newWallpaperPath = imagesDir + "/new." + suffix;

    cv::Mat currentWallpaper = cv::imread(clearWallpaperPath.toStdString());
    if (currentWallpaper.empty()) {
        qWarning() << "OpenCV не смог прочитать картинку";
        return;
    }

    p.prepareImage(currentWallpaper, newWallpaperPath);

    w.setWallpaper(newWallpaperPath.toStdString());
}

//берем обои и копруем в наш каталог чтобы пользователь не удалил случайно
void Backend::copyWallpaperToAppDir()
{
    Wallpaper w;
    QFile wallpaperFile(w.getWallpaperPath().toLocalFile());
    QFileInfo info(wallpaperFile);

    QString imagesDir = QString::fromStdString(settings().getApplicationDir()) + "/data/images";
    QDir().mkpath(imagesDir);

    QString srcDir = QDir::cleanPath(info.absolutePath());
    QString ourDir = QDir::cleanPath(QDir(imagesDir).absolutePath());
    if (srcDir.compare(ourDir, Qt::CaseInsensitive) == 0) {
        return;
    }

    QString newPath = imagesDir + "/clear." + info.suffix();

    QString tmpPath = imagesDir + "/clear_tmp." + info.suffix();
    QFile::remove(tmpPath);
    if (!wallpaperFile.copy(tmpPath)) {
        qWarning() << "Не удалось скопировать обои:" << wallpaperFile.errorString();
        return;
    }

    QDir dir(imagesDir);
    const QStringList oldFiles = dir.entryList({"clear.*"}, QDir::Files);
    for (const QString &f : oldFiles)
        dir.remove(f);

    QFile::rename(tmpPath, newPath);

    Wallpaper::setWallpaper(newPath.toStdString());
}

void Backend::setTimerDuration(const unsigned short &durationMin)
{
    timer.setInterval(std::chrono::minutes(durationMin));
}

void Backend::updateWallpaperAsync()
{
    QtConcurrent::run([](){
        if(!mutex.tryLock())
            return;
        updateWallpaper();
        mutex.unlock();
    });
}

void Backend::startTimerUpdate()
{
    timer.start();
}