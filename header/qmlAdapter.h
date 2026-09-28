#ifndef QMLADAPTER_H
#define QMLADAPTER_H

#include<QObject>
#include"wallpaper.h"


//переходник к QML
class QMLAdapter:public QObject{
    Q_OBJECT

public:
    Q_INVOKABLE bool setWallpalaer(const QUrl &path){
        return wallpaper.setWallpaper(path);
    }


private:
    Wallpaper wallpaper;

};

#endif // QMLADAPTER_H
