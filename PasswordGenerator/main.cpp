#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "database/workersmodel.h"
#include "database/dbworker.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    DbWorker worker;

    WorkersModel *model = new WorkersModel(&app);

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");

    engine.rootContext()->setContextProperty("workersModel", model);

    const QUrl url(QStringLiteral("qrc:/qml/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
