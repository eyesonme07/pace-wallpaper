#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include"header/F1_Request.h"
#include<thread>
#include<chrono>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;


    DriverParser par;


    std::vector<Driver> list = par.getDriversList();

    for(const auto &el:list){
        qDebug()<<"Id - "<<el.id;
        qDebug()<<"Driver Name - "<<el.firstName<<" "<<el.lastName;
        qDebug()<<"Number - "<<el.nuumber;
        qDebug()<<"DirthDate - "<<el.birthDate;
        qDebug()<<"tla - "<<el.tla;

        qDebug()<<"---------------------------------------";
    }

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("F1_Live_Companion", "Main");


    return QGuiApplication::exec();
}
