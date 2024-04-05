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
    int countRow(const QString, int&);
    void closeEvent(QCloseEvent* event);

    //variables
    bool isFileSet = false; //文件路径设置标识符
    bool isFileSave = false;    //文件是否存储标识符
    bool isAutosaveSet = false;
    bool isFirstTimeOpen = false;   //存储是否刚打开
    int autosaveTime = 2;

    QTimer *saveTimer = nullptr;

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
