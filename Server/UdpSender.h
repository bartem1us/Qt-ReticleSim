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
signals:
    void dataSent();
private:

    std::unique_ptr<QUdpSocket> udpSocket_;

    const QHostAddress clientAddress_ = QHostAddress::LocalHost;

    const quint16 clientPort_ = 12345;  // Порт, на который отправляем данные
};

#endif // UDPSENDER_H
