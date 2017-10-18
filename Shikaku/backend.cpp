#include "backend.h"
#include <QtDebug>

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
}

double BackEnd::sliderValue()
{
    return m_sliderValue;
}

void BackEnd::setSliderValue(const double &sliderValue)
{
    if (sliderValue == m_sliderValue)
        return;

    qDebug() << sliderValue;

    m_sliderValue = sliderValue;
    emit sliderValueChanged();
}
