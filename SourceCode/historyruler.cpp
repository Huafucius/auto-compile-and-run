#include "historyruler.h"
#include "mainwindow.h"
#include"ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>

/**
 * @brief HistoryRuler::HistoryRuler
 * 构造函数，初始化路径并连接信号槽。
 * @param mainWindow 指向 MainWindow 实例的指针
 * @param parent 父对象
 * @param basicPath 基础路径
 */
HistoryRuler::HistoryRuler(MainWindow *mainWindow, const QString &basicPath, QObject *parent)
    : QObject(parent)
    , mainWindow_(mainWindow)
    , basicPath_(basicPath) {
    // 设置历史文件的根路径
    historyPath_ = QString("%1/%2").arg(basicPath_, "history");
}

/**
 * @brief HistoryRuler::updateHistory
 * 更新 QListWidget，显示历史文件夹中的子文件夹名称。
 */
void HistoryRuler::updateHistory() {
    // 判空
    QDir dir(historyPath_);
    if (!dir.exists()) {
        return;
    }

    // 获取子文件夹名称
    QStringList subfolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // 清空当前列表
    mainWindow_->ui->historyWidget->clear();

    // 添加子文件夹到列表
    for (const QString &folder : subfolders) {
        mainWindow_->ui->historyWidget->addItem(folder);
    }
}

/**
 * @brief HistoryRuler::onItemClicked
 * 处理 QListWidget 项点击事件，显示被点击的项的内容。
 * @param item 被点击的项
 * @return 可执行文件的路径
 */
void HistoryRuler::onItemClicked(QListWidgetItem *item) {
    // 获取点击项的文本
    QString folderDir = item->text();
    exePath_ = QString("%1/%2/%3/%4.exe").arg(historyPath_,folderDir, "Debug", folderDir);
    QString& exePathReturn = exePath_; // 起个别名

    // 更改可执行文件路径
    qDebug() << exePath_; // 展示用
    mainWindow_->changeExePahth(exePathReturn);

    // 重启terminalWindow（当然如果之前就没启动的话，就直接启动了）
    mainWindow_->ui->switchCheckBox->setChecked(false);
    mainWindow_->ui->switchCheckBox->setChecked(true);
}
