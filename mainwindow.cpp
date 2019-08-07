#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CORNpaper");

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
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showDialogSettings()));

    connect(ui->listWidget_wallpapers, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(changeWallpaper(QListWidgetItem*)));
    connect(ui->actionSet, SIGNAL(triggered()), this, SLOT(changeWallpaper()));

    connect(ui->pushButton_reload, SIGNAL(clicked()), this, SLOT(initWallpapersList()));

    connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(closeWallpaper()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeWallpaper()));

    connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(addWallpaper()));
    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(addWallpaper()));

    connect(ui->pushButton_remove, SIGNAL(clicked()), this, SLOT(removeWallpaper()));
    connect(ui->actionRemove, SIGNAL(triggered()), this, SLOT(removeWallpaper()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutMessage()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(showHelpMessage()));
}

void MainWindow::initGui()
{
    resize(1024, 576);

    ui->pushButton_settings->setIcon(QIcon(":/icons/settings.png"));
    ui->pushButton_settings->setIconSize(QSize(30, 30));
    ui->pushButton_settings->setToolTip("Settings");

    ui->pushButton_reload->setIcon(QIcon(":/icons/reload.png"));
    ui->pushButton_reload->setIconSize(QSize(28, 28));
    ui->pushButton_reload->setToolTip("Reload your wallpaper list");

    ui->pushButton_close->setIcon(QIcon(":/icons/close.png"));
    ui->pushButton_close->setIconSize(QSize(26, 26));
    ui->pushButton_close->setToolTip("Close current wallpaper");

    ui->pushButton_remove->setIcon(QIcon(":/icons/remove.png"));
    ui->pushButton_remove->setIconSize(QSize(30, 30));
    ui->pushButton_remove->setToolTip("Remove selected wallpaper from your wllpaper list");

    ui->pushButton_add->setIcon(QIcon(":/icons/add.png"));
    ui->pushButton_add->setIconSize(QSize(30, 30));
    ui->pushButton_add->setToolTip("Download new wallpaper");

    initWallpapersList();
}

void MainWindow::initWallpapersList()
{
    ui->listWidget_wallpapers->clear();
    wallpapersList.clear();
    QDirIterator it(wallpapersFolder, QStringList() << "*.mp4" << "*.avi" << "*.flv" << "*.wmv" << "*.mov" << "*.mkv",
                    QDir::Files, QDirIterator::Subdirectories);
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
    if (row == -1) settings->setValue("wallpaperFileName", "");
    ui->listWidget_wallpapers->setCurrentRow(row);
}

void MainWindow::loadSettings()
{
    wallpapersFolder = settings->value("wallpapersFolder").toString();
    wallpaperFileName = settings->value("wallpaperFileName").toString();
    panoramic = settings->value("panoramic").toBool();
    if (wallpapersFolder == "" || !QDir(wallpapersFolder).exists()) {
        settings->setValue("panoramic", false);
        panoramic = false;
        settings->setValue("wallpapersFolder",
                          QString(strcat(getenv("SYSTEMDRIVE"), getenv("HOMEPATH"))).replace('\\', '/')
                          + "/CORNpaper/Wallpapers/");
        settings->sync();
        wallpapersFolder = settings->value("wallpapersFolder").toString();
        if (!QDir(wallpapersFolder).exists()) {
            QDir().mkpath(wallpapersFolder);
        }
    }
    if (!QFile(wallpaperFileName).exists()) {
        settings->setValue("wallpaperFileName", "");
        wallpaperFileName = "";
    }
}

void MainWindow::saveSettings()
{
    settings->setValue("wallpapersFolder", wallpapersFolder);
    settings->setValue("panoramic", panoramic);
}

void MainWindow::saveWallpaper(QString fileName)
{
    settings->setValue("wallpaperFileName", fileName);
    wallpaperFileName = settings->value("wallpaperFileName").toString();
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
    dialogSettings = new DialogSettings(this, wallpapersFolder, panoramic);
    dialogSettings->exec();
    wallpapersFolder = dialogSettings->wallpapersFolder;
    panoramic = dialogSettings->panoramic;
    saveSettings();
    initWallpapersList();
}

QString MainWindow::transformWallpaperFileName(QString fileName)
{
    QString ans = QFileInfo(fileName).fileName().replace('-', ' ');
    return ans.left(ans.lastIndexOf('.'));
}

void MainWindow::changeWallpaper()
{
    int current_row = ui->listWidget_wallpapers->currentRow();
    if (current_row < 0) {
        QMessageBox::warning(this, "Warning", "No wallpaper selected");
        return;
    }
    QListWidgetItem *item = ui->listWidget_wallpapers->item(current_row);
    changeWallpaper(item);
}

void MainWindow::changeWallpaper(QListWidgetItem *item)
{
    saveWallpaper(wallpapersList[item->text()]);
    QFile file("weebp/CORNpaper.bat");
    qDebug() << QFileInfo(file).absolutePath();
    file.open(QIODevice::WriteOnly | QFile::Text);
//    QString batch = "taskkill /F /IM mpv.exe\n"
//                    "(wp id)>>x.txt\n"
//                    "set /p var=<x.txt\n"
//                    "del x.txt\n"
//                    "wp run mpv --wid=%var% " + wallpaperFileName + " --loop=inf "
//                    "--player-operation-mode=pseudo-gui "
//                    "--force-window=yes --no-audio";
    QString batch = "taskkill /F /IM mpv.exe\n"
                    "(wp id)>>x.txt\n"
                    "set /p var=<x.txt\n"
                    "del x.txt\n"
                    "wp run mpv --terminal=no --loop=inf " + wallpaperFileName + " --no-audio\n"
                    "wp add --wait " + (panoramic ? "--panoramic" : "--fullscreen") + " --class mpv";
    qDebug() << batch;
    QTextStream out(&file);
    out << batch;
    file.close();

    pro->start("cmd.exe /c cd weebp&CORNpaper.bat");
    pro->waitForFinished();

    QFile::link("weebp/CORNpaper.bat", QString(strcat(getenv("SYSTEMDRIVE"), getenv("HOMEPATH"))).replace('\\', '/') +
                "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/CORNpaper.lnk");
}

void MainWindow::closeWallpaper()
{
    pro->start("cmd.exe /c taskkill /F /IM mpv.exe");
    pro->waitForFinished();

    settings->setValue("wallpaperFileName", "");

    QFile file(QString(strcat(getenv("SYSTEMDRIVE"), getenv("HOMEPATH"))).replace('\\', '/') +
               "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/CORNpaper.lnk");
    file.remove();

    ui->listWidget_wallpapers->setCurrentRow(-1);
}

void MainWindow::removeWallpaper()
{
    QListWidgetItem *item = ui->listWidget_wallpapers->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Warning", "No wallpaper selected!");
    } else {
        QFile file(wallpapersList[item->text()]);
        file.remove();
        closeWallpaper();
        initWallpapersList();
    }
}

void MainWindow::addWallpaper()
{
    QString filter = "Videos (*.mp4 *.avi *.flv *.wmv *.mov *.mkv)";
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Choose wallpaper",
                                                    wallpapersFolder.left(3), filter, &filter);

    for (auto fileName : fileNames)
      QFile::copy(fileName, QString(wallpapersFolder + "/" + QFileInfo(fileName).fileName()).replace("//", "/"));

    initWallpapersList();
}

void MainWindow::showAboutMessage()
{
    QString ref = "https://github.com/corn-team";
    QString text = "CORNpapper<br>"
                   "alfa-version<br>"
                   "created by CORNteam<br>"
                   "e-mail: linux.corn.os.team@gmail.com<br>"
                   "more projects on github: <a href=" + ref + ">corn-team</a>";
    QMessageBox::about(this, "About", text);
}

void MainWindow::showHelpMessage()
{
    QMessageBox::information(this, "Help", "+ to download new wallpaper<br>"
                                           "- to remove selected wallpaper<br>"
                                           "X to close wallpaper<br>", QMessageBox::Ok);
}

MainWindow::~MainWindow()
{
    delete ui;
}
