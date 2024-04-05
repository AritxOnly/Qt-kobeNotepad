#include "autosavedialog.h"
#include "ui_autosavedialog.h"

#include <QCheckBox>

AutosaveDialog::AutosaveDialog(QWidget *parent,
                               int autosaveTime,
                               bool autosaveState)
    : QDialog(parent)
    , ui(new Ui::AutosaveDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("自动保存设置");

    time = autosaveTime;
    isBoxChecked = autosaveState;

    ui->checkBox->setChecked(isBoxChecked);

    ui->timeSettings->setRange(1, 10);
    ui->timeSettings->setValue(time);
    ui->timeSettings->setSuffix(" 分钟");

    connect(ui->checkBox, &QCheckBox::stateChanged, this, [=]() {
        isBoxChecked = ui->checkBox->isChecked();
        qDebug() << isBoxChecked << time;
    });

    connect(ui->timeSettings, &QSpinBox::valueChanged, this, [=]() {
        time = ui->timeSettings->value();
        qDebug() << isBoxChecked << time;
    });
}

AutosaveDialog::~AutosaveDialog()
{
    delete ui;
}
