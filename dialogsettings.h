#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = nullptr, QString wallpapersFolder = "");
    ~DialogSettings();

public:
    QString wallpapersFolder;

private:
    void initGui();

    bool eventFilter(QObject *obj, QEvent* e);

private slots:
    void wallpaperFolderChanged();

    void onChangeFolderButtonClicked();

    void cancel();

    void apply();

private:
    Ui::DialogSettings *ui;
};

#endif // DIALOGSETTINGS_H
