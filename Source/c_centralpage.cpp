#include "c_centralpage.h"

CentralPage::CentralPage(QWidget* parent):QWidget(parent){
    _taskButton=new MenuButton(this);
    _setButton=new MenuButton(this);
    _statusButton=new MenuButton(this);
    _taskButton->setName("任务");
    _setButton->setName("设置");
    _statusButton->setName("状态");
    _taskButton->setColor(QColor("#56aaf5"));
    _setButton->setColor(QColor("#56aaf5"));
    _statusButton->setColor(QColor("#56aaf5"));
    _taskButton->setPixmap(":/Image/MenuButtonTask.png");
    _setButton->setPixmap(":/Image/MenuButtonSet.png");
    _statusButton->setPixmap(":/Image/MenuButtonStatus.png");
    _layout0=new QHBoxLayout();
    _layout0->setContentsMargins(0,0,0,0);
    _layout0->setSpacing(30);
    _layout0->addWidget(_setButton);
    _layout0->addWidget(_taskButton);
    _layout0->addWidget(_statusButton);
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setSpacing(30);
    _layout->addSpacing(500);
    _layout->addLayout(_layout0);
    connect(_taskButton,&MenuButton::clickedSignal,[](){emit hmi->setPageSignal(CP_task);});
    connect(_setButton,&MenuButton::clickedSignal,[](){emit hmi->setPageSignal(CP_set);});
    connect(_statusButton,&MenuButton::clickedSignal,[](){emit hmi->setPageSignal(CP_status);});
}
CentralPage::~CentralPage(){
    _layout0->deleteLater();
}
