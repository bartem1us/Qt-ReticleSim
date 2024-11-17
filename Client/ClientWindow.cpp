#include "ClientWindow.h"
#include "ui_ClientWindow.h"
#include <QResizeEvent>
ClientWindow::ClientWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWindow)
    , udpThread_(new QThread(this))
    , udpReceiver_(std::make_unique<UdpReceiver>())
    , redReticle_(new QGraphicsSvgItem(":/image/image/crosshair_red.svg"))
{
    ui->setupUi(this);
    udpReceiver_.get()->moveToThread(&udpThread_);

    connect(udpReceiver_.get(), &UdpReceiver::dataReceived, this, &ClientWindow::onDataReceived);
    connect(ui->colorBackgroundChoose, &QComboBox::currentIndexChanged, this, &ClientWindow::onColorChanged);
    connect(ui->colorSightChoose, &QComboBox::currentIndexChanged, this, &ClientWindow::onSightChanged);
    connect(&udpThread_, &QThread::started, udpReceiver_.get(), [this]() {
        udpReceiver_->startListening(QHostAddress::LocalHost, 12345);
    });

    udpThread_.start();

    targetAspectRatio_ = ui->displayWidget->height()/ui->displayWidget->width();
    width_ = ui->graphicsView->width();
    height_ = ui->graphicsView->height();

    ui->graphicsView->setBackgroundBrush(QColor(Qt::black));

    QGraphicsScene *scene = new QGraphicsScene(this);

    scene->addItem(redReticle_);


    int viewCenterx = ui->graphicsView->width() / 2;
    int viewCentery = ui->graphicsView->height() / 2;

    QPen greenPen(Qt::green);
    greenPen.setWidth(1);
    QGraphicsLineItem *verticalLine = new QGraphicsLineItem(viewCenterx, viewCentery - 25, viewCenterx, viewCentery + 25);
    verticalLine->setPen(greenPen);

    QGraphicsLineItem *horizontalLine = new QGraphicsLineItem(viewCenterx - 25, viewCentery, viewCenterx + 25, viewCentery);
    horizontalLine->setPen(greenPen);


    lineGroup = new QGraphicsItemGroup();
    lineGroup->addToGroup(verticalLine);
    lineGroup->addToGroup(horizontalLine);

    scene->addItem(lineGroup);

    scene->setSceneRect(0, 0, 400, 300);
    ui->graphicsView->setScene(scene);
    scaleReticleAndCrosshair();

    layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->addItem(new QSpacerItem(0, 0));
    layout->addWidget(ui->displayWidget);

    layout->addItem(new QSpacerItem(0, 0));

}


void ClientWindow::resizeEvent(QResizeEvent *event) {
    float thisAspectRatio = (float)event->size().width() / event->size().height();
    int widgetStretch, outerStretch;

    height_ = ui->graphicsView->height();


    if (thisAspectRatio > targetAspectRatio_) // слишком широкий
    {
        layout->setDirection(QBoxLayout::LeftToRight);
        widgetStretch = height() * targetAspectRatio_; // т.е. ширина виджета
        outerStretch = (width() - widgetStretch) / 2 + 0.5;
    }
    else // слишком высокий
    {
        layout->setDirection(QBoxLayout::TopToBottom);
        widgetStretch = width() * targetAspectRatio_; // т.е. высота виджета
        outerStretch = (height() - widgetStretch) / 2 + 0.5;
    }


    layout->setStretch(0, outerStretch);
    layout->setStretch(1, widgetStretch);
    layout->setStretch(2, outerStretch);



    scaleFactor_ = scaleFactor_ * (ui->graphicsView->width()/width_);
    width_ = ui->graphicsView->width();
    redReticle_->setScale(scaleFactor_ * coefficient_ * angle_);
    scaleReticleAndCrosshair();

}

void ClientWindow::onColorChanged()
{
    QString color = ui->colorBackgroundChoose->currentText();
    QColor selectedColor;


    if (color == "Чёрный") {
        selectedColor = QColor(Qt::black);
    } else if (color == "Серый") {
        selectedColor = QColor(Qt::gray);
    } else {
        selectedColor = QColor(Qt::white);
    }

    ui->graphicsView->setBackgroundBrush(selectedColor);
}

void ClientWindow::onSightChanged()
{
    QString sightColor=ui->colorSightChoose->currentText();
    QString filePath;
    if (sightColor == "Чёрный") {
        filePath = ":/image/image/crosshair_black.svg";
    } else {
        filePath = ":/image/image/crosshair_red.svg";
    }
    redReticle_->setSharedRenderer(new QSvgRenderer(filePath, this));
}

void ClientWindow::onDataReceived(const qreal angle,const qreal xOffset,const qreal yOffset) {
    angle_ = angle;
    xOffset_ = xOffset;
    yOffset_ = yOffset;

    ui->labelValueAngle->setText(QString("Угол камеры: %1 град.").arg(angle_));
    ui->labelValueXOffset->setText(QString("Отступ по горизонтали:%1").arg(xOffset_));
    ui->labelValueYOffset->setText(QString("Отступ по вертикали: %1").arg(yOffset_));

    redReticle_->setScale(scaleFactor_ * coefficient_ * angle_);
    scaleReticleAndCrosshair();
}

void ClientWindow::scaleReticleAndCrosshair() {

    qreal halfWidth = (redReticle_->boundingRect().width() / 2) * scaleFactor_ * coefficient_ * angle_;
    qreal halfHeight = (redReticle_->boundingRect().height() / 2) * scaleFactor_ * coefficient_ * angle_;
    redReticle_->setPos((ui->graphicsView->width() / 2.0) - halfWidth +(ui->graphicsView->width() / 2) * xOffset_,
                       (ui->graphicsView->height() / 2.0) - halfHeight+(ui->graphicsView->height() / 2) * yOffset_);
    lineGroup->setPos((ui->graphicsView->width() / 2) - 200, (ui->graphicsView->height() / 2) - 150);

}

ClientWindow::~ClientWindow()
{
    udpThread_.quit();
    udpThread_.wait();
    delete ui;
}
