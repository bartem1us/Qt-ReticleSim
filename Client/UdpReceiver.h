#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class UdpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UdpReceiver(QObject *parent = nullptr);
    void startListening(const QHostAddress &address, quint16 port);

signals:
    void dataReceived(qreal angle, qreal xOffset, qreal yOffset);

private slots:
    void onReadyRead();

private:

    std::unique_ptr<QUdpSocket> udpSocket_;

};

#endif // UDPRECEIVER_H
