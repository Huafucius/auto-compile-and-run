#include <QApplication>
#include <QMainWindow>
#include "runbutton.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    RunButtonWidget *runButtonWidget = new RunButtonWidget(&mainWindow);
    runButtonWidget->setExecutablePath("E:/Files/Code/model.exe"); // 替换为exe文件路径 需使用/ 不能使用\

    mainWindow.setCentralWidget(runButtonWidget);
    mainWindow.setWindowTitle("Run Executable Example");
    mainWindow.resize(400, 300);
    mainWindow.show();

    return app.exec();
}
