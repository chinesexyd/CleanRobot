#ifndef D_LOGIC_H
#define D_LOGIC_H
#include "b_ros.h"

extern ROS* ros;
class Logic:public QObject{
    Q_OBJECT
public:
    explicit Logic(QObject* parent=nullptr);
    void startup();//开机启动
    void startTask();//开始任务
    void pauseTask();//暂停任务
    void continueTask();//继续任务
    void close();//关闭软件
    void init();//初始化
private:
    void connectSignals();
public slots:
    void loadMapSlot(RESULT_STATUS);
    void initialPoseSlot(RESULT_STATUS);
public:
    struct LogicParams{
        bool isInit=false;
    }logicParams;
};
extern Logic* logic;

#endif // D_LOGIC_H
