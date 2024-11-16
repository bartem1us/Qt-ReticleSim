#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QBoxLayout>
#include <QUdpSocket>
#include <QSvgRenderer>
#include <QThread>
#include "UdpReceiver.h"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

    void resizeEvent(QResizeEvent *event) override;
private slots:
    void onDataReceived(qreal angle, qreal xOffset, qreal yOffset);
    void onColorChanged();
    void onSightChanged();

private:
    void initializeReticles(QGraphicsScene *scene);

    Ui::ClientWindow *ui;

    QGraphicsSvgItem *redReticle_;

    qreal targetAspectRatio_;

    qreal width_;

    qreal height_;

    void scaleReticleAndCrosshair();

    QBoxLayout *layout;

    QGraphicsItemGroup *lineGroup;

    qreal angle_ = 2;
    qreal xOffset_ = 0;
    qreal yOffset_ = 0;

    static constexpr qreal coefficient_ = 360.0/6000.0;

    qreal scaleFactor_ = 1;

    QThread udpThread_;

    std::unique_ptr<UdpReceiver> udpReceiver_;

};

#endif // CLIENTWINDOW_H
