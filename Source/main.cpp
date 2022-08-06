#include "c_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1080,800);
    w.init();
    w.show();
    return a.exec();
}
