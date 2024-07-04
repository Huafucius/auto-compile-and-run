#ifndef HISTORYRULER_H
#define HISTORYRULER_H

#include <QObject>
#include <QString>
#include <QListWidgetItem>

class MainWindow;

/**
 * @brief The HistoryRuler class
 * 该类用于管理和维护历史记录，在指定目录下显示子文件夹的名称，并处理用户点击事件。
 */
class HistoryRuler : public QObject {
    Q_OBJECT

public:
    HistoryRuler(){};

    /**
     * @brief HistoryRuler 构造函数
     * @param mainWindow 指向 MainWindow 实例的指针
     * @param parent 父对象
     * @param basicPath 基础路径，其中包含历史文件夹
     */
    HistoryRuler(MainWindow *mainWindow, const QString &basicPath, QObject *parent = nullptr);

    /**
     * @brief 更新历史记录
     * 在 QListWidget 中显示指定目录下的子文件夹名称。
     */
    void updateHistory();

    /**
     * @brief 处理项点击事件
     * @param item 被点击的项
     */
    void onItemClicked(QListWidgetItem *item);

private:
    QString basicPath_;  ///< 基础路径
    QString historyPath_; ///< 历史文件夹路径
    QString exePath_; ///< 目前可执行文件的路径
    MainWindow *mainWindow_; ///< 指向 MainWindow 实例的指针
};

#endif // HISTORYRULER_H
