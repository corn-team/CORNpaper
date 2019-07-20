#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = nullptr, QString wallpapersFolder = "");
    ~DialogSettings();

private:
    void initGui();

    bool eventFilter(QObject *obj, QEvent* e);

private:
    Ui::DialogSettings *ui;

    QString wallpapersFolder;
};

#endif // DIALOGSETTINGS_H
