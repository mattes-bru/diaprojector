#ifndef TRIGGERCLIENT_H
#define TRIGGERCLIENT_H

#include <QObject>
#include <QUdpSocket>

class TriggerClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
public:
    explicit TriggerClient(QHostAddress addr, quint16 port, QObject *parent = nullptr);

    QString mode() const;

signals:
    void trigger();
    void modeChanged();


public slots:

private:
    QUdpSocket *socket = nullptr;
    QString m_mode = QStringLiteral("client mode");

private slots:
    void handlePendingDatagrams();
    void onConnected();
};

#endif // TRIGGERCLIENT_H
