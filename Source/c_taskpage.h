#ifndef C_TASKPAGE_H
#define C_TASKPAGE_H
#include "b_widget.h"
#include "d_logic.h"
class TaskPage:public QWidget{
    Q_OBJECT
public:
    explicit TaskPage(QWidget* parent=nullptr);
    ~TaskPage();
    void init();
private:
    QVBoxLayout* _layout;
    TitleFrame* _titleFrame;
    QFormLayout* _layout0;
    QHBoxLayout* _layout1;
    CommonLabel* _mapLabel;
    CommonLabel* _mapLine;
    CommonLabel* _areaLabel;
    CommonLabel* _areaLine;
    CommonLabel* _roundLabel;
    CommonLabel* _roundLine;
    MapPathFrame* _mapFrame;
    CommonButton* _initButton;
    CommonButton* _startButton;
};

#endif // C_TASKPAGE_H
