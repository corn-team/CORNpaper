#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent, QString wallpapersFolder) :
    QDialog(parent),
    ui(new Ui::DialogSettings),
    wallpapersFolder(wallpapersFolder)
{
    ui->setupUi(this);

    initGui();
}

void DialogSettings::initGui()
{
    setFixedSize(400, 100);

    ui->pushButton_chooseFolder->setIcon(QIcon(":/icons/folder.png"));
    ui->pushButton_chooseFolder->setIconSize(QSize(18, 18));

    ui->pushButton_chooseFolder->installEventFilter(this);

    ui->lineEdit_folder->setText(wallpapersFolder);
}

bool DialogSettings::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->pushButton_chooseFolder) {
        if (e->type() == QEvent::HoverEnter) {
            ui->pushButton_chooseFolder->setIcon(QIcon(":/icons/folder_hover.png"));
        } else if (e->type() == QEvent::HoverLeave) {
            ui->pushButton_chooseFolder->setIcon(QIcon(":/icons/folder.png"));
        }
    }
}

DialogSettings::~DialogSettings()
{
    delete ui;
}
