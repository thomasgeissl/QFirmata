#include "analogpin.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

AnalogPin::AnalogPin(unsigned int pin, QWidget *parent) : Pin(pin, parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);

    QLabel *label = new QLabel(QString::number(_pin));
    mainLayout->addWidget(label);

    QCheckBox *checkBox = new QCheckBox(this);
    connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(onCheckBoxChange(bool)));
    mainLayout->addWidget(checkBox);

    _spinBox->setMinimum(0);
    _spinBox->setMaximum(1023);
    _spinBox->setDisabled(true);

    mainLayout->addWidget(_spinBox);


}

void AnalogPin::onCheckBoxChange(bool value)
{
    emit pinReportingChanged(_pin, value);
}

