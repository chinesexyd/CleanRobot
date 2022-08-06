#include "c_mainwindow.h"

MainWindow::MainWindow(QWidget* parent):QWidget(parent){
    setWindowFlags(Qt::X11BypassWindowManagerHint|Qt::FramelessWindowHint);
    _statusBar=new StatusBar(this);
    _centralLayout=new QStackedLayout();
    _centralLayout->setContentsMargins(5,5,5,5);
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(0,0,0,0);
    _layout->setSpacing(0);
    _layout->addWidget(_statusBar);
    _layout->addLayout(_centralLayout);
    setLayout(_layout);
    _centralPage=new CentralPage(this);
    _setPage=new SetPage(this);
    _taskPage=new TaskPage(this);
    _statusPage=new StatusPage(this);
    _chooseMapPage=new ChooseMapPage(this);
    _chooseAreaPage=new ChooseAreaPage(this);
    _centralLayout->addWidget(_centralPage);
    _centralLayout->addWidget(_taskPage);
    _centralLayout->addWidget(_setPage);
    _centralLayout->addWidget(_statusPage);
    _centralLayout->addWidget(_chooseMapPage);
    _centralLayout->addWidget(_chooseAreaPage);
    setLayout(_layout);
    connect(hmi,SIGNAL(setPageSignal(CentralPageType)),this,SLOT(setPageSlot(CentralPageType)));
    connect(hmi,SIGNAL(returnPageSignal()),this,SLOT(returnPageSlot()));
    _pageList.append(CP_central);
}
MainWindow::~MainWindow(){
    delete _centralLayout;
    delete hmi;
    delete note;
    delete record;
    delete logic;
    delete ros;
}
void MainWindow::init(){
    record->init();
    _taskPage->init();
    _setPage->init();
//    _statusPage->init();
    _chooseMapPage->init();
    _chooseAreaPage->init();
    logic->startup();
}
void MainWindow::setPageSlot(CentralPageType page){
    _pageList.append(page);
    switch (page) {
    case CP_central:
        _centralLayout->setCurrentWidget(_centralPage);
        break;
    case CP_task:
        _centralLayout->setCurrentWidget(_taskPage);
        break;
    case CP_status:
        _centralLayout->setCurrentWidget(_statusPage);
        break;
    case CP_set:
        _centralLayout->setCurrentWidget(_setPage);
        break;
    case CP_chooseMap:
        _centralLayout->setCurrentWidget(_chooseMapPage);
        break;
    case CP_chooseArea:
        _centralLayout->setCurrentWidget(_chooseAreaPage);
        break;
    default:
        break;
    }
}
void MainWindow::returnPageSlot(){
    _pageList.removeLast();
    CentralPageType page=_pageList.last();
    _pageList.removeLast();
    setPageSlot(page);
}
