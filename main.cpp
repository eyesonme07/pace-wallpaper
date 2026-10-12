#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QQmlContext>
#include<thread>
#include"header/qmlAdapter.h"
#include"header/Backend.h"
#include"header/Logger.h"

Q_LOGGING_CATEGORY(lcNet, "app.net")

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
    Logger::init();

    Backend backend;
    backend.createApplicationDir();

    QtConcurrent::run([&](){
        backend.copyWallpaperToAppDir();
        backend.updateWallpaperAsync();
    });

    backend.setTimerDuration(15);
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