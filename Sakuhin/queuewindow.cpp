#include "queuewindow.h"
#include "queuemanager.h"

QueueWindow::QueueWindow() {

}

void QueueWindow::initializeGL() {
    Window::initializeGL();

    serialPort.setPortName("/dev/ttyUSB0");
    serialPort.setBaudRate(QSerialPort::Baud9600);

    if (!serialPort.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open port";
        return;
    }

    connect(&serialPort, &QSerialPort::readyRead, this, &QueueWindow::readSerial);
}

void QueueWindow::readSerial() {
    if (serialPort.canReadLine()) {
        char buffer[1024];

        if (serialPort.readLine(buffer, sizeof(buffer)) != -1) {
            int signal = (QString(buffer).toInt() == 1);

            if (signal == 1) {
                queuemanager->nextScene();
            }

            qDebug() << "[" << signal << "]";
        }
    }
}
