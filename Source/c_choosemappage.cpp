#include "c_choosemappage.h"

ChooseMapPage::ChooseMapPage(QWidget* parent):QWidget(parent){
    _titleFrame=new TitleFrame(this);
    _villagesWidget=new SlideFrame(this);
    _mapsWidget=new SlideFrame(this);
    _mapFrame=new MapAreaFrame(this);
    _titleLabel=new CommonLabel(this);
    _pathLabel=new CommonLabel(this);
    _confirmButton=new CommonButton(this);
    _titleFrame->setTitle("切换地图");
    _titleLabel->setText("地图路径");
    _confirmButton->setText(" 选择此地图 ");
    _layout2=new QFormLayout();
    _layout2->setContentsMargins(0,0,0,0);
    _layout2->setSpacing(10);
    _layout2->addRow(_titleLabel,_pathLabel);
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
    _layout0->addWidget(_villagesWidget,0,0,2,1);
    _layout0->addWidget(_mapsWidget,0,1,2,1);
    _layout0->addLayout(_layout1,1,2,1,1);
    _layout0->addWidget(_mapFrame,0,2,1,1);
    _layout0->setColumnStretch(0,1);
    _layout0->setColumnStretch(1,1);
    _layout0->setColumnStretch(2,3);
    _layout0->setRowStretch(0,4);
    _layout0->setRowStretch(1,1);
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(20,10,20,20);
    _layout->setSpacing(20);
    _layout->addWidget(_titleFrame);
    _layout->addLayout(_layout0);
    setLayout(_layout);
    _villagesGroup=new QButtonGroup(this);
    _mapsGroup=new QButtonGroup(this);
    _confirmDialog= new SingleButtonDialog(this);
    _confirmDialog->setTitle("切换地图");
    _confirmDialog->setContent("切换地图成功！");

    connect(_confirmButton,SIGNAL(clicked()),this,SLOT(confirmButtonClickedSlot()));
}
ChooseMapPage::~ChooseMapPage(){
    _layout0->deleteLater();
    _layout1->deleteLater();
    _layout2->deleteLater();
}
void ChooseMapPage::init(){
    _confirmDialog->hide();
    loadVillages(MAP_PATH);
}
void ChooseMapPage::loadVillages(QString path){
    QDir dir(path);
    QStringList areaList;
    if(dir.exists()){
        QFileInfoList infoList=dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Dirs);
        for(int i=0;i<infoList.length();i++){
            QFileInfo info=infoList[i];
            ItemButton* item=new ItemButton(_villagesWidget);
            item->setText(info.fileName());
            item->setCheckable(true);
            item->setProperty("filePath",info.filePath());
            _villagesGroup->addButton(item);
            connect(item,SIGNAL(clicked()),this,SLOT(villageItemClickedSlot()));
            _villagesWidget->addItem(item);
            if(i==0){
                item->click();
            }
        }
    }
}
void ChooseMapPage::villageItemClickedSlot(){
    ItemButton* item=static_cast<ItemButton*>(sender());
    QString filePath=item->property("filePath").toString();
    QList<QAbstractButton*> buttons=_mapsGroup->buttons();
    for(int i=0;i<buttons.length();i++){
        _mapsGroup->removeButton(buttons[i]);
    }
    _mapsWidget->clearItem();
    loadMaps(filePath,filePath);
}
void ChooseMapPage::loadMaps(QString path,QString parentpath){
    QDir dir(path);
    if(dir.exists()){
        MapInfo info=TOOL::GetMapInfoFromDir(path);
        if(info.canUse){
            ItemButton* item=new ItemButton(_mapsWidget);
            item->setCheckable(true);
            item->setProperty("filePath",info.mapDir);
            connect(item,SIGNAL(clicked(bool)),this,SLOT(mapItemClickedSlot(bool)));
            QString text=info.absoluteMapPath;
            item->setText(text.remove(parentpath+"/"));
            _mapsWidget->addItem(item);
            _mapsCount++;
            if(_mapsCount==1){
                item->click();
            }
        }else{
             QFileInfoList infoList0=dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Dirs);
             foreach (QFileInfo info, infoList0) {
                 loadMaps(info.filePath(),parentpath);
             }
        }
    }
}
void ChooseMapPage::mapItemClickedSlot(bool isChecked){
    Q_UNUSED(isChecked);
    ItemButton* item=static_cast<ItemButton*>(sender());
    QString filePath=item->property("filePath").toString();
    MapInfo info=TOOL::GetMapInfoFromDir(filePath);
    _mapFrame->setMapInfo(info);
    _pathLabel->setText(info.absoluteMapPath);
    _info=info;
}
void ChooseMapPage::confirmButtonClickedSlot(){
    if(record->taskInfo.map!=_info.absoluteMapPath){
        emit hmi->mapChangedSignal();
        record->taskInfo.map=_info.absoluteMapPath;
        record->taskInfo.areaList.clear();
        record->mapInfo=_info;
    }
    _confirmDialog->show();
}
