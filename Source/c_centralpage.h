#ifndef C_CENTRALPAGE_H
#define C_CENTRALPAGE_H
#include "b_widget.h"
#include "d_logic.h"
class UnStartTaskPart:public QWidget{
    Q_OBJECT
public:
    explicit UnStartTaskPart(QWidget* parent=nullptr);
};
class RunningTaskPart:public QWidget{
    Q_OBJECT
public:
    explicit RunningTaskPart(QWidget* parent=nullptr);
};
class CentralPage:public QWidget{
    Q_OBJECT
public:
    explicit CentralPage(QWidget* parent=nullptr);
    ~CentralPage();
private:
    MenuButton* _taskButton;
    MenuButton* _statusButton;
    MenuButton* _setButton;
    QVBoxLayout* _layout;
    QHBoxLayout* _layout0;
};

#endif // C_CENTRALPAGE_H
