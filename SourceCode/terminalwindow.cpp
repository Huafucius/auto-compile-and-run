#include "terminalwindow.h"
#include"mainwindow.h"
#include<ui_mainwindow.h>

#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>
#include <QWidget>
#include <QMessageBox>

TerminalWindow::TerminalWindow(MainWindow *mainWindow, QObject *parent)
    : QObject(parent)
    , mainWindow(mainWindow)
{
    // 初始化process
    process = (QProcess*)new QProcess(this);
    // 链接process的槽
    connect(process, &QProcess::readyReadStandardOutput, this, &TerminalWindow::readOutput);
    connect(process, &QProcess::readyReadStandardError, this, &TerminalWindow::readError);
}

TerminalWindow::~TerminalWindow()
{
    delete process;
}

// 对外开放的路径设置接口
void TerminalWindow::setExePath(QString setPath){
    exePath = setPath;
}

// 启动终端用的
void TerminalWindow::startTerminal() {
    if (process->state() == QProcess::NotRunning) {
        process->start(exePath);  // 例如，在Windows上启动记事本
        if (!process->waitForStarted()) {
            qWarning() << "无法启动进程:" << process->errorString();
            QMessageBox::warning(mainWindow, "启动失败", "无法启动进程");
        } else {
            qDebug() << "进程已启动";
        }
    } else {
        qDebug() << "进程已经在运行中";
        QMessageBox::information(mainWindow, "进程正在运行", "进程已经在运行中");
    }
}

// 关闭终端用的
void TerminalWindow::endTerminal() {
    if (process->state() == QProcess::Running) {
        process->terminate();  // 先尝试优雅地终止进程
        if (!process->waitForFinished(3000)) {  // 等待3秒钟
            process->kill();  // 强制杀死进程
            process->waitForFinished();
        }
    } else {
        qDebug()<< "当前没有运行的进程，注意是进程主动结束还是内存泄漏！！！！！！";
    }
}

// 执行命令
void TerminalWindow::executeCommand(QString command)
{
    // mainWindow->ui->terminalOutputEdit->append(command);
    // 暂时先不把命令回显到输出框了喵
    process->write(command.toUtf8() + '\n');
    mainWindow->ui->terminalInputEdit->clear();
}

// 在process有输出时执行，读取输出
void TerminalWindow::readOutput() {
    QByteArray output = process->readAllStandardOutput();
    QString decodedOutput = QString::fromUtf8(output);
    mainWindow->ui->terminalOutputEdit->append(decodedOutput);
    mainWindow->ui->terminalOutputEdit->moveCursor(QTextCursor::End);
}

// 在process报错时执行，读取报错
void TerminalWindow::readError() {
    QByteArray error = process->readAllStandardError();
    QString decodedError = QString::fromUtf8(error);
    mainWindow->ui->terminalOutputEdit->append(decodedError);
    mainWindow->ui->terminalOutputEdit->moveCursor(QTextCursor::End);
}
