#ifndef ANALOGPIN_H
#define ANALOGPIN_H

#include <QWidget>
#include <QSpinBox>

#include "pin.h"
class AnalogPin : public Pin
{
    Q_OBJECT
public:
    explicit AnalogPin(unsigned int pin, QWidget *parent = 0);

signals:
    void pinReportingChanged(unsigned int pin, bool value);

public slots:
private slots:
    void onCheckBoxChange(bool value);
};

#endif // ANALOGPIN_H
