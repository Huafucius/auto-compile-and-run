#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"compiler.h"
#include"terminalwindow.h"
#include"historyruler.h"

#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// 主窗口
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    // 更新exe路径的接口
    void changeExePahth(const QString &newExePath);

private slots:
    // 目标文件夹路径选择按钮
    void on_searchFilePathButton_clicked();

    // 编译按钮，开始进行编译
    void on_compileButton_clicked();

    // 运行指令按钮，开始运行指令
    void on_terminalRunButton_clicked();

    // 程序运行开关切换，开启代表着创建一个终端
    void on_switchCheckBox_toggled(bool checked);

    // 历史记录点击按钮，任务交给historyRuler
    void on_historyWidget_itemClicked(QListWidgetItem *item);

private:
    // 基本路径
    QString windowName = "源码的自动编译与运行";
    QString baseDir; // 根工作路径
    QString cmakeDir; // cmake的路径
    QString targetFileDir; // 等待编译的目标文件夹路径
    QString exePath; // 目前可执行文件的路径，编译的时候会更改，点击历史记录也会更改

    // 三大模块
    Compiler compiler; // 编译模块
    TerminalWindow* terminal; // 终端运行模块
    HistoryRuler* historyRuler; // 历史记录维护模块
};
#endif // MAINWINDOW_H
