#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "dbworker.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    DbWorker worker;

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");

    const QUrl url(QStringLiteral("qrc:/qml/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &a, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return a.exec();
}
