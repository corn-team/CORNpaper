#include "pushbuttonremove.h"

PushButtonRemove::PushButtonRemove(QWidget *parent) :
    QPushButton(parent)
{

}

void PushButtonRemove::leaveEvent(QEvent *)
{
    setIcon(QIcon(":/icons/remove.png"));
}

void PushButtonRemove::enterEvent(QEvent *)
{
    setIcon(QIcon(":/icons/remove_hover.png"));
}
