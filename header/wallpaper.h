#ifndef WALLPAPER_H
#define WALLPAPER_H


#include<QUrl>
#include<QDir>
#include<QFile>
#include<opencv2/opencv.hpp>
#include<QImage>


//класс для кстановки обоев
class Wallpaper{
public:
    //установка обоев
    static bool setWallpaper(const QUrl &path);
    static bool setWallpaper(const std::string &path);

    QUrl getWallpaperPath();


private:

};

#endif // WALLPAPER_H
