#ifndef C_STATUSBAR_H
#define C_STATUSBAR_H
#include "d_logic.h"
#include "b_widget.h"
class StatusBar:public QWidget{
    Q_OBJECT
public:
    explicit StatusBar(QWidget* parent=nullptr);
protected:
    void timerEvent(QTimerEvent* event)override;
public slots:
    void showStatusSlot(QString);
private:
    PowerFrame* _powerFrame;
    WaterFrame* _cleanWaterFrame;
    WaterFrame* _dirtyWaterFrame;
    WaterFrame* _fluidWaterFrame;
    PictureFrame* _initFrame;
    StatusLabel* _powerLabel;
    StatusLabel* _cleanWaterLabel;
    StatusLabel* _dirtyWaterLabel;
    StatusLabel* _fluidWaterLabel;
    StatusLabel* _timeLabel;
    StatusLabel* _statusLabel;
    QHBoxLayout* _layout;
private:
    int _timerID;
};

#endif // C_STATUSBAR_H
