#include "c_taskpage.h"

TaskPage::TaskPage(QWidget* parent):QWidget(parent){
    _titleFrame=new TitleFrame(this);
    _mapLabel=new CommonLabel(this);
    _mapLine=new CommonLabel(this);
    _areaLabel=new CommonLabel(this);
    _areaLine=new CommonLabel(this);
    _roundLabel=new CommonLabel(this);
    _roundLine=new CommonLabel(this);
    _mapFrame=new MapPathFrame(this);
    _initButton=new CommonButton(this);
    _startButton=new CommonButton(this);
    _layout0=new QFormLayout();
    _layout0->setContentsMargins(0,0,0,0);
    _layout0->setSpacing(0);
    _layout0->addRow(_mapLabel,_mapLine);
    _layout0->addRow(_areaLabel,_areaLine);
    _layout0->addRow(_roundLabel,_roundLine);
    _layout1=new QHBoxLayout();
    _layout1->setContentsMargins(0,0,0,0);
    _layout1->setSpacing(20);
    _layout1->addWidget(_initButton);
    _layout1->addSpacing(800);
    _layout1->addWidget(_startButton);
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(20,10,20,20);
    _layout->setSpacing(20);
    _layout->addWidget(_titleFrame);
    _layout->addLayout(_layout0);
    _layout->addWidget(_mapFrame);
    _layout->addLayout(_layout1);
    _layout->setStretch(0,1);
    _layout->setStretch(1,3);
    _layout->setStretch(2,9);
    _layout->setStretch(3,1);
    setLayout(_layout);
    _mapLine->setAlignment(Qt::AlignRight);
    _areaLine->setAlignment(Qt::AlignRight);
    _roundLine->setAlignment(Qt::AlignRight);
    _titleFrame->setTitle("任务");
    _mapLabel->setText("地图");
    _areaLabel->setText("区域");
    _roundLabel->setText("循环次数");
    _initButton->setText(" 初始化 ");
    _startButton->setText(" 开始任务 ");
    connect(_initButton,&QPushButton::clicked,[](){logic->init();});
    connect(_startButton,&QPushButton::clicked,[](){logic->startTask();});
}
TaskPage::~TaskPage(){
    _layout0->deleteLater();
    _layout1->deleteLater();
}
void TaskPage::init(){
    _mapLine->setText(record->taskInfo.map);
    _areaLine->setText(TOOL::GetStringFromStringList(record->taskInfo.areaList));
    _roundLine->setNum(record->taskInfo.roundNumber);
    _mapFrame->setMapInfo(record->mapInfo);
    _mapFrame->setAreas(record->taskInfo.areaList);
}
