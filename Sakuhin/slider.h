#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>
#include <QTimer>
#include <QList>

typedef double (*easingFunctionPointer)(double);

class Slider : public QObject {
    Q_OBJECT

    Q_PROPERTY(double value MEMBER value NOTIFY valueChanged)

    public:
        explicit Slider(QObject* parent = nullptr);
        Q_INVOKABLE void startEase(int index);

        double value;

    public slots:
        void easeSlider();

    signals:
        void valueChanged();

    private:
        QTimer* timer;
        double time = 0.0;
        double (* currentFunction)(double);
        static QList<easingFunctionPointer> easingfunctions;
};

#endif // SLIDER_H
