#ifndef C_MAINWINDOW_H
#define C_MAINWINDOW_H
#include "d_logic.h"
#include "c_statusbar.h"
#include "c_setpage.h"
#include "c_centralpage.h"
#include "c_setpage.h"
#include "c_statuspage.h"
#include "c_taskpage.h"
#include "c_chooseareapage.h"
#include "c_choosemappage.h"
class MainWindow:public QWidget{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent=nullptr);
    ~MainWindow();
    void init();
public slots:
    void setPageSlot(CentralPageType);
    void returnPageSlot();
private:
    StatusBar* _statusBar;
    QStackedLayout* _centralLayout;
    CentralPage* _centralPage;
    TaskPage* _taskPage;
    SetPage* _setPage;
    ChooseMapPage* _chooseMapPage;
    ChooseAreaPage* _chooseAreaPage;
    StatusPage* _statusPage;
    QVBoxLayout* _layout;
private:
    QList<CentralPageType> _pageList;
};
#endif // C_MAINWINDOW_H
