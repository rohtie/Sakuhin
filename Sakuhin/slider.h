#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>

class Slider : public QObject {
    Q_OBJECT

    Q_PROPERTY(double value MEMBER value NOTIFY valueChanged)

    public:
        explicit Slider(QObject* parent = nullptr);
        double value;

    signals:
        void valueChanged();
};

#endif // SLIDER_H
