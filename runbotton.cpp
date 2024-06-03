#include "runbutton.h"

RunButtonWidget::RunButtonWidget(QWidget *parent)
    : QWidget(parent), executablePath("") {
    button = new QPushButton("Run Executable", this); // 创建按钮并设置文本
    outputArea = new QTextEdit(this); // 创建文本编辑区域
    outputArea->setReadOnly(true); // 设置文本编辑区域为只读

    QVBoxLayout *layout = new QVBoxLayout(this); // 创建垂直布局
    layout->addWidget(button); // 将按钮添加到布局
    layout->addWidget(outputArea); // 将文本编辑区域添加到布局

    connect(button, &QPushButton::clicked, this, &RunButtonWidget::runExecutable); // 连接按钮点击信号和槽函数
}

void RunButtonWidget::setExecutablePath(const QString &path) {
    executablePath = path; // 设置可执行文件路径
}

void RunButtonWidget::runExecutable() {
    if (executablePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Executable path is not set."); // 显示警告信息
        return;
    }

    QProcess process; // 创建进程对象
    process.start(executablePath); // 启动可执行文件

    if (!process.waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start executable."); // 显示错误信息
        return;
    }

    if (!process.waitForFinished()) {
        QMessageBox::critical(this, "Error", "Failed to execute executable."); // 显示错误信息
        return;
    }

    QString output = process.readAllStandardOutput(); // 读取标准输出
    QString errorOutput = process.readAllStandardError(); // 读取标准错误输出

    if (!errorOutput.isEmpty()) {
        outputArea->append("Error:\n" + errorOutput); // 如果有错误输出，显示错误信息
    }

    outputArea->append("Output:\n" + output); // 显示标准输出
}
