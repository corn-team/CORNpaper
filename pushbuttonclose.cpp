#include "pushbuttonclose.h"

PushButtonClose::PushButtonClose(QWidget *parent) :
    QPushButton(parent)
{

}

void PushButtonClose::leaveEvent(QEvent *)
{
    setIcon(QIcon(":/icons/close.png"));
}

void PushButtonClose::enterEvent(QEvent *)
{
    setIcon(QIcon(":/icons/close_hover.png"));
}
