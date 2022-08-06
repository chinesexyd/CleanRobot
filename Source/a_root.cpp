#include "a_root.h"
//////////////////////////
double TOOL::GetAngleFromTwoPoint(QPointF spos,QPointF epos){
    if(spos.x()==epos.x()){
        if(spos.y()==epos.y()){
            return 0;
        }
        if(spos.y()>epos.y()){
            return -90;
        }else{
            return 90;
        }
    }
    double ret=atan((epos.y()-spos.y())/(epos.x()-spos.x()));
    ret=ret*180/M_PI;
    if(epos.x()<spos.x()){
        if(epos.y()>spos.y()){
            ret+=180;
        }else{
            ret-=180;
        }
    }
    return ret;
}
XyzwPoint TOOL::GetXYZWPOINTFromXYAPOINT(XyaPoint point){
    double angle = point.a;
    double yaw = angle/180.0*M_PI;
    double cy = cos(yaw*0.5);
    double sy = sin(yaw*0.5);
    double cp = 1;
    double sp = 0;
    double cr = 1;
    double sr = 0;
    XyzwPoint xyzwPoint;
    xyzwPoint.x = point.x;
    xyzwPoint.y = point.y;
    xyzwPoint.w = cy*cp*cr+sy*sp*sr;
    xyzwPoint.z = sy*cp*cr-cy*sp*sr;
    return xyzwPoint;
}
MapInfo TOOL::GetMapInfoFromDir(QString dirPath){
    QDir dir(dirPath);
    MapInfo mapInfo;
    if(dir.exists()){
        mapInfo.mapDir=dirPath;
        QFileInfoList infoList=dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Files);
        for(int i=0;i<infoList.length();i++){
            QFileInfo info=infoList[i];
            if(info.suffix()=="pgm"){
                mapInfo.absoluteMapPath=info.filePath();
                mapInfo.relativeMapPath=info.filePath().remove(MAP_PATH);
                mapInfo.isHaveMap=true;
            }
            if(info.suffix()=="yaml"){
                YAML::Node node = YAML::LoadFile(info.filePath().toStdString());
                mapInfo.resolution = node["resolution"].as<double>();
                mapInfo.originX = node["origin"][0].as<double>();
                mapInfo.originY = node["origin"][1].as<double>();
                mapInfo.isHaveYaml=true;
            }
            if(info.fileName()=="Path.json"){
                QFile pathfile(info.filePath());
                pathfile.open(QIODevice::ReadOnly);
                QJsonObject obj=QJsonDocument::fromJson(pathfile.readAll()).object();
                pathfile.close();
                QStringList areaNameList=obj.keys();
                for(int j=0;j<areaNameList.length();j++){
                    QString areaName=areaNameList[j];
                    QJsonObject areaObj=obj.value(areaName).toObject();
                    QJsonArray polygonArray=areaObj.value("Polygon").toArray();
                    mapInfo.areaPolygonMap.insert(areaName,polygonArray);
                    QJsonArray pathArray=areaObj.value("Path").toArray();
                    mapInfo.areaPathMap.insert(areaName,pathArray);
                }
                mapInfo.isHavePath=true;
            }
            if(info.fileName()=="Points.json"){
                QFile pathfile(info.filePath());
                pathfile.open(QIODevice::ReadOnly);
                QJsonObject obj=QJsonDocument::fromJson(pathfile.readAll()).object();
                pathfile.close();
                QJsonObject initObj=obj.value("InitPoint").toObject();
                mapInfo.initPoint.x=initObj.value("x").toDouble();
                mapInfo.initPoint.y=initObj.value("y").toDouble();
                mapInfo.initPoint.a=initObj.value("a").toDouble();
                QJsonObject chargeObj=obj.value("ChargePoint").toObject();
                mapInfo.chargePoint.x=chargeObj.value("x").toDouble();
                mapInfo.chargePoint.y=chargeObj.value("y").toDouble();
                mapInfo.chargePoint.a=chargeObj.value("a").toDouble();
                mapInfo.isHavePoint=true;
            }
        }
    }
    if(mapInfo.isHaveMap&&mapInfo.isHavePath&&mapInfo.isHaveYaml&&mapInfo.isHavePoint){
        mapInfo.canUse=true;
    }
    return mapInfo;
}
QString TOOL::GetStringFromStringList(QStringList list){
    QString str;
    int len=list.length();
    for(int i=0;i<len;i++){
        if(i==len-1){
            str+=list[i];
        }else{
            str+=list[i]+"，";
        }
    }
    return str;
}
//////////////////////////
HMI::HMI(QObject* parent):QObject(parent){}
HMI* hmi=new HMI();
//////////////////////////
NOTE::NOTE(){}
void NOTE::message(MESSAGE_TYPE type, QString title, QString info){
    QString _nowDay = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString _nowFilePath = QString("%1/%2.txt").arg(NOTE_PATH).arg(_nowDay);
    QFile _nowFile(_nowFilePath);
    QString message = createMessage(type,title,info);
    if(!_nowFile.exists()){
        // 新建文建
        removeFrontFile();
        _nowFile.open(QIODevice::WriteOnly |QIODevice::Append|QIODevice::Text);
        _nowFile.write("————————————————————————————————————————\n");
        _nowFile.write("仅保存一个月的日志文件\n");
        _nowFile.write(QString("日期：%1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd dddd")).toUtf8());
        _nowFile.write("格式：yyyy-MM-dd hh:mm:ss:zzz [消息来源] (类型) : 内容\n");
        _nowFile.write("————————————————————————————————————————\n");
        _nowFile.write(message.toUtf8());
        _nowFile.flush();
        _nowFile.close();
    }else{
        _nowFile.open(QIODevice::WriteOnly |QIODevice::Append|QIODevice::Text);
        _nowFile.write(message.toUtf8());
        _nowFile.flush();
        _nowFile.close();
    }
}
void NOTE::removeFrontFile(){
    QString _frontMonthDay = QDateTime::currentDateTime().addMonths(-1).toString("yyyy-MM-dd");
    QString _frontMonthFilePath =QString("%1/%2.txt").arg(NOTE_PATH).arg(_frontMonthDay);
    QFile _frontMonthFile(_frontMonthFilePath);
    if(_frontMonthFile.exists()){
        QFileInfo _frontMonthFileInfo(_frontMonthFile);
        qint64 modifyTime = _frontMonthFileInfo.lastModified().toSecsSinceEpoch();
        QDir dir(NOTE_PATH);
        foreach (const QFileInfo& info, dir.entryInfoList()){
            if(info.lastModified().toSecsSinceEpoch()<=modifyTime){
                QFile file(info.filePath());
                file.remove();
            }
        }
        //清理Ros日志
        QDir rosDir(ROS_NOTE_PATH);
        if(rosDir.exists()){
            foreach(const QFileInfo& fileInfo,rosDir.entryInfoList()){
                if(fileInfo.lastModified().toSecsSinceEpoch()<=modifyTime){
                    QFile rosfile(fileInfo.path());
                    rosfile.remove();
                }
            }
        }
    }
}
QString NOTE::createMessage(MESSAGE_TYPE type,QString title,QString info){
    QString _nowTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString _type;
    switch (type) {
    case MT_HMI:
        _type="HMI";
        break;
    case MT_ROS:
        _type="ROS";
        break;
    case MT_FROM_ROS:
        _type="ROS->HMI";
        break;
    case MT_TO_ROS:
        _type="HMI->ROS";
        break;
    default:
        break;
    }
    QString message=QString("%1 [%2] (%3) : %4\n").arg(_nowTime).arg(_type).arg(title).arg(info);
    qDebug()<<message;
    return message;
}
NOTE* note=new NOTE();
//////////////////////////
void DATA::init(){
    QFile file(CONFIG_PATH);
    file.open(QIODevice::ReadOnly);
    QByteArray byte=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(byte);
    QJsonObject obj=doc.object();
    //////////
    QJsonObject taskInfoObj=obj.value("taskInfo").toObject();
    taskInfo.map=taskInfoObj.value("map").toString();
    taskInfo.areaList.clear();
    QJsonArray areaArray=taskInfoObj.value("areaList").toArray();
    for(int i=0;i<areaArray.size();i++){
        taskInfo.areaList.append(areaArray[i].toString());
    }
    taskInfo.roundNumber=taskInfoObj.value("roundNumber").toInt();
    mapInfo=TOOL::GetMapInfoFromDir(taskInfo.map);
}
void DATA::save(){
    QJsonObject taskInfoObj;
    taskInfoObj.insert("map",taskInfo.map);
    taskInfoObj.insert("roundNumber",taskInfo.roundNumber);
    QJsonArray areaArray;
    for(int i=0;i<taskInfo.areaList.length();i++){
        areaArray.append(taskInfo.areaList[i]);
    }
    taskInfoObj.insert("areaList",areaArray);
    //////////
    QJsonObject obj;
    obj.insert("taskInfo",taskInfoObj);
    QJsonDocument doc;
    doc.setObject(obj);
    QFile file(CONFIG_PATH);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}
DATA* record=new DATA();
