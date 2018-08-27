#include "triggerserver.h"

#include <QNetworkDatagram>

TriggerServer::TriggerServer(int interval, quint16 port, QObject *parent) : QObject(parent), socket(new QUdpSocket(this))
{
    socket->bind(QHostAddress::Any, port );
    connect(socket, &QUdpSocket::readyRead, this, &TriggerServer::handlePendingDatagrams);

    using namespace std::chrono;
    startTimer(seconds(interval),Qt::PreciseTimer );

    m_mode = m_modeProto.arg(socket->localPort()).arg(peers.size());


}

QString TriggerServer::mode() const
{
    return m_mode;
}

void TriggerServer::handlePendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();
        Peer p(datagram.senderAddress(), datagram.senderPort());
        peers.append(p);
        qDebug() << "added client" << p.addr << p.port;
    }

    m_mode = m_modeProto.arg(socket->localPort()).arg(peers.size());
    emit modeChanged();

}

void TriggerServer::timerEvent(QTimerEvent *event)
{
    foreach(Peer p , peers) {
        socket->writeDatagram("n", p.addr, p.port);
    }
    emit trigger();

}
