#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "compile.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , history_model(new QStringListModel(this))
{
    ui->setupUi(this);

    // 历史记录初始化
    ui->history_blog->setModel(history_model);
    ui->history_blog->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 默认路径
    build_dir = QDir::currentPath() + "/history";
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 选择文件
void MainWindow::on_choose_path_clicked()
{
    QString _file_path = QFileDialog::getExistingDirectory(this, "选择文件", "");
    if (!_file_path.isEmpty()) {
        file_path = _file_path;
        ui->file_path_show->setText(file_path);

        // 设置 sorce_dir
        sorce_dir = QFileInfo(file_path).absoluteFilePath();
    }
}

// 进行编译并回显
void MainWindow::on_choose_name_clicked() {

    // 卫语句，编译前必须选择源代码目录
    if (sorce_dir.isEmpty()) {
        QMessageBox::critical(this, "编译失败", "请先选择源代码目录！");
        return;
    }

    // 卫语句，检查编译名称
    exe_name = ui->name_edit->text();
    if (exe_name.isEmpty()) {
        QMessageBox::critical(this, "编译失败", "未指定编译文件的文件名！");
        return;
    }

    // 实例化编译器
    Compile *compiler = new Compile();

    // 编译函数
    std::string exe_path;
    exe_path = compiler->compile_project_interface(
        sorce_dir.toStdString(), build_dir.toStdString(), exe_name.toStdString());

    // 删除编译器
    delete compiler;

    // 更新历史记录
    history_update(exe_path);

    // 进行回显
    run_exe(QString::fromStdString(exe_path));
}

// 更新历史记录
void MainWindow::history_update(std::string &exe_path) {
    if (!exe_path.empty()) {

        // 提取文件名
        QString q_exe_path =
            QString::fromStdString(exe_path); // 将std::string转换为QString
        int lastSlashIndex = q_exe_path.lastIndexOf('/'); // 查找最后一个斜杠的位置
        QString file_name = q_exe_path.mid(lastSlashIndex + 1);

        // 检查历史记录中是否包含该文件名，如果不包含则添加
        if (!history_list.contains(file_name)) {
            history_list.append(file_name);
            history_model->setStringList(history_list); // 更新历史记录模型
        }

        // 更新标签，显示编译成功的信息和路径
        ui->file_path_show->setText("编译成功：" + q_exe_path);
    }

    else {
        // 编译失败
        QMessageBox::critical(this, "编译失败", "编译过程中发生错误！");
    }
}

// 进行回显
void MainWindow::run_exe(const QString& exe_path) {

    QProcess* process = new QProcess(this); // 创建进程对象

    // 连接信号槽以读取标准输出
    QObject::connect(process, &QProcess::readyReadStandardOutput, this, [this, process](){
        QByteArray output = process->readAllStandardOutput();
        QString outputText = QString::fromUtf8(output);
        qDebug() << "Standard Output:" << outputText;
        ui->run_result_show->setText(outputText);
    });

    // 连接信号槽以读取标准错误
    QObject::connect(process, &QProcess::readyReadStandardError, this, [this, process](){
        QByteArray errorOutput = process->readAllStandardError();
        QString errorText = QString::fromUtf8(errorOutput);
        qDebug() << "Error Output:" << errorText;
        if (!errorText.isEmpty()) {
            ui->run_result_show->append("Error:\n" + errorText);
        }
    });

    // 进程结束后清理资源
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                         Q_UNUSED(exitCode);
                         Q_UNUSED(exitStatus);
                         process->deleteLater(); // 删除进程对象
                     });

    process->start(exe_path); // 启动可执行文件
}

