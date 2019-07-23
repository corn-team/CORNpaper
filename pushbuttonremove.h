#ifndef PUSHBUTTONREMOVE_H
#define PUSHBUTTONREMOVE_H

#include <QPushButton>

class PushButtonRemove : public QPushButton
{
public:
    PushButtonRemove(QWidget *parent);

protected:
    void leaveEvent(QEvent*);

    void enterEvent(QEvent*);

};

#endif // PUSHBUTTONREMOVE_H
