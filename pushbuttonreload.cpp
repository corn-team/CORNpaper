#include "pushbuttonreload.h"

PushButtonReload::PushButtonReload(QWidget *parent) :
    QPushButton(parent)
{

}

void PushButtonReload::leaveEvent(QEvent *)
{
    setIcon(QIcon(":/icons/reload.png"));
}

void PushButtonReload::enterEvent(QEvent *)
{
    setIcon(QIcon(":/icons/reload_hover.png"));
}
