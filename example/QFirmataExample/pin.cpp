#include "pin.h"

Pin::Pin(unsigned int pin, QWidget *parent) : QWidget(parent),
    _pin(pin),
    _spinBox(new QSpinBox(this))
{
    _spinBox->setDisabled(true);
}

void Pin::setValue(int value)
{
    _spinBox->setValue(value);
}

