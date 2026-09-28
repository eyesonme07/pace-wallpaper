#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include"header/F1_Request.h"
#include<QQmlContext>
#include"header/qmlAdapter.h"


static QObject* backendProvider(QQmlEngine*, QJSEngine*)
{
    return new QMLAdapter();
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

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
