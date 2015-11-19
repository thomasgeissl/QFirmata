#ifndef DIGITALPIN_H
#define DIGITALPIN_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include "pin.h"

class DigitalPin : public Pin
{
    Q_OBJECT
public:
    explicit DigitalPin(unsigned int pin, QWidget *parent = 0);
private:
    QComboBox *_comboBox;

signals:
    void pinModeChanged(unsigned int pin, int mode);
    void valueChanged(unsigned int pin, unsigned int value, bool pwm=false);

public slots:
private slots:
    void onPinModeChanged(int mode);
    void onValueChanged(int value);
    void setMode(int mode);


};

#endif // DIGITALPIN_H
