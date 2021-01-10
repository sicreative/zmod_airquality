#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QQmlContext>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include "zmodhat.h"
#include "zmodqtdata.h"







int main(int argc, char *argv[])
{




    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;



    ZmodQtData zomdQtData;



    if (zomdQtData.startReadSensor(&app)){
        printf("Can't start read ZMOD sensor timer");
        return -1;
    }

    engine.rootContext()->setContextProperty("zmodQtData",&zomdQtData);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));





    if (engine.rootObjects().isEmpty())
        return -1;



    return app.exec();
}
