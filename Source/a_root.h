#ifndef A_ROOT_H
#define A_ROOT_H
//////////////////////////
#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QIcon>
#include <QPushButton>
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QStackedLayout>
#include <QDir>
#include <QJsonDocument>
#include <QPropertyAnimation>
#include <QButtonGroup>
#include <QDialog>
#include <QFormLayout>
#include <QTimer>
#include <QTcpSocket>
#include "yaml-cpp/yaml.h"
//////////////////////////
#define BackColor "#F4F3F8"
#define FrontColor "#FFFFFF"
#define LineColor "#E2E1E6"
#define GreenColor "#32C95E"
#define FontColor "#4B4B4B"
#define TipColor "#9A999E"
#define GrayColor "#919191"
#define BlueColor "#007aff"
#define CleanWaterColor "#a6e3e9"
#define DirtyWaterColor "#f08a5d"
#define FluidWaterColor "#00b8a9"
//////////////////////////
#define MAP_PATH QDir::homePath()+"/jld_profile/maps"
#define NOTE_PATH QDir::homePath()+"/CleanRobot/CleanRobot/Note"
#define ROS_NOTE_PATH QDir::homePath()+"/jld_profile/log"
#define CONFIG_PATH QDir::homePath()+"/CleanRobot/CleanRobot/Config/config.json"
//////////////////////////
struct XyaPoint{
    double x;
    double y;
    double a;
};
struct XyzwPoint{
    double x;
    double y;
    double z;
    double w;
};
struct TaskInfo{
    QString map;
    QStringList areaList;
    quint8 roundNumber;
};
struct MapInfo{
    QString mapDir;
    QString relativeMapPath;//相对路径
    QString absoluteMapPath;//绝对路径
    double resolution;//比例尺
    double originX;//原点X
    double originY;//原点Y
    XyaPoint initPoint;
    XyaPoint chargePoint;
    QMap<QString,QJsonArray> areaPolygonMap;
    QMap<QString,QJsonArray> areaPathMap;
    bool isHaveMap=false;
    bool isHaveYaml=false;
    bool isHavePoint=false;
    bool isHavePath=false;
    bool canUse=false;
};
enum CentralPageType{
    CP_central,
    CP_set,
    CP_task,
    CP_status,
    CP_chooseMap,
    CP_chooseArea,
};
enum GEAR{
    GR_CLOSE=0, //关闭
    GR_ONE=1,   //1档
    GR_TWO=2,   //2档
    GR_THREE=3  //3档
};
enum BOOL{
    BL_CLOSE=0,
    BL_OPEN=1
};
struct ROBOT_STATE{
    bool charge;//是否在充电
    bool collision; //碰撞是否异常
    double cur; //电流
    bool doorState; //门的状态
    bool emergency; //急停按钮是否触发
    bool imu; //陀螺仪是否异常
    quint8 mah; //电池电量0～100
    QString mapFile;//地图路径名
    QString mcuHwVersion;//下位机版本
    QString mcuSoftVersion;//下位机软件版本
    qint32 mils;//里程数
    quint8 relay;//继电器状态 0关机 1正常 2充电
    QString rosHwVersion;//ros下位机版本
    QString rosSoftVersion;//ros软件版本
    quint8 skip;//环境匹配度 0～100 值越大越不匹配
    bool sonar;//超声波是否异常
    QString type;//类型为robotState才可解析位置数据
    double vol;//电压
    bool wheel;//轮子是否异常
    XyaPoint point;//位置
    XyzwPoint xyzwPoint;
    GEAR cleanWaterOut;
    GEAR cleanWaterIn;
    GEAR cleanBrush;
    BOOL cleanPush;
};
enum RESULT_STATUS{
    RS_SUCCESS,
    RS_FAIL,
    RS_RUNNING
};
//////////////////////////
class TOOL{
public:
    static double GetAngleFromTwoPoint(QPointF startPos,QPointF endPos);
    static XyzwPoint GetXYZWPOINTFromXYAPOINT(XyaPoint point);
    static MapInfo GetMapInfoFromDir(QString dirPath);
    static QString GetStringFromStringList(QStringList list);
};
//////////////////////////
class HMI:public QObject{
    Q_OBJECT
public:
    explicit HMI(QObject* parent=nullptr);
signals:
    void setPageSignal(CentralPageType);//切换页面
    void exitSignal();//关闭软件
    void returnPageSignal();
    void showStatusSignal(QString);//显示状态
    void mapChangedSignal();
    void areaChangedSignal();
};
extern HMI* hmi;
//////////////////////////
class NOTE{
public:
    enum MESSAGE_TYPE{
        MT_FROM_ROS,
        MT_TO_ROS,
        MT_HMI,
        MT_ROS
    };
public:
    NOTE();
    void message(MESSAGE_TYPE type,QString title,QString info);
private:
    void removeFrontFile();
    QString createMessage(MESSAGE_TYPE type,QString title,QString info);
};
extern NOTE* note;
//////////////////////////
class DATA{
public:
    void init();
    void save();
public:
    TaskInfo taskInfo;
    MapInfo mapInfo;
};
extern DATA* record;
#endif // A_ROOT_H
