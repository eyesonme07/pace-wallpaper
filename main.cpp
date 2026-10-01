#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QQmlContext>
#include"header/qmlAdapter.h"
#include"header/Backend.h"


static QObject* backendProvider(QQmlEngine*, QJSEngine*)
{
    return new QMLAdapter();
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    app.setApplicationName("pace-wallpaper");
    app.setOrganizationName("eyesonme");

    Backend backend;

    Backend::createApplicationDir();
    Backend::copyWallpaperToAppDir();
    Backend::updateWallpaper();


    backend.setTimerDuration(10);
    backend.startTimerUpdate();

    qmlRegisterSingletonType<QMLAdapter>("Backend",1,0,"Backend",backendProvider);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("F1_Live_Companion", "Main");


    return QGuiApplication::exec();
}
