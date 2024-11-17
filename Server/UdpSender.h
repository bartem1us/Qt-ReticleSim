#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QObject>
#include <QUdpSocket>


class UdpSender : public QObject
{
    Q_OBJECT
public:
    explicit UdpSender(QObject *parent = nullptr);


public slots:
    void sendData(qreal angle, qreal xOffset, qreal yOffset);

private:

    std::unique_ptr<QUdpSocket> udpSocket_;

    const QHostAddress clientAddress_;

    const quint16 clientPort_;  // Порт, на который отправляем данные
};

#endif // UDPSENDER_H
