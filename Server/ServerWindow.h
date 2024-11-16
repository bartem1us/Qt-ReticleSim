#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QThread>
#include "UdpSender.h"
#include "./ui_ServerWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ServerWindow;
}
QT_END_NAMESPACE

class ServerWindow : public QWidget
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();
private slots:
    void sendTelemetry();
    void updateAngleLabel(int value);
    void updateXOffsetLabel(int value);
    void updateYOffsetLabel(int value);

private:
    Ui::ServerWindow *ui;

    QThread udpThread_;

    std::unique_ptr<UdpSender> udpSender_;
};
#endif // SERVERWINDOW_H
