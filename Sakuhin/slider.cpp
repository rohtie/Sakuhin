#include "slider.h"

#include "easingfunctions.h"

Slider::Slider(QObject* parent) : QObject(parent) {
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(easeSlider()));
}

void Slider::startEase(int index) {
    currentFunction = easingfunctions[index];
    timer->start(50);
    time = 0.0;
}

void Slider::easeSlider() {
    time += 0.025;

    if (time > 1.) {
        time = 1.;
        timer->stop();
    }

    value = currentFunction(time);
    emit valueChanged();
}

QList<easingFunctionPointer> Slider::easingfunctions = QList<easingFunctionPointer>() << &Linear << &QuadraticIn << &QuadraticOut << &QuadraticInOut << &CubicIn << &CubicOut << &CubicInOut << &QuarticIn << &QuarticOut << &QuarticInOut << &QuinticIn << &QuinticOut << &QuinticInOut << &SineIn << &SineOut << &SineInOut << &CircularIn << &CircularOut << &CircularInOut << &ExponentialIn << &ExponentialOut << &ExponentialInOut << &ElasticIn << &ElasticOut << &ElasticInOut << &BackIn << &BackOut << &BackInOut << &BounceOut << &BounceIn << &BounceInOut;
