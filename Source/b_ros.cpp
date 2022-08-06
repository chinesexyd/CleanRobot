#include "b_ros.h"

/*@@@@@@@ROS交互器@@@@@@@*/
ROSTCPChater::ROSTCPChater(QObject *parent): QObject(parent){
    socket=new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(connectedSlot()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
}
void ROSTCPChater::sockedSend(QByteArray data){
    unsigned int len=data.length();
    unsigned int i=0;
    unsigned char CheckSum=0;
    unsigned int index=0;
    static unsigned short int frameseq=0;
    //
    v_sendbuff[index++]=0xA5;
    v_sendbuff[index++]=(uint8_t)((UGVLINK_FRAME_LEN + len) >> 8);
    v_sendbuff[index++]=(uint8_t)(UGVLINK_FRAME_LEN + len);
    //
    v_sendbuff[index++]=(uint8_t)(UGV_ADDR_TOPBOARD >> 8);
    v_sendbuff[index++]=(uint8_t)UGV_ADDR_TOPBOARD;
    //
    v_sendbuff[index++]=(uint8_t)(UGV_ADDR_MIDDLEBOARD  >> 8);
    v_sendbuff[index++]=(uint8_t)UGV_ADDR_MIDDLEBOARD;

    v_sendbuff[index++]=(uint8_t)(frameseq >> 8);
    v_sendbuff[index++]=(uint8_t)(frameseq);
    //
    frameseq++;
    v_sendbuff[index++]=UGV_FRAME_JSON_PACKET;

    for(i=0; i < len; i++){
        v_sendbuff[index++]=(unsigned char)data.data()[i];

    }
    //
    for(i=0; i < index; i++) {
        CheckSum += v_sendbuff[i];
    }
    v_sendbuff[index++]=CheckSum;

    QByteArray out;
    for(i=0;i<index;i++) {
        out.append(v_sendbuff[i]);
    }
    socket->write(out);
    socket->flush();
}
void ROSTCPChater::sockedReceive(unsigned char data){
    unsigned int len;
    unsigned char crc1,crc2;

//  printf("%2x ",data);
    switch(v_sta){
        case 0:{
                if(data==0xA5){
                    v_sta++;
                    v_index=0;
                    v_revbuff[v_index++]=data;
                }
            }
            break;
        case 1:
            v_revbuff[v_index++]=data;
            if(v_index>11){
                len=(v_revbuff[1]<< 8)+v_revbuff[2];
                if(len>=BUFF_MAX_LEN-UGVLINK_FRAME_LEN){
                    v_sta=0;
                    printf("bag len more than 200 err!!!\r\n");
                }
                else if(len==v_index){
                    unsigned int i;
                    crc1=0;
                    crc2=v_revbuff[len-1];
                    for(i=0;i<len-1;i++){
                        crc1+=v_revbuff[i];
                    }
                    if(crc1==crc2){
                        switch (v_revbuff[9]){
                            //得到数据包中的字符串
                            case UGV_FRAME_JSON_PACKET:{
                                std::string msg=std::string((char *)(v_revbuff+10),len-11);
                                QByteArray btarray(msg.c_str() , msg.size());
                                emit receiveMessageSignal(btarray);
                              }
                          break;
                            default:
                                printf("unknow cmd from android %d\r\n ",v_revbuff[9]);
                                //hex_publish(rdata,len-11,revbuff[9]);
                              break;
                          }
                    }else {
                        printf("crc check err \r\n");
                    }
                    v_sta=0;
                }
            }
            break;
        default:
            v_sta=0;
            v_index=0;
            break;
    }
}
void ROSTCPChater::tryConnect(){
    socket->connectToHost(v_host,v_port);
    //
    if(!socket->waitForConnected(2000)){
        v_isConnectSuccess=false;
        emit connectStateSignal(false);
    }
    else{
        v_isConnectSuccess=true;
        emit connectStateSignal(true);
    }
}
bool ROSTCPChater::isConnect(){
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(v_host,v_port);
    if(tcpClient.waitForConnected(1000)){
        if(!v_isConnectSuccess){
            socket->connectToHost(v_host,v_port);
        }
        tcpClient.disconnectFromHost();
        tcpClient.deleteLater();
        return true;
    }else{
        tcpClient.disconnectFromHost();
        tcpClient.deleteLater();
        return false;
    }
}
void ROSTCPChater::tryConnect(QString host, unsigned short int port){
    socket->connectToHost(host,port);
    //
    if(!socket->waitForConnected(2000)){
        v_isConnectSuccess=false;
        emit connectStateSignal(false);
    }
    else{
        v_isConnectSuccess=true;
        emit connectStateSignal(true);
        v_host=host;
        v_port=port;
    }
}
void ROSTCPChater::tryDisConnect(){
    socket->disconnectFromHost();
}
void ROSTCPChater::connectedSlot(){
    v_isConnectSuccess=true;
}
void ROSTCPChater::disconnectedSlot(){
    v_isConnectSuccess=false;
    emit disconnectStateSignal(true);
}
void ROSTCPChater::readyReadSlot(){
    QByteArray buffer=socket->readAll();
    for(int i=0;i<buffer.length();i++){
        sockedReceive((unsigned char)buffer[i]);
    }
}
/*@@@@@@@ROS收发信息打包解包@@@@@@@*/
ROS::ROS(QObject* parent):QObject(parent){
    _tcpChater=new ROSTCPChater(this);
    connect(_tcpChater,SIGNAL(receiveMessageSignal(QByteArray)),this,SLOT(receiveTcpMessageSlot(QByteArray)));
    connect(_tcpChater,SIGNAL(connectStateSignal(bool)),this,SLOT(connectStateSlot(bool)));
    connect(_tcpChater,SIGNAL(disconnectStateSignal(bool)),this,SLOT(disconnectStateSlot(bool)));
}
void ROS::receiveTcpMessageSlot(QByteArray byte){
    QJsonDocument rootDoc=QJsonDocument::fromJson(byte);
    QJsonObject rootObj=rootDoc.object();
    QString returnType=rootObj.value("type").toString();
    if(returnType=="robotState"){
        _robotState(rootObj);
    }else if(returnType=="ackLoadMap"){
        note->message(NOTE::MT_FROM_ROS,"加载地图",byte);
        _ackLoadMap(rootObj);
    }else if(returnType=="ackInitialPose"){
        note->message(NOTE::MT_FROM_ROS,"初始化点",byte);
        _ackInitialPose(rootObj);
    }else if(returnType=="ackMoveGoal"){
        note->message(NOTE::MT_FROM_ROS,"前往目标点",byte);
        _ackMoveGoal(rootObj);
    }else if(returnType=="upLoadError"){
        note->message(NOTE::MT_FROM_ROS,"上报错误",byte);
        _uploadError(rootObj);
    }else if(returnType=="ackMoveGoalFile"){
        note->message(NOTE::MT_FROM_ROS,"多点靠右导航",byte);
        _ackMoveGoalFile(rootObj);
    }else{}
}
void ROS::connectStateSlot(bool isTrue){
    if(isTrue){
        note->message(NOTE::MT_ROS,"连接ROS","连接ROS成功");
    }else{
        note->message(NOTE::MT_ROS,"连接ROS","连接ROS失败");
    }
}
void ROS::disconnectStateSlot(bool isTrue){
    if(isTrue){
        note->message(NOTE::MT_ROS,"断开ROS","断开ROS成功");
    }else{
        note->message(NOTE::MT_ROS,"断开ROS","断开ROS失败");
    }
}
void ROS::tryConnect(){
    _tcpChater->tryConnect();
}
void ROS::disConnect(){
    _tcpChater->tryDisConnect();
}
void ROS::loadMap(QString mapPath){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","loadMap");
    obj.insert("mapFile",mapPath);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"初始化地图",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::initPoint(XyaPoint point){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","initialPose");
    obj.insert("x",point.x);
    obj.insert("y",point.y);
    obj.insert("yaw",point.a);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"初始化点",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::gotoPoint(XyaPoint point,QString tag){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","moveGoal");
    obj.insert("x",point.x);
    obj.insert("y",point.y);
    obj.insert("yaw",point.a);
    obj.insert("tag",tag);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"前往目标点",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::cancelGotoPoint(){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","moveCancel");
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"取消前往目标点",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::turnOnCharge(){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","RelaySwitch");
    obj.insert("relayswitch",2);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"开始充电",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::turnOffCharge(){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","RelaySwitch");
    obj.insert("relayswitch",1);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"停止充电",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::shutdown(){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","RelaySwitch");
    obj.insert("relayswitch",0);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"关机断电",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::sweep(QString filename,QString tag){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","moveGoalFile");
    obj.insert("fileName",filename);
    obj.insert("building",tag);
    obj.insert("robotType","SweepRobot");
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"发送多点靠右导航",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::waterOut(GEAR gear){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","setClean");
    obj.insert("water_out",gear);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"抽水",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::waterIn(GEAR gear){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","setClean");
    obj.insert("water_in",gear);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"吸水",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::brush(GEAR gear){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","setClean");
    obj.insert("brush",gear);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"刷子",byte);
    _tcpChater->sockedSend(byte);
}
void ROS::push(BOOL isOpen){
    QJsonObject obj;
    QJsonDocument document;
    obj.insert("type","setClean");
    obj.insert("push",isOpen);
    document.setObject(obj);
    QByteArray byte=document.toJson(QJsonDocument::Compact);
    note->message(NOTE::MT_TO_ROS,"推杆",byte);
    _tcpChater->sockedSend(byte);
}
GEAR ROS::intToGear(int value){
    if(value==0){
        return GR_CLOSE;
    }else if(value==1){
        return GR_ONE;
    }else if(value==2){
        return GR_TWO;
    }else if(value==3){
        return GR_THREE;
    }else{
        return GR_CLOSE;
    }
}
BOOL ROS::intToBool(int value){
    if(value==0){
        return BL_CLOSE;
    }else if(value==1){
        return BL_OPEN;
    }else{
        return BL_CLOSE;
    }
}
RESULT_STATUS ROS::resultStatus(quint8 result,quint8 status){
    if(result==0 && status==0){
        return RS_SUCCESS;
    }else if(result!=0&&status==0){
        return RS_FAIL;
    }else{
        return RS_RUNNING;
    }
}
void ROS::_robotState(QJsonObject obj){
    robotState.charge=bool(obj.value("charge").toInt());
    robotState.collision=obj.value("collision").toBool();
    robotState.cur= obj.value("cur").toInt()/100.0;
    robotState.doorState=obj.value("doorState").toBool();
    robotState.emergency=obj.value("emergency").toBool();
    robotState.imu=obj.value("imu").toBool();
    robotState.mah=obj.value("mah").toInt();
    robotState.mapFile=obj.value("mapFile").toString();
    robotState.mcuHwVersion=obj.value("mcu_hw_version").toString();
    robotState.mcuSoftVersion=obj.value("mcu_soft_version").toString();
    robotState.mils =obj.value("mils").toInt();
    robotState.relay=obj.value("relay").toInt();
    robotState.rosHwVersion=obj.value("ros_soft_version").toString();
    robotState.rosSoftVersion=obj.value("ros_soft_version").toString();
    robotState.skip=obj.value("skip").toInt();
    robotState.sonar=obj.value("sonar").toBool();
    robotState.vol=obj.value("vol").toInt()/100.0;
    robotState.wheel=obj.value("wheel").toBool();
    double x=obj.value("x").toDouble();
    double y=obj.value("y").toDouble();
    double a=obj.value("yaw").toDouble();
    double z=obj.value("z").toDouble();
    double w=obj.value("w").toDouble();
    robotState.point.x=x;
    robotState.point.y=y;
    robotState.point.a=a;
    robotState.xyzwPoint.x=x;
    robotState.xyzwPoint.y=y;
    robotState.xyzwPoint.z=z;
    robotState.xyzwPoint.w=w;
    robotState.cleanWaterOut=intToGear(obj.value("clean_water_out").toInt());
    robotState.cleanWaterIn=intToGear(obj.value("clean_water_in").toInt());
    robotState.cleanBrush=intToGear(obj.value("clean_brush").toInt());
    robotState.cleanPush=intToBool(obj.value("clean_push").toInt());
    emit receiveRobotStateSignal();
}
void ROS::_ackLoadMap(QJsonObject obj){
    quint8 result=obj.value("result").toInt();
    quint8 status=obj.value("status").toInt();
    RESULT_STATUS rs=resultStatus(result,status);
    emit loadMapSignal(rs);
}
void ROS::_ackInitialPose(QJsonObject obj){
    quint8 result = obj.value("result").toInt();
    quint8 status = obj.value("status").toInt();
    RESULT_STATUS rs=resultStatus(result,status);
    emit initialPoseSignal(rs);
}
void ROS::_ackMoveGoal(QJsonObject obj){
    quint8 result = obj.value("result").toInt();
    quint8 status = obj.value("status").toInt();
    RESULT_STATUS rs=resultStatus(result,status);
    emit moveGoalSignal(rs);
}
void ROS::_uploadError(QJsonObject obj){
    quint32 errcode = obj.value("errCode").toVariant().toUInt();
    QString msg = obj.value("msg").toString();
    emit uploadErrorSignal(errcode,msg);
}
void ROS::_ackMoveGoalFile(QJsonObject obj){
    quint8 result = obj.value("result").toInt();
    quint8 status = obj.value("status").toInt();
    RESULT_STATUS rs=resultStatus(result,status);
    emit moveGoalFileSignal(rs);
}

