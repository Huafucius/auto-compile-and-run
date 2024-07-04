#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include"terminalwindow.h"
#include"historyruler.h"

#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口的基本样式
    setWindowTitle(windowName);
    setStyleSheet("QWidget { background-color : beige;}");

    // 初始化路径
    baseDir = QDir::currentPath();
    cmakeDir = "";
    targetFileDir = "";

    // 初始化编译模块的提示
    ui->targetFileNameEdit->setPlaceholderText("编译后exe文件名称（可修改）");

    // 初始化运行terminal及其区块
    ui->terminalInputEdit->setPlaceholderText("请在此键入命令，并点击运行");
    ui->terminalOutputEdit->setReadOnly(true);
    // ui->terminalOutputEdit->overwriteMode(); // byd腹泻模式没用啊，已经在别的地方设置了手动清除回显
    terminal = nullptr;

    // 初始化历史记录
    historyRuler = (HistoryRuler*)new HistoryRuler(this, baseDir);
    historyRuler->updateHistory();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete historyRuler;
}

void MainWindow::on_searchFilePathButton_clicked()
{
    // 打开文件夹选择对话框，并获取选中的文件夹路径
    targetFileDir = QFileDialog::getExistingDirectory(this, "选择文件夹", "/home");

    // 卫语句：如果没有选择文件夹，显示提示信息
    if (targetFileDir.isEmpty()){
        QMessageBox::warning(this, "警告", "未选择文件夹");
        return;
    }

    // 显示文件夹路径
    ui->sourceFilePathLable->setText(targetFileDir);

    // 获取文件夹名称，并显示在编译名称中
    QFileInfo folderInfo(targetFileDir);
    ui->targetFileNameEdit->setText(folderInfo.fileName());
}

// 槽函数：用户点击了开始编译
void MainWindow::on_compileButton_clicked()
{
    exePath = compiler.compileInterface(targetFileDir, baseDir, ui->targetFileNameEdit->text());

    if(exePath == ""){
        QMessageBox::warning(this, "警告", "编译失败，请检查路径或代码！");;
    }
    else{
        QMessageBox::warning(this, "注意", "编译成功");
    }
    historyRuler->updateHistory(); // 更新历史记录
}

// 槽函数：用户运行了命令！
void MainWindow::on_terminalRunButton_clicked() {
    // 卫语句
    if (terminal == nullptr) {
        QMessageBox::warning(this, "注意", "程序运行开关未开启");
        return;
    }

    QString command = ui->terminalInputEdit->text();
    if (command.isEmpty()) {
        QMessageBox::warning(this, "注意", "未输入指令");
        return;
    }

    terminal->executeCommand(command);
    historyRuler->updateHistory(); // 更新历史记录
}


// 槽函数：用户切换了终端的状态！
void MainWindow::on_switchCheckBox_toggled(bool checked)
{

    // 开关开启则启动
    if(checked == true){

        if(exePath == ""){
            QMessageBox::warning(this,"注意","未指定编译程序");
            ui->switchCheckBox->setChecked(false);
            return;
        }

        // 没有terminal怎么办？new一个。
        terminal = new TerminalWindow(this);
        terminal->setExePath(exePath);
        terminal->startTerminal();
    }
    // 关闭就删除，顺便清空回显

    else{
        terminal->endTerminal();
        delete terminal;
        terminal = nullptr;

        // 清除回显
        ui->terminalOutputEdit->clear();
    }
    historyRuler->updateHistory(); // 更新历史记录
}

// 槽函数：用户点击了历史记录！
void MainWindow::on_historyWidget_itemClicked(QListWidgetItem *item)
{
    historyRuler->onItemClicked(item);
    historyRuler->updateHistory(); // 更新历史记录
}

// exe路径接口
void MainWindow::changeExePahth(const QString& newExePath){
    exePath = newExePath;
}
