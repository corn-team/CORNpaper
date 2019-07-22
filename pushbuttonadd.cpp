#include "pushbuttonadd.h"

PushButtonAdd::PushButtonAdd(QWidget *parent) :
    QPushButton(parent)
{

}

void PushButtonAdd::leaveEvent(QEvent *)
{
    setIcon(QIcon(":/icons/add.png"));
}

void PushButtonAdd::enterEvent(QEvent *)
{
    setIcon(QIcon(":/icons/add_hover.png"));
}
