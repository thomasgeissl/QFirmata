#include "digitalpin.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

DigitalPin::DigitalPin(unsigned int pin, QWidget *parent) : Pin(pin, parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);

    QLabel *label = new QLabel(QString::number(_pin));
    mainLayout->addWidget(label);

    _comboBox = new QComboBox(this);
    QStringList pinModeItemList; pinModeItemList << "None" << "Digital Input" << "Digital Output" << "PWM Output";
    _comboBox->addItems(pinModeItemList);
    connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onPinModeChanged(int)));
    connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)));

    mainLayout->addWidget(_comboBox);

    _spinBox->setMinimum(0);
    _spinBox->setMaximum(255);
    connect(_spinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    mainLayout->addWidget(_spinBox);

}

void DigitalPin::setMode(int mode)
{
    switch(mode)
    {
        //none
    case 0:
    case 1:
         _spinBox->setDisabled(true);
        break;
    case 2:
        _spinBox->setEnabled(true);
        _spinBox->setMaximum(1);
        break;
    case 3:
        _spinBox->setEnabled(true);
        _spinBox->setMaximum(1023);
        break;
    default:
        break;
    }
}


void DigitalPin::onPinModeChanged(int mode)
{
    emit pinModeChanged(_pin, mode);
}

void DigitalPin::onValueChanged(int value)
{
    bool pwm = false;
    if(_comboBox->currentIndex() == 3) pwm = true;
    emit valueChanged(_pin, value, pwm);
}


