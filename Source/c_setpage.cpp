#include "c_setpage.h"

SetPage::SetPage(QWidget* parent):QWidget(parent){
    _titleFrame=new TitleFrame(this);
    _slideWidget=new SlideWidget(this);
    _centralWidget=new QWidget(_slideWidget);
    _taskWidget=new ItemsWidget(_centralWidget);
    _chooseMapItem=new EnterItem(_taskWidget);
    _chooseAreaItem=new EnterItem(_taskWidget);
    _roundItem=new NumberItem(_taskWidget);
    _taskWidget->addItem(_chooseMapItem);
    _taskWidget->addItem(_chooseAreaItem);
    _taskWidget->addItem(_roundItem);
    _setWidget=new ItemsWidget(_centralWidget);
    _waterUptakeItem=new GradeItem(_setWidget);
    _pumpItem=new GradeItem(_setWidget);
    _rollerBrushItem=new GradeItem(_setWidget);
    _putterItem=new SwitchItem(_setWidget);
    _setWidget->addItem(_waterUptakeItem);
    _setWidget->addItem(_pumpItem);
    _setWidget->addItem(_rollerBrushItem);
    _setWidget->addItem(_putterItem);
    _layout0=new QVBoxLayout(_centralWidget);
    _layout0->setContentsMargins(0,0,0,0);
    _layout0->setSpacing(50);
    _layout0->addWidget(_taskWidget);
    _layout0->addWidget(_setWidget);
    _centralWidget->setLayout(_layout0);
    _slideWidget->setCentralWidget(_centralWidget);
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(20,10,20,20);
    _layout->setSpacing(20);
    _layout->addWidget(_titleFrame);
    _layout->addWidget(_slideWidget);
    _titleFrame->setTitle("设置");
    _chooseMapItem->setTitle("切换地图");
    _chooseAreaItem->setTitle("选择区域");
    _roundItem->setTitle("任务循环次数");
    _roundItem->setTip("注意：0代表无限循环");
    _waterUptakeItem->setTitle("吸水");
    _pumpItem->setTitle("抽水");
    _rollerBrushItem->setTitle("滚刷速度");
    _putterItem->setTitle("推杆");
    _chooseMapItem->setPixmap(":/Image/ItemMap.png");
    _chooseAreaItem->setPixmap(":/Image/ItemArea.png");
    _roundItem->setPixmap(":/Image/ItemRound.png");
    _waterUptakeItem->setPixmap(":/Image/ItemWaterup.png");
    _pumpItem->setPixmap(":/Image/ItemPump.png");
    _rollerBrushItem->setPixmap(":/Image/ItemRoller.png");

    connect(_chooseMapItem,&EnterItem::enterSignal,[](){hmi->setPageSignal(CP_chooseMap);});
    connect(_chooseAreaItem,&EnterItem::enterSignal,[](){hmi->setPageSignal(CP_chooseArea);});
    connect(hmi,SIGNAL(mapChangedSignal()),this,SLOT(mapChangedSlot()));
    connect(hmi,SIGNAL(areaChangedSignal()),this,SLOT(areaChangedSlot()));
    connect(_waterUptakeItem,&GradeItem::gradeSignal,[](GEAR grade){ros->waterIn(grade);});
    connect(_pumpItem,&GradeItem::gradeSignal,[](GEAR grade){ros->waterOut(grade);});
    connect(_rollerBrushItem,&GradeItem::gradeSignal,[](GEAR grade){ros->brush(grade);});
    connect(_putterItem,&SwitchItem::switchSignal,[](bool isTrue){BOOL b=isTrue?BL_OPEN:BL_CLOSE;ros->push(b);});
    connect(ros,SIGNAL(receiveRobotStateSignal()),this,SLOT(receiveRobotStateSlot()));
}
void SetPage::init(){
    _chooseMapItem->setValue(record->taskInfo.map);
    _chooseAreaItem->setValue(TOOL::GetStringFromStringList(record->taskInfo.areaList));
    _roundItem->setValue(record->taskInfo.roundNumber);
}
void SetPage::mapChangedSlot(){
    _chooseMapItem->setValue(record->taskInfo.map);
    _chooseAreaItem->setValue(TOOL::GetStringFromStringList(record->taskInfo.areaList));
}
void SetPage::areaChangedSlot(){
    _chooseAreaItem->setValue(TOOL::GetStringFromStringList(record->taskInfo.areaList));
}
void SetPage::receiveRobotStateSlot(){
    _waterUptakeItem->setValue(ros->robotState.cleanWaterOut);
    _pumpItem->setValue(ros->robotState.cleanWaterIn);
    _rollerBrushItem->setValue(ros->robotState.cleanBrush);
    _putterItem->setBool((ros->robotState.cleanPush==BL_OPEN));
}
