#ifndef C_AREAPAGE_H
#define C_AREAPAGE_H
#include "b_widget.h"
#include "d_logic.h"
class ChooseAreaPage:public QWidget{
    Q_OBJECT
public:
    explicit ChooseAreaPage(QWidget* parent=nullptr);
    ~ChooseAreaPage();
    void init();
public slots:
    void confirmButtonClickedSlot();
    void mapChangedSlot();
    void areaItemClickedSlot(bool);
private:
    QVBoxLayout* _layout;
    TitleFrame* _titleFrame;
    QGridLayout* _layout0;
    SlideFrame* _areasWidget;
    MapPathFrame* _mapFrame;
    QHBoxLayout* _layout1;
    CommonLabel* _titleLabel;
    CommonLabel* _areasLabel;
    QFormLayout* _layout2;
    CommonButton* _confirmButton;
    SingleButtonDialog* _confirmDialog;
private:
    QStringList _areas;
};

#endif // C_AREAPAGE_H
