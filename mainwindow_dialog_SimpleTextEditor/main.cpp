#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("牢大记事本");
    w.setWindowIcon(QIcon(":/img/kobe.png"));
    w.show();
    return a.exec();
}
