#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    //Custom functions
    bool msgSave();
    void setTimer();
    int countRow(const QString, int&);

    //variables
    bool isFileSet = false; //文件路径设置标识符
    bool isFileSave = false;    //文件是否存储标识符
    bool isRunning = false;

    QTimer *timer = nullptr;  //定时器指针

public slots:
    void fileSave();

private:
    Ui::MainWindow *ui;
    QString txtInEdit;
    std::string txtInEditStd;
    QString fileDir;
    // QStringList fileDirList;
};
#endif // MAINWINDOW_H
