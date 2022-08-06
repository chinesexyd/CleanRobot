#include "c_chooseareapage.h"

ChooseAreaPage::ChooseAreaPage(QWidget* parent):QWidget(parent){
    _titleFrame=new TitleFrame(this);
    _areasWidget=new SlideFrame(this);
    _mapFrame=new MapPathFrame(this);
    _titleLabel=new CommonLabel(this);
    _areasLabel=new CommonLabel(this);
    _confirmButton=new CommonButton(this);
    _titleFrame->setTitle("选择区域");
    _titleLabel->setText("已选区域");
    _confirmButton->setText(" 确认这些区域 ");
    _layout2=new QFormLayout();
    _layout2->setContentsMargins(0,0,0,0);
    _layout2->setSpacing(10);
    _layout2->addRow(_titleLabel,_areasLabel);
    _layout1=new QHBoxLayout();
    _layout1->setContentsMargins(0,10,0,0);
    _layout1->setSpacing(10);
    _layout1->addLayout(_layout2);
    _layout1->addWidget(_confirmButton);
    _layout1->setStretch(0,4);
    _layout1->setStretch(1,1);
    _layout0=new QGridLayout();
    _layout0->setContentsMargins(0,0,0,0);
    _layout0->setSpacing(5);
    _layout0->addWidget(_areasWidget,0,0,2,1);
    _layout0->addLayout(_layout1,1,1,1,1);
    _layout0->addWidget(_mapFrame,0,1,1,1);
    _layout0->setColumnStretch(0,1);
    _layout0->setColumnStretch(1,4);
    _layout0->setRowStretch(0,4);
    _layout0->setRowStretch(1,1);
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(20,10,20,20);
    _layout->setSpacing(20);
    _layout->addWidget(_titleFrame);
    _layout->addLayout(_layout0);
    setLayout(_layout);
    _confirmDialog= new SingleButtonDialog(this);
    _confirmDialog->setTitle("选择区域");
    _confirmDialog->setContent("选择区域成功！");

    connect(_confirmButton,SIGNAL(clicked()),this,SLOT(confirmButtonClickedSlot()));
    connect(hmi,SIGNAL(mapChangedSignal()),this,SLOT(mapChangedSlot()));
}
ChooseAreaPage::~ChooseAreaPage(){
    _layout0->deleteLater();
    _layout1->deleteLater();
    _layout2->deleteLater();
}
void ChooseAreaPage::init(){
    _mapFrame->setMapInfo(record->mapInfo);
    _mapFrame->setAreas(record->taskInfo.areaList);
    mapChangedSlot();
    _confirmDialog->hide();
}
void ChooseAreaPage::confirmButtonClickedSlot(){
    record->taskInfo.areaList=_areas;
    emit hmi->areaChangedSignal();
    _confirmDialog->show();
}
void ChooseAreaPage::mapChangedSlot(){
    _areasWidget->clearItem();
    _mapFrame->setMapInfo(record->mapInfo);
    _mapFrame->setAreas(record->taskInfo.areaList);
    QStringList areas=record->mapInfo.areaPathMap.keys();
    for(int i=0;i<areas.length();i++){
        ItemButton* item=new ItemButton(_areasWidget);
        item->setText(areas[i]);
        item->setCheckable(true);
        item->setProperty("area",areas[i]);
        connect(item,SIGNAL(clicked(bool)),this,SLOT(areaItemClickedSlot(bool)));
        _areasWidget->addItem(item);
        if(record->taskInfo.areaList.contains(areas[i])){
            item->setChecked(true);
        }
    }
    _areas=record->taskInfo.areaList;
    _areasLabel->setText(TOOL::GetStringFromStringList(_areas));
}
void ChooseAreaPage::areaItemClickedSlot(bool isChecked){
    ItemButton* item=static_cast<ItemButton*>(sender());
    QString area=item->property("area").toString();
    if(isChecked){
        if(!_areas.contains(area)){
            _areas.append(area);
        }
    }else{
        if(_areas.contains(area)){
            _areas.removeOne(area);
        }
    }
    _mapFrame->setAreas(_areas);
    _areasLabel->setText(TOOL::GetStringFromStringList(_areas));
}
