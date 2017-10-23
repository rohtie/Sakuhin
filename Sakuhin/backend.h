#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class BackEnd : public QObject {
    Q_OBJECT
    Q_PROPERTY(
    	double sliderValue
    	READ sliderValue
    	WRITE setSliderValue
    	NOTIFY sliderValueChanged
    )

    public:
        explicit BackEnd(QObject *parent = nullptr);

        double sliderValue();
        void setSliderValue(const double &sliderValue);
        Q_INVOKABLE void createSession();

    signals:
        void sliderValueChanged();

    private:
        double m_sliderValue;
};

#endif // BACKEND_H
