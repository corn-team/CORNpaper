#ifndef PUSHBUTTONSETTINGS_H
#define PUSHBUTTONSETTINGS_H

#include <QPushButton>

#include <QMouseEvent>
#include <QDebug>

class PushButtonSettings : public QPushButton
{
public:
    PushButtonSettings(QWidget *parent);

protected:
    void leaveEvent(QEvent*);

    void enterEvent(QEvent*);

};

#endif // PUSHBUTTONSETTINGS_H
