#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_choose_path_clicked();

    void on_choose_name_clicked();

private:
    Ui::MainWindow *ui;

    QString sorce_dir;
    QString build_dir;

    QString file_path;
    QString exe_name;

    QStringList history_list;
    QStringListModel *history_model;

    void history_update(std::string &exe_path);
    void run_exe(const QString &exe_path);
};
#endif // MAINWINDOW_H
