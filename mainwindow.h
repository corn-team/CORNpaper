#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialogsettings.h"

#include <QMainWindow>

#include <QLabel>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>
#include <QSettings>

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

    void saveWallpapersFolder();

    void saveWallpaper();

    bool eventFilter(QObject *obj, QEvent *e);

private slots:
    void onYourButtonClicked();

    void onNewButtonClicked();

    void showDialogSettings();

private:
    Ui::MainWindow *ui;

    QString settingsFile;
    QString wallpapersFolder;
    QString wallpaperName;

    DialogSettings *dialogSettings;
};

#endif // MAINWINDOW_H
