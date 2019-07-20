#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent, QString wallpapersFolder) :
    QDialog(parent),
    ui(new Ui::DialogSettings),
    wallpapersFolder(wallpapersFolder)
{
    ui->setupUi(this);

    initGui();

    connect(ui->lineEdit_folder, SIGNAL(textEdited(QString)), this, SLOT(wallpaperFolderChanged()));
    connect(ui->pushButton_chooseFolder, SIGNAL(clicked()), this, SLOT(onChangeFolderButtonClicked()));

    connect(ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->pushButton_apply, SIGNAL(clicked()), this, SLOT(apply()));
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

void DialogSettings::wallpaperFolderChanged()
{
    QDir dir(ui->lineEdit_folder->text());
    if (!dir.exists()) {
        ui->lineEdit_folder->setStyleSheet("color: red");
    } else {
        ui->lineEdit_folder->setStyleSheet("color: black");
    }
    ui->lineEdit_folder->setText(ui->lineEdit_folder->text().replace('\\', '/'));
}

void DialogSettings::onChangeFolderButtonClicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Choose Directory");
    ui->lineEdit_folder->setText(path);
}

void DialogSettings::cancel()
{
    QDialog::close();
}

void DialogSettings::apply()
{
    if (!QDir(ui->lineEdit_folder->text()).exists()) {
        QMessageBox q;
        q.setWindowTitle("Warning");
        q.setText("The directory you entered does not exist\n"
                  "Do you want to create it?");
        q.setStandardButtons(QMessageBox::Yes);
        q.addButton(QMessageBox::No);
        q.setIcon(QMessageBox::Warning);
        int reply = q.exec();
        if (reply == QMessageBox::Yes) {
            wallpapersFolder = ui->lineEdit_folder->text();
            QDir().mkpath(wallpapersFolder);
        }
    } else {
        wallpapersFolder = ui->lineEdit_folder->text();
    }
    QDialog::close();
}

DialogSettings::~DialogSettings()
{
    delete ui;
}
