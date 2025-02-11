#include "UdpSender.h"

UdpSender::UdpSender(QObject *parent)
    : QObject{parent}, udpSocket_(std::make_unique<QUdpSocket>(this)),
    clientAddress_(QHostAddress::LocalHost) , clientPort_(12345)
{}

void UdpSender::sendData(qreal angle, qreal xOffset, qreal yOffset)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << angle << xOffset << yOffset;  // Записываем данные в поток

    udpSocket_.get()->writeDatagram(data, clientAddress_, clientPort_);

}
