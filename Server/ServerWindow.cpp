#include "ServerWindow.h"


ServerWindow::ServerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWindow)
    , udpSender_(std::make_unique<UdpSender>())
{
    ui->setupUi(this);

    udpSender_.get()->moveToThread(&udpThread_);

    connect(ui->sendButton, &QPushButton::clicked, this, &ServerWindow::sendTelemetry);
    connect(ui->angleSlider, &QSlider::valueChanged, this, &ServerWindow::updateAngleLabel);
    connect(ui->xOffsetSlider, &QSlider::valueChanged, this, &ServerWindow::updateXOffsetLabel);
    connect(ui->yOffsetSlider, &QSlider::valueChanged, this, &ServerWindow::updateYOffsetLabel);

    //connect(udpSender, &UdpSender::dataSent, &udpThread, &QThread::quit);

    udpThread_.start();

}

ServerWindow::~ServerWindow()
{
    udpThread_.quit();
    udpThread_.wait();
    delete ui;

}

void ServerWindow::sendTelemetry()
{
    qreal angle = ui->angleSlider->value() / 10.0;
    qreal xOffset = ui->xOffsetSlider->value() / 100.0;
    qreal yOffset = ui->yOffsetSlider->value() / 100.0;

    QMetaObject::invokeMethod(udpSender_.get(), "sendData", Qt::QueuedConnection,
                              Q_ARG(qreal, angle),
                              Q_ARG(qreal, xOffset),
                              Q_ARG(qreal, yOffset));

}

void ServerWindow::updateAngleLabel(int value)
{
    ui->labelValueAngleSlider->setText(QString::number(value/10.0));
}

void ServerWindow::updateXOffsetLabel(int value)
{
    ui->labelValueXOffsetSlider->setText(QString::number(value/100.0));
}

void ServerWindow::updateYOffsetLabel(int value)
{
    ui->labelValueYOffsetSlider->setText(QString::number(value/100.0));
}
