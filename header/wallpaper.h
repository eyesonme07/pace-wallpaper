#ifndef WALLPAPER_H
#define WALLPAPER_H


#include<QUrl>
#include<QDir>
#include<QFile>
#include<windows.h>


//класс для кстановки обоев
class Wallpaper{
public:
    //установка обоев
    bool setWallpaper(const QUrl &path);
    bool setWallpaper(const std::string &path);
private:

};

#endif // WALLPAPER_H
