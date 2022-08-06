#include "b_widget.h"
///////////////////////////////
///////////////////////////////
//          Widget          ///
///////////////////////////////
///////////////////////////////
SlideWidget::SlideWidget(QWidget* parent):QWidget(parent){}
void SlideWidget::setCentralWidget(QWidget *centralWidget){
    _centralWidget=centralWidget;
}
void SlideWidget::mousePressEvent(QMouseEvent *event){
    _startY=event->pos().y();
    _startCY=_cY;
}
void SlideWidget::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    _cY=_centralWidget->y();
}
void SlideWidget::mouseMoveEvent(QMouseEvent *event){
    int distance=event->pos().y()-_startY;
    _cY=_startCY+distance;
    double ch = _centralWidget->height();
    double h = height();
    if(_cY>0){_cY=0;}
    if(ch>h&&_cY<(h-ch)){_cY = h-ch;}
    if(ch<=h){_cY=0;}
    _centralWidget->move(0,_cY);
}
void SlideWidget::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    _centralWidget->setParent(this);
    _centralWidget->move(0,0);
    _centralWidget->setFixedWidth(width());
}
ItemsWidget::ItemsWidget(QWidget *parent):QWidget(parent){
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(0,0,0,0);
    _layout->setSpacing(0);
    setLayout(_layout);
}
void ItemsWidget::addItem(QWidget *item){
    item->setParent(this);
    item->setFixedHeight(50);
    _layout->addWidget(item);
    _items.append(item);
    update();
}
int ItemsWidget::indexOf(QWidget* item){
    if(_items.contains(item)){
        int index=_items.indexOf(item);
        if(index==0){
            return 1;
        }else if(index==_items.size()-1){
            return -1;
        }else{
            return 0;
        }
    }else{
        return -2;
    }
}
void ItemsWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(FrontColor)));
    painter.drawRoundedRect(rect(),15,15);
    painter.setBrush(QBrush(QColor(LineColor)));
    int size=_items.size();
    for(int i=1;i<size;i++){
        QRectF lineRect=QRectF(70,i*50-1,width()-70,1.5);
        painter.drawRect(lineRect);
    }
}
SwitchItem::SwitchItem(QWidget* parent):QWidget(parent){}
void SwitchItem::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void SwitchItem::setTitle(QString title){
    _title=title;
    update();
}
void SwitchItem::setBool(bool isTrue){
    _isTrue=isTrue;
    update();
}
void SwitchItem::mousePressEvent(QMouseEvent *event){
    QRect switchRect(width()-70,0,70,height());
    if(switchRect.contains(event->pos())){
        _canMove=false;
        _isTrue=_isTrue?false:true;
        emit switchSignal(_isTrue);
        update();
    }else{
        _canMove=true;
        QWidget::mousePressEvent(event);
    }
}
void SwitchItem::mouseMoveEvent(QMouseEvent *event){
    if(_canMove){
        QWidget::mouseMoveEvent(event);
    }
}
void SwitchItem::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    QRect pixmapRect(15,5,40,40);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QRect titleRect(70,0,width()-70,height());
    painter.setPen(QColor(FontColor));
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(titleRect,Qt::AlignLeft|Qt::AlignVCenter,_title);
    painter.setPen(Qt::NoPen);
    QBrush brush;
    QRect switchRect(width()-85,5,70,40);
    QRect ellispeRect;
    if(_isTrue){
        brush=QBrush(QColor(GreenColor));
        ellispeRect=QRect(width()-17-36,7,36,36);
    }else{
        brush=QBrush(QColor(GrayColor));
        ellispeRect=QRect(width()-83,7,36,36);
    }
    painter.setBrush(brush);
    painter.drawRoundedRect(switchRect,20,20);
    painter.setBrush(QBrush(QColor(FrontColor)));
    painter.drawEllipse(ellispeRect);
}
EnterItem::EnterItem(QWidget* parent):QWidget(parent){
    _enterPixmap=QPixmap(":/Image/EnterItemPix.png");
}
void EnterItem::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void EnterItem::setTitle(QString title){
    _title=title;
    update();
}
void EnterItem::setValue(QString value){
    _value=value;
    update();
}
void EnterItem::mousePressEvent(QMouseEvent *event){
    Q_UNUSED(event);
    _isPress=true;
    update();
    QWidget::mousePressEvent(event);
}
void EnterItem::mouseReleaseEvent(QMouseEvent *event){
    _isPress=false;
    _startY=event->pos().x();
    emit enterSignal();
    update();
    QWidget::mouseReleaseEvent(event);
}
void EnterItem::mouseMoveEvent(QMouseEvent *event){
    int distance=event->pos().y()-_startY;
    if(distance>20){
        QWidget::mouseMoveEvent(event);
    }
}
void EnterItem::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    if(_isPress){
        QPainterPath backPath;
        ItemsWidget* parent=qobject_cast<ItemsWidget*>(parentWidget());
        int index=parent->indexOf(this);
        if(index==1){
            QPainterPath top;
            top.addRoundedRect(rect(),15,15);
            QPainterPath bot;
            bot.addRect(QRect(0,20,width(),height()-20));
            backPath=top+bot;
        }else if(index==-1){
            QPainterPath top;
            top.addRoundedRect(rect(),15,15);
            QPainterPath bot;
            bot.addRect(QRect(0,0,width(),20));
            backPath=top+bot;
        }else{
            backPath.addRect(rect());
        }
        painter.setPen(Qt::NoPen);
        QColor color("#000000");
        color.setAlpha(50);
        painter.setBrush(QBrush(color));
        painter.drawPath(backPath);
    }
    QRect pixmapRect(15,5,40,40);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QRect titleRect(70,0,width()-115,height());
    painter.setPen(QColor(FontColor));
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(titleRect,Qt::AlignLeft|Qt::AlignVCenter,_title);
    painter.setPen(QColor(GrayColor));
    painter.drawText(titleRect,Qt::AlignRight|Qt::AlignVCenter,_value);
    QRect enterPixmapRect(width()-35,15,20,20);
    painter.drawPixmap(enterPixmapRect,_enterPixmap,_enterPixmap.rect());
}
NumberItem::NumberItem(QWidget* parent):QWidget(parent){
    _minusPixmap=QPixmap(":/Image/NumberItemMinus.png");
    _plusPixmap=QPixmap(":/Image/NumberItemPlus.png");
}
void NumberItem::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void NumberItem::setTitle(QString title){
    _title=title;
    update();
}
void NumberItem::setValue(quint8 value){
    _value=value;
    update();
}
void NumberItem::setTip(QString tip){
    _tip=tip;
    update();
}
void NumberItem::mousePressEvent(QMouseEvent *event){
    QRect plusRect(width()-65,0,50,height());
    QRect minusRect(width()-165,0,50,height());
    if(plusRect.contains(event->pos())){
        if(_value<200){
            _value++;
            emit numberSignal(_value);
            update();
        }
        _canMove=false;
    }else if(minusRect.contains(event->pos())){
        if(_value>0){
            _value--;
            emit numberSignal(_value);
            update();
        }
        _canMove=false;
    }else{
        _canMove=true;
        QWidget::mousePressEvent(event);
    }
}
void NumberItem::mouseMoveEvent(QMouseEvent *event){
    if(_canMove){
        QWidget::mouseMoveEvent(event);
    }
}
void NumberItem::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    QRect pixmapRect(15,5,40,40);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QRect titleRect(70,0,width()-240,height());
    painter.setPen(QColor(FontColor));
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(titleRect,Qt::AlignLeft|Qt::AlignVCenter,_title);
    painter.setPen(QColor(TipColor));
    painter.drawText(titleRect,Qt::AlignRight|Qt::AlignVCenter,_tip);
    QRect backRect(width()-165,5,150,height()-10);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(GrayColor)));
    painter.drawRoundedRect(backRect,20,20);
    painter.setPen(QPen(QColor(FrontColor)));
    painter.drawText(backRect,Qt::AlignCenter,QString::number(_value));
    QPainterPath plusPath;
    QPainterPath plusPath0;
    QPainterPath plusPath1;
    plusPath0.addRect(QRect(width()-145,7,29,36));
    plusPath1.addRoundedRect(QRect(width()-163,7,36,36),18,18);
    plusPath=plusPath0+plusPath1;
    QPainterPath minusPath;
    QPainterPath minusPath0;
    QPainterPath minusPath1;
    minusPath0.addRect(QRect(width()-64,7,29,36));
    minusPath1.addRoundedRect(QRect(width()-53,7,36,36),18,18);
    minusPath=minusPath0+minusPath1;
    painter.setBrush(QBrush(QColor(FrontColor)));
    painter.drawPath(minusPath);
    painter.drawPath(plusPath);
    QRect minusRect(width()-150,15,20,20);
    QRect plusRect(width()-50,15,20,20);
    painter.drawPixmap(plusRect,_plusPixmap,_plusPixmap.rect());
    painter.drawPixmap(minusRect,_minusPixmap,_minusPixmap.rect());
}
GradeItem::GradeItem(QWidget* parent):QWidget(parent){}
void GradeItem::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void GradeItem::setTitle(QString title){
    _title=title;
    update();
}
void GradeItem::setValue(quint8 value){
    _value=value;
    update();
}
void GradeItem::mousePressEvent(QMouseEvent *event){
    QRect zeroRect(width()-215,0,50,height());
    QRect lowRect(width()-165,0,50,height());
    QRect midRect(width()-115,0,50,height());
    QRect upRect(width()-65,0,50,height());
    if(lowRect.contains(event->pos())){
        _value=1;
        emit gradeSignal(GR_ONE);
        update();
        _canMove=false;
    }else if(midRect.contains(event->pos())){
        _value=2;
        emit gradeSignal(GR_TWO);
        update();
        _canMove=false;
    }else if(upRect.contains(event->pos())){
        _value=3;
        emit gradeSignal(GR_THREE);
        update();
        _canMove=false;
    }else if(zeroRect.contains(event->pos())){
        _value=0;
        emit gradeSignal(GR_CLOSE);
        update();
        _canMove=false;
    }else{
        _canMove=true;
    }
    QWidget::mousePressEvent(event);
}
void GradeItem::mouseMoveEvent(QMouseEvent *event){
    if(_canMove){
        QWidget::mouseMoveEvent(event);
    }
}
void GradeItem::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    QRect pixmapRect(15,5,40,40);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QRect titleRect(70,0,width()-70,height());
    painter.setPen(QColor(FontColor));
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(titleRect,Qt::AlignLeft|Qt::AlignVCenter,_title);
    QRect gradeRect(width()-215,5,200,height()-10);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(GrayColor)));
    painter.drawRoundedRect(gradeRect,20,20);
    QPainterPath zeroPath;
    QPainterPath zeroPath0;
    QPainterPath zeroPath1;
    zeroPath0.addRect(QRect(width()-195,7,29,36));
    zeroPath1.addRoundedRect(QRect(width()-213,7,36,36),18,18);
    zeroPath=zeroPath0+zeroPath1;
    QPainterPath lowPath;
    lowPath.addRect(QRect(width()-164,7,48,36));
    QPainterPath midPath;
    midPath.addRect(QRect(width()-114,7,48,36));
    QPainterPath upPath;
    QPainterPath upPath0;
    QPainterPath upPath1;
    upPath0.addRect(QRect(width()-64,7,29,36));
    upPath1.addRoundedRect(QRect(width()-53,7,36,36),18,18);
    upPath=upPath0+upPath1;
    QRect zeroRect(width()-215,0,50,height());
    QRect lowRect(width()-165,0,50,height());
    QRect midRect(width()-115,0,50,height());
    QRect upRect(width()-65,0,50,height());
    if(_value==1){
        painter.setBrush(QBrush(QColor(GrayColor)));
        painter.drawPath(midPath);
        painter.drawPath(upPath);
        painter.drawPath(zeroPath);
        painter.setBrush(QBrush(QColor(FrontColor)));
        painter.drawPath(lowPath);
        painter.setPen(QColor(FrontColor));
        painter.drawText(zeroRect,Qt::AlignCenter,"关");
        painter.drawText(midRect,Qt::AlignCenter,"中");
        painter.drawText(upRect,Qt::AlignCenter,"高");
        painter.setPen(QColor(TipColor));
        painter.drawText(lowRect,Qt::AlignCenter,"低");
    }else if(_value==2){
        painter.setBrush(QBrush(QColor(GrayColor)));
        painter.drawPath(lowPath);
        painter.drawPath(upPath);
        painter.drawPath(zeroPath);
        painter.setBrush(QBrush(QColor(FrontColor)));
        painter.drawPath(midPath);
        painter.setPen(QColor(FrontColor));
        painter.drawText(zeroRect,Qt::AlignCenter,"关");
        painter.drawText(lowRect,Qt::AlignCenter,"低");
        painter.drawText(upRect,Qt::AlignCenter,"高");
        painter.setPen(QColor(TipColor));
        painter.drawText(midRect,Qt::AlignCenter,"中");
    }else if(_value==3){
        painter.setBrush(QBrush(QColor(GrayColor)));
        painter.drawPath(lowPath);
        painter.drawPath(midPath);
        painter.drawPath(zeroPath);
        painter.setBrush(QBrush(QColor(FrontColor)));
        painter.drawPath(upPath);
        painter.setPen(QColor(FrontColor));
        painter.drawText(zeroRect,Qt::AlignCenter,"关");
        painter.drawText(lowRect,Qt::AlignCenter,"低");
        painter.drawText(midRect,Qt::AlignCenter,"中");
        painter.setPen(QColor(TipColor));
        painter.drawText(upRect,Qt::AlignCenter,"高");
    }else if(_value==0){
        painter.setBrush(QBrush(QColor(GrayColor)));
        painter.drawPath(lowPath);
        painter.drawPath(midPath);
        painter.drawPath(upPath);
        painter.setBrush(QBrush(QColor(FrontColor)));
        painter.drawPath(zeroPath);
        painter.setPen(QColor(FrontColor));
        painter.drawText(lowRect,Qt::AlignCenter,"低");
        painter.drawText(midRect,Qt::AlignCenter,"中");
        painter.drawText(upRect,Qt::AlignCenter,"高");
        painter.setPen(QColor(TipColor));
        painter.drawText(zeroRect,Qt::AlignCenter,"关");
    }
}
MapButton::MapButton(QWidget* parent):QPushButton(parent){
    setStyleSheet("QPushButton{background-color:#FFFFFF;border:none;border-radius:5px;}QPushButton:pressed{background-color:#718093;}");
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@MapAreaFrame@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
MapAreaFrame::MapAreaFrame(QWidget* parent):QWidget(parent){
    _zoomInButton=new MapButton(this);
    _zoomInButton->setFixedSize(50,50);
    _zoomInButton->setIcon(QIcon(":/Image/MapButtonZoomin.png"));
    _zoomInButton->setIconSize(QSize(50,50));
    _zoomOutButton=new MapButton(this);
    _zoomOutButton->setFixedSize(50,50);
    _zoomOutButton->setIcon(QIcon(":/Image/MapButtonZoomout.png"));
    _zoomOutButton->setIconSize(QSize(50,50));
    _resetButton=new MapButton(this);
    _resetButton->setFixedSize(50,50);
    _resetButton->setIcon(QIcon(":/Image/MapButtonReset.png"));
    _resetButton->setIconSize(QSize(50,50));
    _zoomInButton->hide();
    _zoomOutButton->hide();
    _resetButton->hide();
    _colorList<<QColor("#FF0000")<<QColor("#00FF00")<<QColor("#0000FF")<<QColor("#FF00FF")<<QColor("#00FFFF")<<QColor("#70DB93")<<QColor("#5C3317")<<QColor("#9F5F9F")<<QColor("#B5A642")<<QColor("#871F78");
    connect(_zoomInButton,SIGNAL(clicked()),this,SLOT(zoomIn()));
    connect(_zoomOutButton,SIGNAL(clicked()),this,SLOT(zoomOut()));
    connect(_resetButton,&QPushButton::clicked,[this](){setMapInfo(_info);});
}
void MapAreaFrame::setMapInfo(MapInfo info){
    _info=info;
    _mapPix=QPixmap(info.absoluteMapPath);
    _mapPixRect=_mapPix.rect();
    double wr=width()/(float)_mapPixRect.width();
    double hr=height()/(float)_mapPixRect.height();
    _scale=(wr<=hr)?wr:hr;
    _mapDrawRect.setX((width()-_scale*_mapPixRect.width())/2.0);
    _mapDrawRect.setY((height()-_scale*_mapPixRect.height())/2.0);
    _mapDrawRect.setWidth(_scale*_mapPixRect.width());
    _mapDrawRect.setHeight(_scale*_mapPixRect.height());
    _zoomInButton->show();
    _zoomOutButton->show();
    _resetButton->show();
    update();
}
void MapAreaFrame::resizeEvent(QResizeEvent *){
    int x=width()-60;
    _zoomOutButton->move(x,10);
    _zoomInButton->move(x,70);
    _resetButton->move(x,130);
    setMapInfo(_info);
}
void MapAreaFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QColor backgroundColor("#FFFFFF");
    backgroundColor.setAlpha(30);
    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundColor);
    painter.drawRect(rect());
    painter.drawPixmap(_mapDrawRect,_mapPix,_mapPix.rect());
    quint8 count=0;
    foreach(QString pathKey,_info.areaPolygonMap.keys()){
        QJsonArray pathArray=_info.areaPolygonMap.value(pathKey);
        QPainterPath path;
        QPointF startPos;
        for(int i=0;i<pathArray.size();i++){
            QJsonArray p=pathArray[i].toArray();
            QPointF localPos=localPosFromSourcePos(QPointF(p[0].toDouble(),p[1].toDouble()));
            if(i==0){
                startPos=localPos;
                path=QPainterPath(localPos);
            }else if(i==pathArray.size()-1){
                path.lineTo(localPos);
                path.lineTo(startPos);
            }else{
                path.lineTo(localPos);
            }
        }
        QColor brushcolor;
        QColor pencolor;
        brushcolor=_colorList[count%10];
        pencolor=_colorList[count%10];
        brushcolor.setAlpha(50);
        pencolor.setAlpha(200);
        QPen pen(pencolor);
        pen.setWidth(3);
        painter.setBrush(QBrush(brushcolor));
        painter.setPen(pen);
        painter.drawPath(path);
    }
    painter.restore();
}
void MapAreaFrame::mousePressEvent(QMouseEvent* event){
    _isMousePress=true;
    _mousePressPos=event->pos();
    _mousePressTimeMapDrawRect=_mapDrawRect;
}
void MapAreaFrame::mouseReleaseEvent(QMouseEvent*){
    _isMousePress=false;
}
void MapAreaFrame::mouseMoveEvent(QMouseEvent *event){
    QPointF distance=event->pos()-_mousePressPos;
    double nx=_mousePressTimeMapDrawRect.x()+distance.x();
    double ny=_mousePressTimeMapDrawRect.y()+distance.y();
    if(nx>width()-20){
        nx=width()-20;
    }else if(nx<20-_mapDrawRect.width()){
        nx=20-_mapDrawRect.width();
    }
    if(ny>height()-20){
        ny=height()-20;
    }else if(ny<20-_mapDrawRect.height()){
        ny=20-_mapDrawRect.height();
    }
    _mapDrawRect.setX(nx);
    _mapDrawRect.setY(ny);
    _mapDrawRect.setWidth(_mousePressTimeMapDrawRect.width());
    _mapDrawRect.setHeight(_mousePressTimeMapDrawRect.height());
    update();
}
void MapAreaFrame::zoomIn(){
    _scale=_scale*0.9;
    double nx=width()/2.0+(_mapDrawRect.x()-width()/2.0)*0.9;
    double ny=height()/2.0+(_mapDrawRect.y()-height()/2.0)*0.9;
    if(nx>width()-20){
        nx=width()-20;
    }else if(nx<20-_mapDrawRect.width()){
        nx=20-_mapDrawRect.width();
    }
    if(ny>height()-20){
        ny=height()-20;
    }else if(ny<20-_mapDrawRect.height()){
        ny=20-_mapDrawRect.height();
    }
    _mapDrawRect.setX(nx);
    _mapDrawRect.setY(ny);
    _mapDrawRect.setWidth(_scale*_mapPixRect.width());
    _mapDrawRect.setHeight(_scale*_mapPixRect.height());
    update();
}
void MapAreaFrame::zoomOut(){
    _scale=_scale*1.1;
    double nx=width()/2.0+(_mapDrawRect.x()-width()/2.0)*1.1;
    double ny=height()/2.0+(_mapDrawRect.y()-height()/2.0)*1.1;
    if(nx>width()-20){
        nx=width()-20;
    }else if(nx<20-_mapDrawRect.width()){
        nx=20-_mapDrawRect.width();
    }
    if(ny>height()-20){
        ny=height()-20;
    }else if(ny<20-_mapDrawRect.height()){
        ny=20-_mapDrawRect.height();
    }
    _mapDrawRect.setX(nx);
    _mapDrawRect.setY(ny);
    _mapDrawRect.setWidth(_scale*_mapPixRect.width());
    _mapDrawRect.setHeight(_scale*_mapPixRect.height());
    update();
}
QPointF MapAreaFrame::localPosFromSourcePos(QPointF pos){
    double x=(pos.x()-_info.originX)/_info.resolution*_scale+_mapDrawRect.x();
    double y=(_mapPixRect.height()-(pos.y()-_info.originY)/_info.resolution)*_scale+_mapDrawRect.y();
    return QPointF(x,y);
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@MapPathFrame@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
MapPathFrame::MapPathFrame(QWidget* parent):QWidget(parent){
    _zoomInButton=new MapButton(this);
    _zoomInButton->setFixedSize(50,50);
    _zoomInButton->setIcon(QIcon(":/Image/MapButtonZoomin.png"));
    _zoomInButton->setIconSize(QSize(50,50));
    _zoomOutButton=new MapButton(this);
    _zoomOutButton->setFixedSize(50,50);
    _zoomOutButton->setIcon(QIcon(":/Image/MapButtonZoomout.png"));
    _zoomOutButton->setIconSize(QSize(50,50));
    _resetButton=new MapButton(this);
    _resetButton->setFixedSize(50,50);
    _resetButton->setIcon(QIcon(":/Image/MapButtonReset.png"));
    _resetButton->setIconSize(QSize(50,50));
    _zoomInButton->hide();
    _zoomOutButton->hide();
    _resetButton->hide();
    _colorList<<QColor("#FF0000")<<QColor("#00FF00")<<QColor("#0000FF")<<QColor("#FF00FF")<<QColor("#00FFFF")<<QColor("#70DB93")<<QColor("#5C3317")<<QColor("#9F5F9F")<<QColor("#B5A642")<<QColor("#871F78");
    connect(_zoomInButton,SIGNAL(clicked()),this,SLOT(zoomIn()));
    connect(_zoomOutButton,SIGNAL(clicked()),this,SLOT(zoomOut()));
    connect(_resetButton,&QPushButton::clicked,[this](){setMapInfo(_info);});
}
void MapPathFrame::setMapInfo(MapInfo info){
    _info=info;
    _mapPix=QPixmap(info.absoluteMapPath);
    _mapPixRect=_mapPix.rect();
    double wr=width()/(float)_mapPixRect.width();
    double hr=height()/(float)_mapPixRect.height();
    _scale=(wr<=hr)?wr:hr;
    _mapDrawRect.setX((width()-_scale*_mapPixRect.width())/2.0);
    _mapDrawRect.setY((height()-_scale*_mapPixRect.height())/2.0);
    _mapDrawRect.setWidth(_scale*_mapPixRect.width());
    _mapDrawRect.setHeight(_scale*_mapPixRect.height());
    _zoomInButton->show();
    _zoomOutButton->show();
    _resetButton->show();
    update();
}
void MapPathFrame::setAreas(QStringList areas){
    _areasList=areas;
    quint8 sweepAreaNumber=areas.length();
    double length=0;
    foreach(QString pathKey,areas){
        QJsonArray pathArray=_info.areaPathMap.value(pathKey);
        for(int i=0;i<pathArray.size()-1;i++){
            QJsonArray p0=pathArray[i].toArray();
            QJsonArray p1=pathArray[i+1].toArray();
            double x0=p0[0].toDouble();
            double y0=p0[1].toDouble();
            double x1=p1[0].toDouble();
            double y1=p1[1].toDouble();
            double distance=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
            length+=distance;
        }
    }
    emit sendSweepAreaNumAndLength(sweepAreaNumber,length);
    update();
}
void MapPathFrame::resizeEvent(QResizeEvent *){
    int x=width()-60;
    _zoomOutButton->move(x,10);
    _zoomInButton->move(x,70);
    _resetButton->move(x,130);
    setMapInfo(_info);
}
void MapPathFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QColor backgroundColor("#FFFFFF");
    backgroundColor.setAlpha(30);
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(),5,5);
    painter.drawPixmap(_mapDrawRect,_mapPix,_mapPix.rect());
    quint8 areaCount=0;
    QPainterPath path;
    foreach(QString pathKey,_info.areaPathMap.keys()){
        if(_areasList.contains(pathKey)){
            QJsonArray pathArray=_info.areaPathMap.value(pathKey);
            QPointF startPos;
            for(int i=0;i<pathArray.size();i++){
                QJsonArray p=pathArray[i].toArray();
                QPointF localPos=localPosFromSourcePos(QPointF(p[0].toDouble(),p[1].toDouble()));
                if(i==0&&areaCount==0){
                    startPos=localPos;
                    path=QPainterPath(localPos);
                }else{
                    path.lineTo(localPos);
                }
            }
        }
    }
    QColor brushcolor;
    QColor pencolor;
    pencolor=_colorList[0];
    pencolor.setAlpha(200);
    QPen pen(pencolor);
    pen.setWidth(1);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);
    painter.drawPath(path);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    QPointF initPoint=localPosFromSourcePos(QPointF(_info.initPoint.x,_info.initPoint.y));
    QPointF chargePoint=localPosFromSourcePos(QPointF(_info.chargePoint.x,_info.chargePoint.y));
    painter.drawEllipse(initPoint,3,3);
    painter.drawEllipse(chargePoint,3,3);
    painter.setPen(Qt::blue);
    painter.drawText(initPoint,"初始化点");
    painter.drawText(chargePoint,"充电点");
    painter.restore();
}
void MapPathFrame::mousePressEvent(QMouseEvent* event){
    _isMousePress=true;
    _mousePressPos=event->pos();
    _mousePressTimeMapDrawRect=_mapDrawRect;
}
void MapPathFrame::mouseReleaseEvent(QMouseEvent*){
    _isMousePress=false;
}
void MapPathFrame::mouseMoveEvent(QMouseEvent *event){
    QPointF distance=event->pos()-_mousePressPos;
    double nx=_mousePressTimeMapDrawRect.x()+distance.x();
    double ny=_mousePressTimeMapDrawRect.y()+distance.y();
    if(nx>width()-20){
        nx=width()-20;
    }else if(nx<20-_mapDrawRect.width()){
        nx=20-_mapDrawRect.width();
    }
    if(ny>height()-20){
        ny=height()-20;
    }else if(ny<20-_mapDrawRect.height()){
        ny=20-_mapDrawRect.height();
    }
    _mapDrawRect.setX(nx);
    _mapDrawRect.setY(ny);
    _mapDrawRect.setWidth(_mousePressTimeMapDrawRect.width());
    _mapDrawRect.setHeight(_mousePressTimeMapDrawRect.height());
    update();
}
void MapPathFrame::zoomIn(){
    _scale=_scale*0.9;
    double nx=width()/2.0+(_mapDrawRect.x()-width()/2.0)*0.9;
    double ny=height()/2.0+(_mapDrawRect.y()-height()/2.0)*0.9;
    if(nx>width()-20){
        nx=width()-20;
    }else if(nx<20-_mapDrawRect.width()){
        nx=20-_mapDrawRect.width();
    }
    if(ny>height()-20){
        ny=height()-20;
    }else if(ny<20-_mapDrawRect.height()){
        ny=20-_mapDrawRect.height();
    }
    _mapDrawRect.setX(nx);
    _mapDrawRect.setY(ny);
    _mapDrawRect.setWidth(_scale*_mapPixRect.width());
    _mapDrawRect.setHeight(_scale*_mapPixRect.height());
    update();
}
void MapPathFrame::zoomOut(){
    _scale=_scale*1.1;
    double nx=width()/2.0+(_mapDrawRect.x()-width()/2.0)*1.1;
    double ny=height()/2.0+(_mapDrawRect.y()-height()/2.0)*1.1;
    if(nx>width()-20){
        nx=width()-20;
    }else if(nx<20-_mapDrawRect.width()){
        nx=20-_mapDrawRect.width();
    }
    if(ny>height()-20){
        ny=height()-20;
    }else if(ny<20-_mapDrawRect.height()){
        ny=20-_mapDrawRect.height();
    }
    _mapDrawRect.setX(nx);
    _mapDrawRect.setY(ny);
    _mapDrawRect.setWidth(_scale*_mapPixRect.width());
    _mapDrawRect.setHeight(_scale*_mapPixRect.height());
    update();
}
QPointF MapPathFrame::localPosFromSourcePos(QPointF pos){
    double x=(pos.x()-_info.originX)/_info.resolution*_scale+_mapDrawRect.x();
    double y=(_mapPixRect.height()-(pos.y()-_info.originY)/_info.resolution)*_scale+_mapDrawRect.y();
    return QPointF(x,y);
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@PowerFrame@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
PowerFrame::PowerFrame(QWidget* parent):QWidget(parent){}
void PowerFrame::setPower(quint8 power){
    _power=power;
    int r = power*(152-255)/100+255;
    int g = power*(251-0)/100+0;
    int b = power*(152-0)/100+0;
    _powerColor=QColor(r,g,b);
    update();
}
void PowerFrame::setCharge(bool isTrue){
    _isCharge=isTrue;
    update();
}
void PowerFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPainterPath border01;
    QRectF borderRect01(0,0,width()-5,height());
    border01.addRoundedRect(borderRect01,2,2);
    QPainterPath border02;
    QRectF borderRect02(width()-7,(height()-10)/2.0,5,10);
    border02.addRect(borderRect02);
    QPainterPath border03;
    QRectF borderRect03(width()-4,(height()-10)/2.0,4,10);
    border03.addRoundedRect(borderRect03,2,2);
    QPainterPath border04;
    QRectF borderRect04(2,2,width()-9,height()-4);
    border04.addRoundedRect(borderRect04,2,2);
    QPainterPath powerBorder=border01+border02+border03-border04;
    QPainterPath power;
    QRectF powerRect(3,3,(width()-13)/100.0*_power,height()-6);
    power.addRoundedRect(powerRect,2,2);
    if(_isCharge){
        double wunit=(width()-13)/4.0;
        double hunit=(height()-8)/3.0;
        QVector<QPointF> polygon01={
            QPointF(4+wunit*2,4),
            QPointF(4+wunit*2,4+hunit),
            QPointF(4+wunit*3,4),
            QPointF(4+wunit*2,4+hunit*3),
            QPointF(4+wunit*2,4+hunit*2),
            QPointF(4+wunit,4+hunit*3),
            QPointF(4+wunit*2,4)
            };
        QPainterPath charge;
        charge.addPolygon(QPolygonF(polygon01));
        power=power-charge;
        QPen pen;
        pen.setColor("#191970");
        pen.setWidth(2);
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor("#00D7FE")));
        painter.drawPath(charge);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(GrayColor)));
    painter.drawPath(powerBorder);
    painter.setBrush(QBrush(QColor(_powerColor)));
    painter.drawPath(power);
    painter.restore();
}
WaterFrame::WaterFrame(QWidget* parent):QWidget(parent){}
void WaterFrame::setWater(quint8 water){
    _water=water;
    update();
}
void WaterFrame::setType(WF_TYPE type){
    _type=type;
    update();
}
void WaterFrame::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(Qt::NoPen);
    QPainterPath border;
    QPainterPath border1;
    QPainterPath border2;
    border1.addRoundedRect(rect(),2,2);
    border2.addRoundedRect(QRect(2,0,width()-4,height()-2),2,2);
    border=border1-border2;
    painter.setBrush(QBrush(QColor(GrayColor)));
    painter.drawPath(border);
    QRect waterRect(3,(height()-3)*(1-_water/100.0),width()-6,(height()-3)*(_water/100.0));
    QColor color;
    if(_type==WF_Clean){
        color=QColor(CleanWaterColor);
    }else if(_type==WF_Dirty){
        color=QColor(DirtyWaterColor);
    }else{
        color=QColor(FluidWaterColor);
    }
    painter.setBrush(QBrush(color));
    painter.drawRoundedRect(waterRect,2,2);
}
StatusLabel::StatusLabel(QWidget* parent):QLabel(parent){
    setStyleSheet("color:#4B4B4B;font:bold 15pt \"Ubuntu\";");
}
PictureFrame::PictureFrame(QWidget* parent):QWidget(parent){}
void PictureFrame::setBool(bool isTrue){
    _isTrue=isTrue;
    update();
}
void PictureFrame::setTruePicture(QString path){
    _truePix=QPixmap(path);
}
void PictureFrame::setFalsePicture(QString path){
    _falsePix=QPixmap(path);
}
void PictureFrame::paintEvent(QPaintEvent* event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    if(_isTrue){
        painter.drawPixmap(rect(),_truePix,_truePix.rect());
    }else{
        painter.drawPixmap(rect(),_falsePix,_falsePix.rect());
    }
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@MenuButton@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
MenuButton::MenuButton(QWidget* parent):QWidget(parent){
    _aninormal = new QPropertyAnimation(this);
    _aninormal->setTargetObject(this);
    _aninormal->setStartValue(0);
    _aninormal->setEndValue(9);
    _aninormal->setDuration(100);
    connect(_aninormal,SIGNAL(valueChanged(QVariant)),this,SLOT(animationValueChangedSlot(QVariant)));
}
void MenuButton::setName(QString name){
    _name=name;
    update();
}
void MenuButton::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void MenuButton::setColor(QColor color){
    _color=color;
    update();
}
void MenuButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.setBrush(_color);
    painter.setPen(Qt::NoPen);
    QRectF rect(_border,_border,width()-_border*2,height()-_border*2);
    painter.drawRoundedRect(rect,10,10);
    QRectF pixmapRect((width()/2.0-height()/2.0)/2.0,height()/4.0,height()/2.0,height()/2.0);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QRectF textRect(width()/2.0,0,width()/2.0,height());
    painter.setPen(QColor("#FFFFFF"));
    QFont font;
    font.setPointSize(35);//字体点大小
    painter.setFont(font);
    painter.drawText(textRect,Qt::AlignLeft|Qt::AlignVCenter,_name);
    painter.restore();
}
void MenuButton::mousePressEvent(QMouseEvent *){
    _isPress=true;
    _aninormal->stop();
    _aninormal->start();
}
void MenuButton::mouseReleaseEvent(QMouseEvent *){
    _isPress=false;
    _aninormal->stop();
    _aninormal->start();
    emit clickedSignal();
}
void MenuButton::animationValueChangedSlot(QVariant value){
    Q_UNUSED(value);
    if(_isPress){
        if(_border<=10){
            _border+=1;
        }else{
            _aninormal->stop();
        }
    }else{
        if(_border>=0){
            _border-=1;
        }else{
            _aninormal->stop();
        }
    }
    update();
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@TitleFrame@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
TitleFrame::TitleFrame(QWidget* parent):QWidget(parent){
    setFixedHeight(50);
    _pixmap=QPixmap(":/Image/ReturnButton.png");
}
void TitleFrame::setTitle(QString title){
    _title=title;
    update();
}
void TitleFrame::mousePressEvent(QMouseEvent *e){
    QRect returnRect(width()-100,0,100,height());
    if(returnRect.contains(e->pos())){
        emit hmi->returnPageSignal();
    }
}
void TitleFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    QColor backgroundColor(GrayColor);
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(),5,5);
    QRectF pixmapRect(width()-100+(100-height()-10)/2.0,5,height()-10,height()-10);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QFont font;
    font.setPointSize(20);//字体点大小
    painter.setFont(font);
    painter.setPen(QColor("#FFFFFF"));
    painter.drawText(rect(),Qt::AlignCenter,_title);
    painter.restore();
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@SlideFrame@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
SlideFrame::SlideFrame(QWidget* parent):QWidget(parent){
    _centralFrame= new QFrame(this);
    _centralFrame->move(0,0);
    _vLayout = new QVBoxLayout(_centralFrame);
    _vLayout->setContentsMargins(0,0,0,0);
    _vLayout->setSpacing(5);
    _centralFrame->setLayout(_vLayout);
}
void SlideFrame::addItem(QWidget *item){
    item->setParent(_centralFrame);
    item->setFixedHeight(50);
    _vLayout->addWidget(item);
    _itemList.append(item);
    _height+=55;
    _centralFrame->setFixedHeight(_height);
}
QList<QWidget*> SlideFrame::items(){
    return _itemList;
}
void SlideFrame::clearItem(){
    QWidget* item;
    foreach (item, _itemList){
        _vLayout->removeWidget(item);
        item->deleteLater();
    }
    _itemList.clear();
    _height=0;
    _centralFrame->update();
}
void SlideFrame::keepPose(double y){
    double ch = _centralFrame->height();
    double h = height();
    if(y>0){y=0;}
    if(ch>h&&y<(h-ch)){y = h-ch;}
    if(ch<=h){y=0;}
    _centralFrame->move(0,y);
}
void SlideFrame::resizeEvent(QResizeEvent *){
    _centralFrame->setFixedWidth(width());
}
void SlideFrame::mousePressEvent(QMouseEvent *event){
    _isPressing = true;
    _startPoint = event->pos();
}
void SlideFrame::mouseMoveEvent(QMouseEvent *event){
    QPointF distance = event->pos()-_startPoint;
    double distanceY = distance.y();
    keepPose(_centralY+distanceY);
}
void SlideFrame::mouseReleaseEvent(QMouseEvent *){
    _isPressing = false;
    _centralY = _centralFrame->y();
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ItemButton@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
ItemButton::ItemButton(QWidget* parent):QPushButton(parent){
    setStyleSheet("QPushButton{color:#4B4B4B;background-color:#FFFFFF;font:75 15pt \"Ubuntu\";border-radius:5px;}QPushButton:checked{background-color:#919191;color:#FFFFFF;}");
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@CommonButton@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
CommonButton::CommonButton(QWidget* parent):QPushButton(parent){
    setFixedHeight(50);
    setStyleSheet("QPushButton{color:#4B4B4B;background-color:#FFFFFF;font:75 15pt \"Ubuntu\";border-radius:5px;}QPushButton:pressed{background-color:#919191;color:#FFFFFF;}");
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@CommonLabel@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
CommonLabel::CommonLabel(QWidget* parent):QLabel(parent){
    setFixedHeight(50);
    setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    setStyleSheet("color:#050505;background-color:transparent;font:75 15pt \"Ubuntu\";");
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@SingleButtonDialog@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
SingleButtonDialog::SingleButtonDialog(QWidget* parent):QDialog(parent){
    setWindowFlags(Qt::X11BypassWindowManagerHint|Qt::FramelessWindowHint);
    setFixedSize(530,280);
    _titleLabel=new QLabel(this);
    _contentLabel=new QLabel(this);
    _titleLabel->setStyleSheet("color:#050505;background-color:transparent;font:bold 20pt \"Ubuntu\";");
    _contentLabel->setStyleSheet("color:#050505;background-color:transparent;font:75 18pt \"Ubuntu\";");
    _titleLabel->setAlignment(Qt::AlignCenter);
    _contentLabel->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    _contentLabel->setWordWrap(true);
    _layout=new QVBoxLayout(this);
    _layout->setContentsMargins(40,40,40,90);
    _layout->setSpacing(10);
    _layout->addWidget(_titleLabel);
    _layout->addWidget(_contentLabel);
    _layout->setStretch(0,1);
    _layout->setStretch(1,3);
    setLayout(_layout);
}
void SingleButtonDialog::setTitle(QString title){
    _titleLabel->setText(title);
}
void SingleButtonDialog::setContent(QString content){
    _contentLabel->setText(content);
}
void SingleButtonDialog::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);
    QColor backgroundColor(FrontColor);
    backgroundColor.setAlpha(220);
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(),10,10);
    if(_isPress){
        QColor buttonbackColor(FontColor);
        buttonbackColor.setAlpha(50);
        painter.setBrush(buttonbackColor);
        painter.drawPath(_buttonPath);
    }
    QFont font;
    font.setPointSize(20);//字体点大小
    painter.setFont(font);
    painter.setPen(QColor(BlueColor));
    painter.drawText(_buttonRect,Qt::AlignCenter,"好的");
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(LineColor));
    painter.drawRect(_lineRect);
    painter.restore();
}
void SingleButtonDialog::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    _buttonRect=QRect(0,height()-90,width(),90);
    _lineRect=QRectF(0,height()-90,width(),1.5);
    QPainterPath path0;
    QPainterPath path1;
    path0.addRoundedRect(_buttonRect,10,10);
    path1.addRect(QRect(0,height()-90,width(),10));
    _buttonPath=path0+path1;
}
void SingleButtonDialog::mousePressEvent(QMouseEvent* event){
    if(_buttonRect.contains(event->pos())){
        _isPress=true;
        update();
    }
}
void SingleButtonDialog::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    _isPress=false;
    update();
    close();
}
