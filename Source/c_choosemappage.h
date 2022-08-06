#ifndef C_MAPPAGE_H
#define C_MAPPAGE_H
#include "b_widget.h"
#include "d_logic.h"
class ChooseMapPage:public QWidget{
    Q_OBJECT
public:
    explicit ChooseMapPage(QWidget* parent=nullptr);
    ~ChooseMapPage();
    void init();
private:
    void loadVillages(QString path);
    void loadMaps(QString path,QString parentpath);
public slots:
    void villageItemClickedSlot();
    void mapItemClickedSlot(bool);
    void confirmButtonClickedSlot();
private:
    QVBoxLayout* _layout;
    TitleFrame* _titleFrame;
    QGridLayout* _layout0;
    SlideFrame* _villagesWidget;
    SlideFrame* _mapsWidget;
    MapAreaFrame* _mapFrame;
    QHBoxLayout* _layout1;
    QFormLayout* _layout2;
    CommonLabel* _titleLabel;
    CommonLabel* _pathLabel;
    CommonButton* _confirmButton;
    SingleButtonDialog* _confirmDialog;
    QButtonGroup* _villagesGroup;
    QButtonGroup* _mapsGroup;
private:
    int _mapsCount=0;
    MapInfo _info;
};

#endif // C_MAPPAGE_H
