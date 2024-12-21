#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // 创建 QApplication 对象
    MainWindow w;                // 创建 MainWindow 对象
    w.show();                    // 显示主窗口
    return a.exec();             // 启动事件循环
}
