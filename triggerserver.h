#ifndef TRIGGERSERVER_H
#define TRIGGERSERVER_H

#include <QObject>
#include <QUdpSocket>


class TriggerServer : public QObject
{
    Q_OBJECT
     Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
public:
    explicit TriggerServer(int interval, quint16 port, QObject *parent = nullptr);

    QString mode() const;

signals:
    void trigger();
    void modeChanged();

public slots:

private:
    struct Peer {
        Peer(QHostAddress addr, quint16 port): addr(addr), port(port) {}
        QHostAddress addr;
        quint16 port;
    };

    QList<Peer> peers;
    QUdpSocket *socket = nullptr;

    QString m_mode = QStringLiteral("Server mode");
    QString m_modeProto = QStringLiteral("Server mode [Port: %1] with %2 clients connected");



private slots:
    void handlePendingDatagrams();

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // TRIGGERSERVER_H
