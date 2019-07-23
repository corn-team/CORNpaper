#ifndef PUSHBUTTONADD_H
#define PUSHBUTTONADD_H

#include <QPushButton>

class PushButtonAdd : public QPushButton
{
public:
    PushButtonAdd(QWidget *parent);

protected:
    void leaveEvent(QEvent*);

    void enterEvent(QEvent*);

};

#endif // PUSHBUTTONADD_H
