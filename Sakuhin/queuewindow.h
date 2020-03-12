#ifndef QUEUEWINDOW_H
#define QUEUEWINDOW_H

#include <QSerialPort>

#include "window.h"

class QueueWindow : public Window {
    public:
        QueueWindow();
        void initializeGL();


    private slots:
        void readSerial();

    protected:
        QSerialPort serialPort;
};

#endif // QUEUEWINDOW_H
