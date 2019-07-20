#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    settings = new QSettings(settingsFile, QSettings::IniFormat);

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
    connect(ui->listWidget_wallpapers, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(chooseWallpapers(QListWidgetItem*)));
}

void MainWindow::initGui()
{
    resize(1024, 576);

    ui->pushButton_settings->setIcon(QIcon(":/icons/settings.png"));
    ui->pushButton_settings->setIconSize(QSize(30, 30));

    ui->pushButton_settings->installEventFilter(this);

    initWallpapersList();
}

void MainWindow::initWallpapersList()
{
    QDirIterator it(wallpapersFolder, QStringList() << "*.mp4", QDir::Files, QDirIterator::Subdirectories);
    int cnt = 0, row = -1;
    while (it.hasNext()) {
        QString next = it.next(), newNext = transformWallpaperFileName(next);
        wallpapersList[newNext] = next;
        ui->listWidget_wallpapers->addItem(newNext);
        if (next == wallpaperFileName) {
            row = cnt;
        }
        cnt++;
    }
    ui->listWidget_wallpapers->setCurrentRow(row);
}

void MainWindow::loadSettings()
{
    wallpapersFolder = settings->value("wallpapersFolder").toString();
    wallpaperFileName = settings->value("wallpapersFileName").toString();
    qDebug() << wallpapersFolder << wallpaperFileName;
    if (wallpapersFolder == "" || !QDir(wallpapersFolder).exists()) {
        settings->setValue("wallpapersFolder",
                          QString(QString(getenv("SYSTEMDRIVE")) + QString(getenv("HOMEPATH"))).replace('\\', '/')
                          + "/CORNpaper/Wallpapers");
        settings->sync();
        loadSettings();
        if (!QDir(wallpapersFolder).exists()) {
            QDir().mkpath(wallpapersFolder);
        }
    }
}

void MainWindow::saveWallpapersFolder()
{
    settings->setValue("wallpapersFolder", wallpapersFolder);
}

void MainWindow::saveWallpaper(QString fileName)
{
    settings->setValue("wallpapersFileName", fileName);
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
    wallpapersFolder = dialogSettings->wallpapersFolder;
    saveWallpapersFolder();
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

QString MainWindow::transformWallpaperFileName(QString fileName)
{
    QString ans = "";
    int id = fileName.lastIndexOf("/");
    for (int i = id + 1; i < fileName.size(); i++) {
        ans += fileName[i] == '-' ? QChar(' ') : fileName[i];
    }
    return ans.left(ans.lastIndexOf('.'));
}

void MainWindow::chooseWallpapers(QListWidgetItem *item)
{
    saveWallpaper(wallpapersList[item->text()]);
}

MainWindow::~MainWindow()
{
    delete ui;
}
