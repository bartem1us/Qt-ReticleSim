#include "UdpReceiver.h"

UdpReceiver::UdpReceiver(QObject *parent)
    : QObject(parent), udpSocket_(std::make_unique<QUdpSocket>(this)) {}

void UdpReceiver::startListening(const QHostAddress &address, quint16 port)
{
    if (!udpSocket_.get()->bind(address, port)) {
        qWarning() << "Failed to bind UDP socket:" << udpSocket_->errorString();
        return ;
    }
    connect(udpSocket_.get(), &QUdpSocket::readyRead, this, &UdpReceiver::onReadyRead);
}

void UdpReceiver::onReadyRead()
{
    while (udpSocket_.get()->hasPendingDatagrams())
    {
        QByteArray datagram(udpSocket_.get()->pendingDatagramSize(),Qt::Uninitialized);

        udpSocket_.get()->readDatagram(datagram.data(), datagram.size());

        QDataStream stream(datagram);
        qreal angle, xOffset, yOffset;
        stream >> angle >> xOffset >> yOffset;

        emit dataReceived(angle, xOffset, yOffset);
    }
}
