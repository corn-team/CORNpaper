#ifndef PUSHBUTTONCLOSE_H
#define PUSHBUTTONCLOSE_H

#include <QPushButton>

class PushButtonClose : public QPushButton
{
public:
    PushButtonClose(QWidget *parent);

protected:
    void leaveEvent(QEvent*);

    void enterEvent(QEvent*);

};

#endif // PUSHBUTTONCLOSE_H
