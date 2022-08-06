#ifndef C_STATUSPAGE_H
#define C_STATUSPAGE_H
#include "b_widget.h"
#include "d_logic.h"
class StatusPage:public QWidget{
    Q_OBJECT
public:
    explicit StatusPage(QWidget* parent=nullptr);
private:
    QVBoxLayout* _layout;
    TitleFrame* _titleFrame;
};

#endif // C_STATUSPAGE_H
