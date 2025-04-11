#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "database/workersmodel.h"
#include "database/dbworker.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/img/icon.ico"));

    WorkersModel *model = new WorkersModel(&app);
    ProxyWorkersModel *proxy = new ProxyWorkersModel(model);

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    QQuickStyle::setStyle("Basic");

    engine.rootContext()->setContextProperty("workersModel", model);
    engine.rootContext()->setContextProperty("proxyWorkers", proxy);

    const QUrl url(QStringLiteral("qrc:/qml/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
