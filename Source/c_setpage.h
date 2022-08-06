#ifndef C_SETPAGE_H
#define C_SETPAGE_H
#include "d_logic.h"
#include "b_widget.h"
class SetPage:public QWidget{
    Q_OBJECT
public:
    explicit SetPage(QWidget* parent=nullptr);
    void init();
public slots:
    void mapChangedSlot();
    void areaChangedSlot();
    void receiveRobotStateSlot();
private:
    QVBoxLayout* _layout;
    TitleFrame* _titleFrame;
    SlideWidget* _slideWidget;
    QWidget* _centralWidget;
    QVBoxLayout* _layout0;
    ItemsWidget* _taskWidget;
    EnterItem* _chooseMapItem;
    EnterItem* _chooseAreaItem;
    NumberItem* _roundItem;
    ItemsWidget* _setWidget;
    GradeItem* _waterUptakeItem;//吸水
    GradeItem* _pumpItem;//抽水
    GradeItem* _rollerBrushItem;//滚刷
    SwitchItem* _putterItem;//推杆
};

#endif // C_SETPAGE_H
