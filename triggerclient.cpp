#include "triggerclient.h"

#include <QNetworkDatagram>

TriggerClient::TriggerClient(QHostAddress addr, quint16 port,QObject *parent) : QObject(parent), socket(new QUdpSocket(this))
{
    socket->connectToHost(addr, port);
    connect(socket, &QUdpSocket::readyRead, this, &TriggerClient::handlePendingDatagrams);
    connect(socket, &QUdpSocket::connected, this, &TriggerClient::onConnected);
    socket->writeDatagram(QNetworkDatagram("hello"));
}

QString TriggerClient::mode() const
{
    return m_mode;
}

void TriggerClient::handlePendingDatagrams()
{
    while(socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();
        emit trigger();
    }
}

void TriggerClient::onConnected()
{
    m_mode = "Client mode, connected to " + socket->peerAddress().toString() + ":" + QString::number(socket->peerPort());
    emit modeChanged();
}
