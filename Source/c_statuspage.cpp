#include "c_statuspage.h"

StatusPage::StatusPage(QWidget* parent):QWidget(parent){
    _titleFrame=new TitleFrame(this);
    _titleFrame->setTitle("状态");
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(20,10,20,20);
    _layout->setSpacing(20);
    _layout->addWidget(_titleFrame);
    _layout->addSpacing(700);
}
