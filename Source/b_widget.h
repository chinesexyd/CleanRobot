#ifndef WIDGET_H
#define WIDGET_H
#include "a_root.h"
///////////////////////////////
///////////////////////////////
//           Widget          //
///////////////////////////////
///////////////////////////////
class SlideWidget:public QWidget{
    Q_OBJECT
public:
    explicit SlideWidget(QWidget* parent=nullptr);
    void setCentralWidget(QWidget* centralWidget);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    double _speed=0;
    QWidget* _centralWidget;
    int _startY=0;
    int _startCY=0;
    int _cY=0;
};
class ItemsWidget:public QWidget{
    Q_OBJECT
public:
    explicit ItemsWidget(QWidget* parent=nullptr);
    void addItem(QWidget* item);
    int indexOf(QWidget* item);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QVBoxLayout* _layout;
    QList<QWidget*> _items;
};
class SwitchItem:public QWidget{
    Q_OBJECT
public:
    explicit SwitchItem(QWidget* parent=nullptr);
    void setPixmap(QString path);
    void setTitle(QString title);
    void setBool(bool isTrue);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void switchSignal(bool);
private:
    QPixmap _pixmap;
    QString _title;
    bool _isTrue=false;
    bool _canMove=false;
};
class EnterItem:public QWidget{
    Q_OBJECT
public:
    explicit EnterItem(QWidget* parent=nullptr);
    void setPixmap(QString path);
    void setTitle(QString title);
    void setValue(QString value);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void enterSignal();
private:
    QPixmap _pixmap;
    QString _title;
    QString _value;
    QPixmap _enterPixmap;
    bool _isPress=false;
    bool _canMove=false;
    int _startY;
};
class NumberItem:public QWidget{
    Q_OBJECT
public:
    explicit NumberItem(QWidget* parent=nullptr);
    void setPixmap(QString path);
    void setTitle(QString title);
    void setTip(QString tip);
    void setValue(quint8 value);
protected:
    void mousePressEvent(QMouseEvent* event)override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void numberSignal(quint8);
private:
    QPixmap _pixmap;
    QString _title;
    quint8 _value;
    QString _tip;
    QPixmap _plusPixmap;
    QPixmap _minusPixmap;
    bool _canMove=false;
};
class GradeItem:public QWidget{
    Q_OBJECT
public:
    explicit GradeItem(QWidget* parent=nullptr);
    void setPixmap(QString path);
    void setTitle(QString title);
    void setValue(quint8 value);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
signals:
    void gradeSignal(GEAR);
private:
    QPixmap _pixmap;
    QString _title;
    quint8 _value=0;
    bool _canMove=false;
};
class MapButton:public QPushButton{
    Q_OBJECT
public:
    explicit MapButton(QWidget* parent=nullptr);
};
class MapAreaFrame:public QWidget{
    Q_OBJECT
public:
    explicit MapAreaFrame(QWidget* parent=nullptr);
    void setMapInfo(MapInfo info);
protected:
    void resizeEvent(QResizeEvent*)override;
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent*)override;
    void mouseReleaseEvent(QMouseEvent*)override;
    void mouseMoveEvent(QMouseEvent*)override;
private:
    QPointF localPosFromSourcePos(QPointF mapPos);
public slots:
    void zoomIn();
    void zoomOut();
signals:
    void sendSweepAreaNumAndLength(quint8,double);
private:
    MapButton* _zoomInButton;
    MapButton* _zoomOutButton;
    MapButton* _resetButton;
    QPixmap _mapPix;
    MapInfo _info;
    double _scale;//缩放比例
    QRect _mapPixRect;
    QRectF _mapDrawRect;
    bool _isMousePress=false;
    QPointF _mousePressPos;
    QRectF _mousePressTimeMapDrawRect;
    QList<QColor> _colorList;
};
class MapPathFrame:public QWidget{
    Q_OBJECT
public:
    explicit MapPathFrame(QWidget* parent=nullptr);
    void setMapInfo(MapInfo info);
    void setAreas(QStringList areas);
protected:
    void resizeEvent(QResizeEvent*)override;
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent*)override;
    void mouseReleaseEvent(QMouseEvent*)override;
    void mouseMoveEvent(QMouseEvent*)override;
private:
    QPointF localPosFromSourcePos(QPointF mapPos);
public slots:
    void zoomIn();
    void zoomOut();
signals:
    void sendSweepAreaNumAndLength(quint8,double);
private:
    MapButton* _zoomInButton;
    MapButton* _zoomOutButton;
    MapButton* _resetButton;
    QPixmap _mapPix;
    double _scale;//缩放比例
    QRect _mapPixRect;
    QRectF _mapDrawRect;
    bool _isMousePress=false;
    QPointF _mousePressPos;
    QRectF _mousePressTimeMapDrawRect;
    QList<QColor> _colorList;
    QStringList _areasList;
    MapInfo _info;
};
class PowerFrame:public QWidget{
    Q_OBJECT
public:
    explicit PowerFrame(QWidget* parent=nullptr);
    void setPower(quint8 power);
    void setCharge(bool isTrue);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    quint8 _power;
    QColor _powerColor;
    bool _isCharge=false;
};
class WaterFrame:public QWidget{
    Q_OBJECT
public:
    enum WF_TYPE{
        WF_Clean,//清水
        WF_Dirty,//脏水
        WF_Fluid//清洁液
    };
public:
    explicit WaterFrame(QWidget* parent=nullptr);
    void setWater(quint8 water);
    void setType(WF_TYPE type);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    WF_TYPE _type;
    quint8 _water;
};
class StatusLabel:public QLabel{
    Q_OBJECT
public:
    explicit StatusLabel(QWidget* parent=nullptr);
};
class PictureFrame:public QWidget{
    Q_OBJECT
public:
    explicit PictureFrame(QWidget* parent=nullptr);
    void setBool(bool isTrue);
    void setFalsePicture(QString path);
    void setTruePicture(QString path);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap _falsePix;
    QPixmap _truePix;
    bool _isTrue=false;
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@MenuButton@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
class MenuButton:public QWidget{
    Q_OBJECT
public:
    explicit MenuButton(QWidget* parent=nullptr);
    void setPixmap(QString path);
    void setName(QString text);
    void setColor(QColor color);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    void clickedSignal();
public slots:
    void animationValueChangedSlot(QVariant);
private:
    QPropertyAnimation *_aninormal;
private:
    QString _name;
    QPixmap _pixmap;
    QColor _color;
    double _border=0;
    bool _isPress=false;
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@TitleFrame@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
class TitleFrame:public QWidget{
    Q_OBJECT
public:
    explicit TitleFrame(QWidget* parent=nullptr);
    void setTitle(QString title);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap _pixmap;
    QString _title;
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@SlideFrame@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
class SlideFrame:public QWidget{
    Q_OBJECT
public:
    explicit SlideFrame(QWidget* parent=nullptr);
    void addItem(QWidget* item);
    void clearItem();
    QList<QWidget*> items();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void keepPose(double y);
private:
    QFrame* _centralFrame;
    QVBoxLayout* _vLayout;
    quint32 _height = 0;
    QList<QWidget*> _itemList;
    ////////
    bool _isPressing;
    QPointF _startPoint;
    double _centralY=0;
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ItemButton@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
class ItemButton:public QPushButton{
    Q_OBJECT
public:
    explicit ItemButton(QWidget* parent=nullptr);
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@CommonButton@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
class CommonButton:public QPushButton{
    Q_OBJECT
public:
    explicit CommonButton(QWidget* parent=nullptr);
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@CommonLabel@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
class CommonLabel:public QLabel{
    Q_OBJECT
public:
    explicit CommonLabel(QWidget* parent=nullptr);
};
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@SingleButtonDialog@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
class SingleButtonDialog:public QDialog{
    Q_OBJECT
public:
    explicit SingleButtonDialog(QWidget* parent=nullptr);
    void setTitle(QString title);
    void setContent(QString content);
protected:
    void paintEvent(QPaintEvent* event)override;
    void mousePressEvent(QMouseEvent* event)override;
    void mouseReleaseEvent(QMouseEvent* event)override;
    void resizeEvent(QResizeEvent *event) override;
private:
    QLabel* _titleLabel;
    QLabel* _contentLabel;
    QVBoxLayout* _layout;
private:
    QRect _buttonRect;
    QRectF _lineRect;
    QPainterPath _buttonPath;
    bool _isPress=false;
};

#endif // WIDGET_H
