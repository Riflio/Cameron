#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "appcore.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    AppCore * appCore = new AppCore(engine.rootObjects().at(0));

    return app.exec();
}
