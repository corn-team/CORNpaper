#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settingsFile = QApplication::applicationDirPath() + "/settings.ini";

    loadSettings();
    initGui();

    //Setting application font
    int id = QFontDatabase::addApplicationFont(":/fonts/roboto-condensed.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont defaultFont(family);
    setFont(defaultFont);

    ui->horizontalLayout_tabs->setAlignment(Qt::AlignLeft);

    //Connecting SIGNALs and SLOTs
    connect(ui->pushButton_your, SIGNAL(clicked()), this, SLOT(onYourButtonClicked()));
    connect(ui->pushButton_new, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));
    connect(ui->pushButton_settings, SIGNAL(clicked()), this, SLOT(showDialogSettings()));
}

void MainWindow::initGui()
{
    resize(1024, 576);

    ui->pushButton_settings->setIcon(QIcon(":/icons/settings.png"));
    ui->pushButton_settings->setIconSize(QSize(30, 30));

    ui->pushButton_settings->installEventFilter(this);
}

void MainWindow::loadSettings()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    qDebug() << settingsFile;
    wallpapersFolder = settings.value("wallpapersFolder").toString();
    if (wallpapersFolder == "") {
        settings.setValue("wallpapersFolder", QString(getenv("SYSTEMDRIVE")) + QString(getenv("HOMEPATH")) + "\\CORNpaper\\Wallpapers");
        settings.sync();
        loadSettings();
    }
}

void MainWindow::saveWallpapersFolder()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.setValue("wallpapersFolder", wallpapersFolder);
}

void MainWindow::saveWallpaper()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
}

void MainWindow::onYourButtonClicked() {
    ui->pushButton_your->setChecked(true);
    ui->pushButton_new->setChecked(false);
}

void MainWindow::onNewButtonClicked() {
    ui->pushButton_new->setChecked(true);
    ui->pushButton_your->setChecked(false);
}

void MainWindow::showDialogSettings()
{
    dialogSettings = new DialogSettings(this, wallpapersFolder);
    dialogSettings->exec();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->pushButton_settings) {
        if (e->type() == QEvent::HoverEnter) {
            ui->pushButton_settings->setIcon(QIcon(":/icons/settings_hover.png"));
        } else if (e->type() == QEvent::HoverLeave) {
            ui->pushButton_settings->setIcon(QIcon(":/icons/settings.png"));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
