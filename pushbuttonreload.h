#ifndef PUSHBUTTONRELOAD_H
#define PUSHBUTTONRELOAD_H

#include <QPushButton>

class PushButtonReload : public QPushButton
{
public:
    PushButtonReload(QWidget *parent);

protected:
    void leaveEvent(QEvent*);

    void enterEvent(QEvent*);

};

#endif // PUSHBUTTONRELOAD_H
