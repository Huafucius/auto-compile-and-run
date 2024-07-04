 #ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QObject>
#include <QProcess>

class MainWindow;

class TerminalWindow : public QObject
{
    Q_OBJECT

public:
    TerminalWindow(MainWindow *mainWindow, QObject *parent = nullptr);
    ~TerminalWindow();

    // 设置可执行文件路径
    void setExePath(QString setPath);
    // 启动终端
    void startTerminal();
    // 关闭终端
    void endTerminal();
    // 执行命令
    void executeCommand(QString command);

private:
    // 读取输出流
    void readOutput();
    // 读取报错流
    void readError();

    QProcess *process;
    QString exePath;
    MainWindow *mainWindow;
};

#endif // TERMINALWINDOW_H
