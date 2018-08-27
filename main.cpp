#include <QApplication>
#include <QQmlApplicationEngine>
#include "triggerclient.h"
#include "triggerserver.h"
#include <QtQml>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QCommandLineParser parser;
    app.setApplicationVersion("2.0");
    parser.setApplicationDescription("synchronized image viewer");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
                          {{"c" , "client"},
                           "run in client mode with specified master",
                          "client"},
                          {{"p" , "port"},
                           "UDP port, used by the master application",
                          "port",
                          "5555"},
                          {{"i" , "interval"},
                           "image change interval (ignored in client mode)",
                          "interval",
                          "1"}
                      });

    parser.process(app);

    bool parseOk;
    quint16 port = parser.value("port").toInt(&parseOk);

    if(!parseOk) {
        qFatal("Invalid port given!");
    }

    int interval = parser.value("interval").toInt(&parseOk);
    if(!parseOk) {
        qFatal("Invalid interval given!");
    }

    QQmlApplicationEngine engine;
    QObject *trigger = nullptr;
    if(parser.isSet("client")) {
        QString masterAddress = parser.value("client");
        qDebug() << "starting client with master at" << masterAddress << port;
        trigger = new TriggerClient(QHostAddress(masterAddress), port);
    } else {
        qDebug() << "starting master listening at port " << port;
        trigger = new TriggerServer(interval, port);

    }

    engine.rootContext()->setContextProperty("trigger", trigger);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
