#include "pushbuttonsettings.h"

PushButtonSettings::PushButtonSettings(QWidget *parent) :
    QPushButton(parent)
{
    qDebug() << 3;
}

void PushButtonSettings::leaveEvent(QEvent*)
{
    setIcon(QIcon(":/icons/settings.png"));
}

void PushButtonSettings::enterEvent(QEvent*)
{
    setIcon(QIcon(":/icons/settings_hover.png"));
}
