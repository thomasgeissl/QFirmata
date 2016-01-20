#include "mainwindow.h"
#include <QDebug>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStatusBar>
#include <QLabel>
#include <QGroupBox>
#include <QScrollArea>
#include <QTableWidget>
#include <QHeaderView>

#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _firmata(new QFirmata(this)),
      _firstDigitalPinIndex(2)
{
    for(unsigned int i = 0; i <= 5; i++)
    {
        _analogPinVec.push_back(new AnalogPin(i, this));
        connect(_analogPinVec[i], SIGNAL(pinReportingChanged(uint,bool)), this, SLOT(onAnalogPinReportinChange(uint,bool)));
    }
    for(unsigned int i = 2; i <= 13; i++)
    {
        _digitalPinVec.push_back(new DigitalPin(i, this));
        connect(_digitalPinVec.last(), SIGNAL(pinModeChanged(uint,int)), this, SLOT(onDigitalPinModeChange(uint,int)));
        connect(_digitalPinVec.last(), SIGNAL(valueChanged(uint,uint,bool)), this, SLOT(onDigitalPinWidgetValueChanged(uint,uint,bool)));
    }

    _portComboBox = new QComboBox(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        _portComboBox->addItem(info.portName());
    }

    _connectButton = new QPushButton(tr("connect"),this);
    _disconnectButton = new QPushButton(tr("disconnect"),this);
    _firmwareLabel = new QLabel();


    QWidget *widget = new QWidget(this);


    QVBoxLayout *layout = new QVBoxLayout(widget);

    QWidget *serialWidget = new QWidget(this);
    QHBoxLayout *serialWidgetLayout = new QHBoxLayout(serialWidget);
    serialWidget->setLayout(serialWidgetLayout);
    serialWidgetLayout->addWidget(_portComboBox);
    serialWidgetLayout->addWidget(_connectButton);
    serialWidgetLayout->addWidget(_disconnectButton);
    serialWidgetLayout->addWidget(_firmwareLabel);
    serialWidgetLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding));

    QLabel *serialLabel = new QLabel("Serial settings");
    QFont font = serialLabel->font();
    font.setBold(true);
    font.setPointSize(20);
    serialLabel->setFont(font);



    layout->addWidget(serialLabel);
    layout->addWidget(serialWidget);

    QWidget *pinWidget = new QWidget(this);
    _analogPinWidget = new QWidget(this);
    _digitalPinWidget = new QWidget(this);

    QHBoxLayout *pinLayout = new QHBoxLayout(pinWidget);
    QVBoxLayout *analogPinLayout = new QVBoxLayout(_analogPinWidget);
    analogPinLayout->setSpacing(0);
    analogPinLayout->setMargin(0);
    QVBoxLayout *digitalPinLayout = new QVBoxLayout(_digitalPinWidget);
    digitalPinLayout->setSpacing(0);
    digitalPinLayout->setMargin(0);
    pinWidget->setLayout(pinLayout);
    _analogPinWidget->setLayout(analogPinLayout);
    _digitalPinWidget->setLayout(digitalPinLayout);

    layout->addWidget(pinWidget);

    pinLayout->addWidget(_analogPinWidget);
    pinLayout->addWidget(_digitalPinWidget);

    QLabel *analogPinLabel = new QLabel("Analog input pins");
    QFont pinFont = analogPinLabel->font();
    pinFont.setBold(true);
    pinFont.setPointSize(16);
    analogPinLabel->setFont(pinFont);
    analogPinLayout->addWidget(analogPinLabel);
    analogPinLayout->addSpacerItem(new QSpacerItem(1,1, QSizePolicy::Expanding));

    foreach(AnalogPin *pin, _analogPinVec)
    {
        analogPinLayout->addWidget(pin);
    }

    QLabel *digitalPinLabel = new QLabel("Digitale pins");
    digitalPinLabel->setFont(pinFont);
    digitalPinLayout->addWidget(digitalPinLabel);
    foreach(DigitalPin *pin, _digitalPinVec)
    {
        digitalPinLayout->addWidget(pin);
    }



    widget->setLayout(layout);
    setCentralWidget(widget);



    connect(_connectButton, SIGNAL(clicked()), this, SLOT(onConnectButtonClicked()));
    connect(_disconnectButton, SIGNAL(clicked()), this, SLOT(onDisconnectButtonClicked()));

    connect(_firmata, SIGNAL(firmwareVersionReceived(int, int)), this, SLOT(onFirmwareVersionReceived(int,int)));
    connect(_firmata, SIGNAL(firmwareNameReceived(QString)), this, SLOT(onFirmwareNameReceived(QString)));
    connect(_firmata, SIGNAL(initialized(int,int,QString)), this, SLOT(onInitialized(int,int,QString)));
    connect(_firmata, SIGNAL(digitalPinChanged(int)), this, SLOT(onDigitalPinChanged(int)));
    connect(_firmata, SIGNAL(analogPinChanged(int)), this, SLOT(onAnalogPinChanged(int)));

//    _analogPinWidget->setDisabled(true);
//    _digitalPinWidget->setDisabled(true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::onConnectButtonClicked()
{
    if(_firmata->open(_portComboBox->currentText()))
    {
        statusBar()->showMessage(tr("[Notice] Successfully opened connection"), 5000);
    }
    else
    {
        statusBar()->showMessage(tr("[Error] Could not open connection"), 5000);
    }

}
void MainWindow::onDisconnectButtonClicked()
{
    _firmata->close();
    _connectButton->show();
    _disconnectButton->hide();
    _firmwareLabel->setText("");
    statusBar()->showMessage(tr("[Notice] Successfully closed connection"), 5000);

}


void MainWindow::onPinModeComboBoxChange(int index)
{
    qDebug()<<"pinmode changed"<<_ioDigitalPinModeComboBoxVec[index]->currentText();
    switch(_ioDigitalPinModeComboBoxVec[index]->currentIndex())
    {
    case 0: _firmata->sendDigitalPinReporting(index+2, ARD_OFF); break;
    case 1: _firmata->sendDigitalPinReporting(index+2, ARD_ON); _firmata->sendDigitalPinMode(index+2, ARD_INPUT); _ioDigitalCheckBoxVec[index]->show(); _ioDigitalCheckBoxVec[index]->setEnabled(false); _ioDigitalPinSliderVec[index]->hide(); break;
    case 2: _firmata->sendDigitalPinMode(index+2, ARD_OUTPUT); _ioDigitalCheckBoxVec[index]->show(); _ioDigitalCheckBoxVec[index]->setEnabled(true); _ioDigitalPinSliderVec[index]->hide(); break;
    case 3: _firmata->sendDigitalPinMode(index+2, ARD_PWM); _ioDigitalCheckBoxVec[index]->hide(); _ioDigitalPinSliderVec[index]->show(); _ioDigitalPinSliderVec[index]->setEnabled(true); break;
    }
}


void MainWindow::onDigitalPinWidgetValueChanged(unsigned int id, unsigned int value, bool pwm)
{
    qDebug()<<"digital pin"<<id<<value<<pwm;

    if(pwm)
    {
        _firmata->sendPWM(id, value);
    }
    else
    {
        _firmata->sendDigital(id, value);
    }
}
void MainWindow::onFirmwareVersionReceived(const int majorVersion, const int minorVersion)
{
    qDebug()<<"major"<<majorVersion<<"minor"<<minorVersion;
}
void MainWindow::onFirmwareNameReceived(QString firmwareName)
{
    qDebug()<<"firmware name"<<firmwareName;
}
void MainWindow::onInitialized(const int majorVersion, const int minorVersion, QString firmwareName)
{
    qDebug()<<"firmate initialized"<<majorVersion<<minorVersion<<firmwareName;
    _analogPinWidget->setEnabled(true);
    _digitalPinWidget->setEnabled(true);
    statusBar()->showMessage(tr("initialized"), 5000);

    QString statusText = QString::number(majorVersion)+"."+QString::number(minorVersion);
}
void MainWindow::onDigitalPinChanged(int pin)
{
    qDebug()<<"digital pin"<<pin<<"changed. new value"<<_firmata->getDigital(pin);
    _digitalPinVec[pin-2]->setValue(_firmata->getDigital(pin));
}

void MainWindow::onAnalogPinChanged(int pin)
{
    if(pin < _analogPinVec.size())
    {
        _analogPinVec[pin]->setValue(_firmata->getAnalog(pin));
    }
}
void MainWindow::onSysExReceived(const QVector<unsigned char> message)
{
    qDebug()<<"sysExReceived"<<message;
}

void MainWindow::onAnalogPinReportinChange(unsigned int pin, bool value)
{
    qDebug()<<"analog pin reporting"<<pin<<"changed. new value"<<value;

    if(value)
    {
        _firmata->sendAnalogPinReporting(pin, ARD_ANALOG);
    }
    else
    {
        _firmata->sendAnalogPinReporting(pin, ARD_OFF);
    }
}

void MainWindow::onDigitalPinModeChange(unsigned int pin, int mode)
{
    switch(mode)
    {
        //none
    case 0: _firmata->sendDigitalPinReporting(pin, ARD_OFF);
        break;
    case 1: _firmata->sendDigitalPinMode(pin, ARD_INPUT); _firmata->sendDigitalPinReporting(pin, ARD_ON);
        break;
    case 2: _firmata->sendDigitalPinReporting(pin, ARD_OFF); _firmata->sendDigitalPinMode(pin, ARD_OUTPUT);
        break;
    case 3: _firmata->sendDigitalPinReporting(pin, ARD_OFF); _firmata->sendDigitalPinMode(pin, ARD_PWM);
        break;
    default:
        break;
    }

}
