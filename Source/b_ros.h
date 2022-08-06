#ifndef B_ROS_H
#define B_ROS_H

#include "a_root.h"
#define UGVLINK_FRAME_LEN 11
#define BUFF_MAX_LEN 60000
typedef enum{
    UGV_ADDR_BOTTOMBOARD = 0x0001,
    UGV_ADDR_MIDDLEBOARD = 0x0002,
    UGV_ADDR_TOPBOARD = 0x0003
}UGV_DeviceAddr;
typedef enum{
    UGV_FRAME_JSON_PACKET = 0x30,
    UGV_FRAME_UNKNOWN = 0xFF
}UGV_FrameType;
/*@@@@@@@ROS交互器@@@@@@@*/
class ROSTCPChater:public QObject{
    Q_OBJECT
public:
    ROSTCPChater(QObject *parent=nullptr);
    void tryConnect();
    void tryConnect(QString m_host,unsigned short int m_port);
    void tryDisConnect();
    void sockedSend(QByteArray data);
    bool isConnect();
private:
    void sockedReceive(unsigned char data);
public slots:
    void connectedSlot();
    void disconnectedSlot();
    void readyReadSlot();
signals:
    void receiveMessageSignal(QByteArray);
    void connectStateSignal(bool);
    void disconnectStateSignal(bool);
private:
    QTcpSocket *socket;
    QString v_host = "192.168.1.101";
    unsigned short int v_port=9999;
    bool v_isConnectSuccess=false;
    unsigned int v_index;
    unsigned char v_sta,v_revbuff[BUFF_MAX_LEN],v_sendbuff[BUFF_MAX_LEN];
};
/*@@@@@@@ROS@@@@@@@*/
class ROS:public QObject{
    Q_OBJECT
public:
    explicit ROS(QObject* parent=nullptr);
    void tryConnect();
    void disConnect();
    void loadMap(QString mapPath);//初始化地图（相对路径）
    void initPoint(XyaPoint point);
    void gotoPoint(XyaPoint point,QString tag);
    void cancelGotoPoint();//取消前往目标点
    void turnOnCharge();//充电
    void turnOffCharge();//断电
    void shutdown();//关机
    void sweep(QString filename,QString tag);
    void waterOut(GEAR gear);
    void waterIn(GEAR gear);
    void brush(GEAR gear);
    void push(BOOL isOpen);
private:
    GEAR intToGear(int value);
    BOOL intToBool(int value);
    RESULT_STATUS resultStatus(quint8 result,quint8 status);
    void _robotState(QJsonObject);
    void _ackLoadMap(QJsonObject);
    void _ackInitialPose(QJsonObject);
    void _ackMoveGoal(QJsonObject);
    void _uploadError(QJsonObject);
    void _ackMoveGoalFile(QJsonObject);
public slots:
    void receiveTcpMessageSlot(QByteArray);
    void connectStateSlot(bool isTrue);
    void disconnectStateSlot(bool isTrue);
signals:
    void connectSignal(bool);
    void disconnectSignal(bool);
    void receiveRobotStateSignal();
    void loadMapSignal(RESULT_STATUS);
    void initialPoseSignal(RESULT_STATUS);
    void moveGoalSignal(RESULT_STATUS);
    void uploadErrorSignal(quint32,QString);
    void moveGoalFileSignal(RESULT_STATUS);
public:
    ROBOT_STATE robotState;
private:
    ROSTCPChater* _tcpChater;
};

#endif // B_ROS_H
