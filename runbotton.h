#ifndef RUNBUTTON_H
#define RUNBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QProcess>

class RunButtonWidget : public QWidget {
    Q_OBJECT

public:
    explicit RunButtonWidget(QWidget *parent = nullptr); // 构造函数
    void setExecutablePath(const QString &path); // 设置可执行文件路径

private slots:
    void runExecutable(); // 槽函数，运行可执行文件

private:
    QPushButton *button; // 按钮
    QTextEdit *outputArea; // 文本编辑区域，用于显示输出
    QString executablePath; // 可执行文件路径
};

#endif // RUNBUTTON_H
