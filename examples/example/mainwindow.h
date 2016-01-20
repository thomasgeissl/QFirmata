#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTableWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QSignalMapper>
#include <QModelIndex>

#include "analogpin.h"
#include "digitalpin.h"

#include "qfirmata.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QFirmata *_firmata;
    int _firstDigitalPinIndex;

    QTableWidget *_configTableWidget;
    QComboBox *_portComboBox;
    QVector<QSpinBox*> _ioAnalogInputSpinBoxVec;
    QVector<QComboBox*> _ioDigitalPinModeComboBoxVec;
    QVector<QCheckBox*> _ioDigitalCheckBoxVec;
    QVector<QSlider*> _ioDigitalPinSliderVec;
    QVector<QSpinBox*> _ioDigitalPinSpinBoxVec;

    QPushButton *_connectButton;
    QPushButton *_disconnectButton;
    QLabel *_firmwareLabel;

    QScrollArea *_ioDigitalContainerWidget;
    QScrollArea *_ioAnalogInputContainerWidget;
    QWidget *_analogPinWidget;
    QWidget *_digitalPinWidget;

    QVector<AnalogPin*> _analogPinVec;
    QVector<DigitalPin*> _digitalPinVec;
private slots:
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();

    void onPinModeComboBoxChange(int index);
    void onDigitalPinWidgetValueChanged(unsigned int id, unsigned int value, bool pwm);

    void onFirmwareVersionReceived(const int majorVersion, const int minorVersion);
    void onFirmwareNameReceived(QString firmwareName);
    void onInitialized(const int majorVersion, const int minorVersion, QString firmwareName);
    void onDigitalPinChanged(int pin);
    void onAnalogPinChanged(int pin);
    void onSysExReceived(const QVector<unsigned char> message);

    void onAnalogPinReportinChange(unsigned int pin, bool value);
    void onDigitalPinModeChange(unsigned int pin, int mode);
};

#endif // MAINWINDOW_H
