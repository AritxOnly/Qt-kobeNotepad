#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QAction>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QLabel>
#include <QFontDialog>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
    *   实现字数检测的基础：设置一个键盘输入事件，当textedit改变，通过text.length获得整形数字
    */

    //状态栏设定
    QLabel *cntCh = new QLabel(this);
    QLabel *rowClm = new QLabel(this);
    ui->statusbar->addPermanentWidget(cntCh);
    ui->statusbar->addWidget(rowClm);
    ui->statusbar->setSizeGripEnabled(false);   //不要小点！

    connect(ui->textEdit, &QTextEdit::textChanged, this, [=]() {
        QString content = ui->textEdit->toPlainText();
        int len = content.length();

        //输出行列统计
        //优化性能？
        int row, clm;
        row = countRow(content, clm);
        rowClm->setText(QString::number(row)
                            .append("  行  ")
                            .append(QString::number(clm))
                            .append("  列"));

        //输出字符统计
        QString tmp = "共";
        cntCh->setText(tmp.append(QString::number(len).append("个字符    ")));
    });


    //自动保存
    // QTimer *saveTimer = new QTimer(this);
    // saveTimer->setInterval(300);
    // saveTimer->start();
    // connect(saveTimer, &QTimer::timeout, this, &MainWindow::fileSave);

    //将Text edit设置为central
    setCentralWidget(ui->textEdit);

    //图标设置
    ui->actionNew->setIcon(QIcon(":/img/new.png"));
    ui->actionOpen_File->setIcon(QIcon(":/img/open.png"));
    ui->actionSave->setIcon(QIcon(":/img/save.png"));
    ui->actionAbout->setIcon(QIcon(":/img/about.png"));

    //快捷键设置
    ui->actionSave->setShortcut(tr("Ctrl+S"));

    //Debug输出
    connect(ui->actionDebug_Ouput, &QAction::triggered, this, [=]() {
        txtInEdit = ui->textEdit->toPlainText();
        qDebug() << txtInEdit;
    });

    //收集路径
    connect(ui->actionOpen_File, &QAction::triggered, this, [=]() {
        fileDir = QFileDialog::getOpenFileName(this,
                                               "打开文件",
                                               fileDir,
                                               tr("文本文档(*.txt)"));
        //导入输入框
        QFile file(fileDir);
        file.open(QIODevice::ReadOnly);
        QByteArray content = file.readAll();    //接收信息
        ui->textEdit->setText(content);
        file.close();
        isFileSet = true;   //标记文件路径存储成功
    });

    //Save功能
    connect(ui->actionSave, &QAction::triggered, this, [=]() {
        if(isFileSet)
        {
            fileSave();
        } else {
            fileDir = QFileDialog::getSaveFileName(this,
                                                   "保存文件",
                                                   fileDir,
                                                   tr("文本文档(*.txt)"));
            fileSave();
        }
    });

    //New功能
    connect(ui->actionNew, &QAction::triggered, this, [=]() {
        if(!isFileSave)
        {
            if(isFileSet)
            {
                if(msgSave())
                    fileSave();
            } else {
                QString content = ui->textEdit->toPlainText();
                int len = content.length(); //通过输入框中是否有文本判断
                if(len != 0 && msgSave())  //编辑框中有文本
                {
                    fileDir = QFileDialog::getSaveFileName(this,
                                                           "保存文件",
                                                           fileDir,
                                                           tr("文本文档(*.txt)"));
                    fileSave();
                }
            }
        }
        fileDir = "";
        isFileSet = false;  //重载标记
        isFileSave = false; //重载标记
        ui->textEdit->setText("");  //输入框清零
    });

    //字体选择
    connect(ui->actionFont, &QAction::triggered, this, [=]() {
        bool isSelected = false;
        QFont font = QFontDialog::getFont(&isSelected, this);
        ui->textEdit->setFont(font);
    });

    //设置界面
    connect(ui->actionSettings, &QAction::triggered, this, [=]() {
        SettingsDialog settings(this);
        settings.exec();
    });

    //About
    connect(ui->actionAbout, &QAction::triggered, this, [=]() {
        QMessageBox about(this);
        about.setText("版本：v1.0-alpha3\n开发者：Aritx Zhou\n功能仍在开发中...");
        about.setWindowTitle("关于");
        about.exec();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString content = ui->textEdit->toPlainText();

    // qDebug() << content.length() << content.isEmpty();

    if(content.isEmpty() && !isFileSet)
    {
        event->accept();
        // qDebug() << "Close Event Accepted";
        return;
    }

    if(!isFileSave)
    {
        QMessageBox saveMessage;
        saveMessage.setWindowTitle("警告");
        saveMessage.setText("更改尚未保存 \n要保存后再退出吗？");
        saveMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        saveMessage.setDefaultButton(QMessageBox::Yes);
        int isYesPressed = saveMessage.exec();

        if(isYesPressed == QMessageBox::Yes)
        {
            if(isFileSet)
                fileSave();
            else {
                fileDir = QFileDialog::getSaveFileName(this,
                                                        "保存文件",
                                                        fileDir,
                                                        tr("文本文档(*.txt)"));
                fileSave();
            }
            event->accept();
        } else if(isYesPressed == QMessageBox::No) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void MainWindow::fileSave()
{
    //打开文件
    QFile file(fileDir);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QByteArray content = ui->textEdit->toPlainText().toUtf8();
    //toUTF8,将QString->QByteArray, 可以输出中文
    file.write(content);
    file.close();
    isFileSave = true;
}

bool MainWindow::msgSave()
{
    QMessageBox::StandardButton msgBox;
    msgBox = QMessageBox::question(this, "提示",
                                   "更改未保存，是否保存？",
                                   QMessageBox::Yes | QMessageBox::No);
    if(msgBox == QMessageBox::Yes)
        return true;
    else
        return false;
}

int MainWindow::countRow(const QString content, int& column)
{
    //用最简单的遍历方式去数
    int i = 0, cnt = 0, maxColumn = 0, clm = 0;
    while(content[i] != '\x0')
    {
        if(content[i] == '\n')
        {
            cnt++;
            if(maxColumn < clm)
                maxColumn = clm;
            clm = 0;
        }
        clm++;
        i++;
    }
    column = maxColumn;
    return cnt + 1;
}
