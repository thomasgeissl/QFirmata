#ifndef PIN_H
#define PIN_H

#include <QWidget>
#include <QSpinBox>

class Pin : public QWidget
{
    Q_OBJECT
public:
    explicit Pin(unsigned int pin, QWidget *parent = 0);
    void setValue(int value);
protected:
    unsigned int _pin;
    QSpinBox *_spinBox;

signals:

public slots:
};

#endif // PIN_H
