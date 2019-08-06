#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialogsettings.h"

#include <QMainWindow>

#include <QLabel>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QDirIterator>
#include <QListWidgetItem>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initGui();

    void loadSettings();

    void saveSettings();

    void saveWallpaper(QString fileName);

    QString transformWallpaperFileName(QString fileName);

private slots:
    void initWallpapersList();

    void onYourButtonClicked();

    void onNewButtonClicked();

    void showDialogSettings();

    void changeWallpaper(QListWidgetItem *item);

    void closeWallpaper();

    void removeWallpaper();

    void addWallpaper();

private:
    Ui::MainWindow *ui;

    QString wpId;
    QProcess *pro;

    QString settingsFile;
    QSettings *settings;
    QString wallpapersFolder;
    bool panoramic;
    QString wallpaperFileName;
    QMap<QString, QString> wallpapersList;

    DialogSettings *dialogSettings;
};

#endif // MAINWINDOW_H
