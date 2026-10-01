#ifndef BACKEND_H
#define BACKEND_H

//тут будет все про программу (все связывает)

#include<QDir>
#include"F1_Request.h"
#include"PrepareImage.h"
#include"settings.h"
#include"wallpaper.h"
#include<opencv2/opencv.hpp>
#include<QTimer>
#include<QObject>
#include<chrono>
using namespace std::chrono_literals;

class Backend:public QObject{
    Q_OBJECT
public:
    Backend();

    static void createApplicationDir();

    //берем обои и копируем в AppData
    static void copyWallpaperToAppDir();

    //запускаем таймер на автообновление обоев
    void startTimerUpdate();

    void setTimerDuration(const unsigned short &durationMin);
public slots:
    //тут мы берем текущие обои и добавляем информации
    static void updateWallpaper();
private:
    QTimer timer;
};

#endif // BACKEND_H
