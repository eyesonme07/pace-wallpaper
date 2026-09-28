#include"../header/wallpaper.h"

bool Wallpaper::setWallpaper(const QUrl &path)
{
    QString absolutePath = path.toLocalFile();

    return setWallpaper(absolutePath.toStdString());
}

bool Wallpaper::setWallpaper(const std::string &path)
{
    QFileInfo info(QString::fromStdString(path));

    if(!info.exists()){
        qWarning()<<"File not found";
        return false;
    }

    QString absPath = QDir::toNativeSeparators(info.absoluteFilePath());

    std::wstring wpath = absPath.toStdWString();

    BOOL ok = SystemParametersInfoW(SPI_SETDESKWALLPAPER,0,(PVOID)wpath.c_str(),SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    return ok;
}

