#ifndef AUTOSAVEDIALOG_H
#define AUTOSAVEDIALOG_H

#include <QDialog>

namespace Ui {
class AutosaveDialog;
}

class AutosaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutosaveDialog(QWidget *parent = nullptr,
                            int autosaveTime = 2,
                            bool autosaveState = false);
    ~AutosaveDialog();

    bool isBoxChecked;
    int time;

private:
    Ui::AutosaveDialog *ui;
};

#endif // AUTOSAVEDIALOG_H
