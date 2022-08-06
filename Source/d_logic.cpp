#include "d_logic.h"

ROS* ros=new ROS();
Logic::Logic(QObject* parent):QObject(parent){
    connect(ros,SIGNAL(loadMapSignal(RESULT_STATUS)),this,SLOT(loadMapSlot(RESULT_STATUS)));
    connect(ros,SIGNAL(initialPoseSignal(RESULT_STATUS)),this,SLOT(initialPoseSlot(RESULT_STATUS)));
}
//开机启动
void Logic::startup(){
    ros->tryConnect();
}
//开始任务
void Logic::startTask(){
    //将路径中的点都拼接在一起，保存至文件，保存成功后调用ROS多点靠右行驶。
    if(logicParams.isInit){
        QString path=record->taskInfo.map+"/Path.json";
        QFile file(path);
        if(file.exists()){
            file.open(QIODevice::ReadOnly);
            QByteArray byte=file.readAll();
            file.close();
            QString sweepPath=record->taskInfo.map+"/sweep";
            QFile sweepFile(sweepPath);
            if(sweepFile.exists()){
                sweepFile.remove();
            }
            sweepFile.open(QIODevice::WriteOnly);
            QJsonDocument doc=QJsonDocument::fromJson(byte);
            QJsonObject obj=doc.object();
            QJsonArray array;
            for(int i=0;i<record->taskInfo.areaList.length();i++){
                QString area=record->taskInfo.areaList[i];
                QJsonObject areaObj=obj.value(area).toObject();
                QJsonArray pathArray=areaObj.value("Path").toArray();
                for(int k=0;k<pathArray.size();k++){
                    array.append(pathArray[k]);
                }
            }
            sweepFile.write("#000000\n");
            for(int j=0;j<array.size()-1;j++){
                QJsonArray sarray=array[j].toArray();
                QPointF spos=QPointF(sarray[0].toDouble(),sarray[1].toDouble());
                QJsonArray earray=array[j+1].toArray();
                QPointF epos=QPointF(earray[0].toDouble(),earray[1].toDouble());
                double angle=TOOL::GetAngleFromTwoPoint(spos,epos);
                XyaPoint xyaPoint;
                xyaPoint.x=spos.x();
                xyaPoint.y=spos.y();
                xyaPoint.a=angle;
                XyzwPoint xyzwPoint=TOOL::GetXYZWPOINTFromXYAPOINT(xyaPoint);
                QString xyzwStr=QString("%1,%2,%3,%4\n")
                        .arg(QString::number(xyzwPoint.x,'f',3))
                        .arg(QString::number(xyzwPoint.y,'f',3))
                        .arg(QString::number(xyzwPoint.z,'f',3))
                        .arg(QString::number(xyzwPoint.w,'f',3));
                sweepFile.write(xyzwStr.toUtf8());
            }
            sweepFile.close();
            ros->sweep("sweep","#000000");
        }
    }else{
        emit hmi->showStatusSignal("请先初始化");
    }
}
//暂停任务
void Logic::pauseTask(){

}
//继续任务
void Logic::continueTask(){

}
//关闭软件
void Logic::close(){
    ros->disConnect();
    record->save();
    emit hmi->exitSignal();
}
//初始化
void Logic::init(){
    if(logicParams.isInit){
        emit hmi->showStatusSignal("不可重复初始化");
    }else{
        MapInfo info=TOOL::GetMapInfoFromDir(record->taskInfo.map);
        QString map=info.relativeMapPath;
        ros->loadMap(map);
        emit hmi->showStatusSignal("加载地图中");
    }
}
//加载地图反馈
void Logic::loadMapSlot(RESULT_STATUS status){
    if(status==RS_SUCCESS){
        ros->initPoint(record->mapInfo.initPoint);
        emit hmi->showStatusSignal("加载地图完成，初始化点");
    }else if(status==RS_RUNNING){
        emit hmi->showStatusSignal("加载地图中...");
    }else{
        emit hmi->showStatusSignal("加载地图失败");
    }
}
//初始化点反馈
void Logic::initialPoseSlot(RESULT_STATUS status){
    if(status==RS_SUCCESS){
        logicParams.isInit=true;
        emit hmi->showStatusSignal("初始化点成功，初始化完成");
    }else if(status==RS_RUNNING){
        emit hmi->showStatusSignal("初始化点中...");
    }else{
        emit hmi->showStatusSignal("初始化点失败");
    }
}
Logic* logic=new Logic();

