#ifndef QMLADAPTER_H
#define QMLADAPTER_H

#include<QObject>
#include"wallpaper.h"
#include"Backend.h"

//переходник C++ к QML
class QMLAdapter:public QObject{
    Q_OBJECT

public:
    Q_INVOKABLE bool setWallpalaer(const QUrl &path){
        return wallpaper.setWallpaper(path);
    }
    Q_INVOKABLE QUrl getWallpaperPath(){return wallpaper.getWallpaperPath();};
    Q_INVOKABLE void updateWallpaper(){
        QtConcurrent::run([&](){
            backend.updateWallpaperAsync();
        });
    };


private:
    Wallpaper wallpaper;
    Backend backend;

};

#endif // QMLADAPTER_H
