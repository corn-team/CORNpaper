#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pro = new QProcess(this);

    settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    settings = new QSettings(settingsFile, QSettings::IniFormat);

    loadSettings();
    initGui();

    //Setting application font
    int id = QFontDatabase::addApplicationFont(":/fonts/roboto-condensed.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont defaultFont(family);
    QApplication::setFont(defaultFont);

    ui->horizontalLayout_tabs->setAlignment(Qt::AlignLeft);

    //Connecting SIGNALs and SLOTs
    connect(ui->pushButton_your, SIGNAL(clicked()), this, SLOT(onYourButtonClicked()));
    connect(ui->pushButton_new, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));
    connect(ui->pushButton_settings, SIGNAL(clicked()), this, SLOT(showDialogSettings()));
    connect(ui->listWidget_wallpapers, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(changeWallpapers(QListWidgetItem*)));
}

void MainWindow::initGui()
{
    resize(1024, 576);

    ui->pushButton_settings->setIcon(QIcon(":/icons/settings.png"));
    ui->pushButton_settings->setIconSize(QSize(30, 30));

    ui->pushButton_reload->setIcon(QIcon(":/icons/reload.png"));
    ui->pushButton_reload->setIconSize(QSize(28, 28));

    initWallpapersList();
}

void MainWindow::initWallpapersList()
{
    ui->listWidget_wallpapers->clear();
    wallpapersList.clear();
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
    if (!QFile(wallpaperFileName).exists()) {
        settings->setValue("wallpapersFileName", "");
        wallpaperFileName = "";
    }
}

void MainWindow::saveWallpapersFolder()
{
    settings->setValue("wallpapersFolder", wallpapersFolder);
}

void MainWindow::saveWallpaper(QString fileName)
{
    settings->setValue("wallpapersFileName", fileName);
    wallpaperFileName = settings->value("wallpapersFileName").toString();
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
    initWallpapersList();
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

void MainWindow::changeWallpapers(QListWidgetItem *item)
{
    saveWallpaper(wallpapersList[item->text()]);
    QFile file("weebp/CORNpaper.bat");
    qDebug() << QFileInfo(file).absolutePath();
    file.open(QIODevice::WriteOnly | QFile::Text);
    QString batch = "taskkill /F /IM mpv.exe\n"
                    "(wp id)>>x.txt\n"
                    "set /p var=<x.txt\n"
                    "del x.txt\n"
                    "wp run mpv --wid=%var% " + wallpaperFileName + " --loop=inf "
                    "--player-operation-mode=pseudo-gui "
                    "--force-window=yes --no-audio";
    qDebug() << batch;
    QTextStream out(&file);
    out << batch;
    file.close();

    pro->start("cmd.exe /c cd weebp&CORNpaper.bat");
    pro->waitForFinished();
}

MainWindow::~MainWindow()
{
    delete ui;
}
