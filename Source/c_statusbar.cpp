#include "c_statusbar.h"

StatusBar::StatusBar(QWidget* parent):QWidget(parent){
    setFixedHeight(30);
    _powerFrame=new PowerFrame(this);
    _cleanWaterFrame=new WaterFrame(this);
    _dirtyWaterFrame=new WaterFrame(this);
    _fluidWaterFrame=new WaterFrame(this);
    _initFrame=new PictureFrame(this);
    _powerLabel=new StatusLabel(this);
    _cleanWaterLabel=new StatusLabel(this);
    _dirtyWaterLabel=new StatusLabel(this);
    _fluidWaterLabel=new StatusLabel(this);
    _timeLabel=new StatusLabel(this);
    _statusLabel=new StatusLabel(this);
    _layout=new QHBoxLayout(this);
    _layout->setContentsMargins(3,3,3,3);
    _layout->setSpacing(3);
    _layout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding));
    _layout->addWidget(_statusLabel);
    _layout->addWidget(_initFrame);
    _layout->addWidget(_timeLabel);
    _layout->addWidget(_cleanWaterFrame);
    _layout->addWidget(_cleanWaterLabel);
    _layout->addWidget(_dirtyWaterFrame);
    _layout->addWidget(_dirtyWaterLabel);
    _layout->addWidget(_fluidWaterFrame);
    _layout->addWidget(_fluidWaterLabel);
    _layout->addWidget(_powerFrame);
    _layout->addWidget(_powerLabel);
    setLayout(_layout);
    _powerFrame->setFixedSize(55,24);
    _cleanWaterFrame->setFixedSize(30,24);
    _dirtyWaterFrame->setFixedSize(30,24);
    _fluidWaterFrame->setFixedSize(30,24);
    _powerLabel->setFixedWidth(50);
    _initFrame->setFixedSize(25,25);
//    _cleanWaterLabel->setFixedWidth(50);
//    _dirtyWaterLabel->setFixedWidth(50);
//    _fluidWaterLabel->setFixedWidth(50);
    _cleanWaterFrame->setType(WaterFrame::WF_Clean);
    _dirtyWaterFrame->setType(WaterFrame::WF_Dirty);
    _fluidWaterFrame->setType(WaterFrame::WF_Fluid);
    _initFrame->setTruePicture(":/Image/StatusInit.png");
    _initFrame->setFalsePicture(":/Image/StatusUninit.png");
    _timerID=startTimer(1000);
    ///////
    _powerFrame->setPower(100);
    _cleanWaterFrame->setWater(60);
    _dirtyWaterFrame->setWater(60);
    _fluidWaterFrame->setWater(60);
    _powerLabel->setText("100%");
    _cleanWaterLabel->setText("60%");
    _dirtyWaterLabel->setText("60%");
    _fluidWaterLabel->setText("60%");
    _initFrame->setBool(false);
    connect(hmi,SIGNAL(showStatusSignal(QString)),this,SLOT(showStatusSlot(QString)));
}
void StatusBar::timerEvent(QTimerEvent *event){
    Q_UNUSED(event);
    _timeLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
}
void StatusBar::showStatusSlot(QString status){
    _statusLabel->setText(status);
}
