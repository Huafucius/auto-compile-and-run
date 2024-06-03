#include <QApplication>
#include <QMainWindow>
#include "runbutton.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // 创建应用程序对象

    QMainWindow mainWindow; // 创建主窗口
    RunButtonWidget *runButtonWidget = new RunButtonWidget(&mainWindow); // 创建RunButtonWidget对象
    runButtonWidget->setExecutablePath("E:/Files/Code/model.exe"); // 设置可执行文件路径

    mainWindow.setCentralWidget(runButtonWidget); // 将RunButtonWidget设置为主窗口的中心部件
    mainWindow.setWindowTitle("Run Executable Example"); // 设置主窗口标题
    mainWindow.resize(400, 300); // 设置主窗口大小
    mainWindow.show(); // 显示主窗口

    return app.exec(); // 进入应用程序事件循环
}
