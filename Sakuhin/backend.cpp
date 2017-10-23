#include "backend.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtDebug>

BackEnd::BackEnd(QObject *parent) : QObject(parent) {

}

double BackEnd::sliderValue() {
    return m_sliderValue;
}

void BackEnd::setSliderValue(const double &sliderValue) {
    if (sliderValue == m_sliderValue)
        return;

    qDebug() << sliderValue;

    m_sliderValue = sliderValue;
    emit sliderValueChanged();
}

void BackEnd::createSession() {
    QString creationTime = QString::number(QDateTime::currentSecsSinceEpoch());
    sessionID = creationTime;
    QString sessionPath = "sessions/" + sessionID;

    QDir rootDirectory(QDir::currentPath());

    if (!rootDirectory.mkpath(sessionPath)) {
        qDebug() << "Couldn't make path";
        return;
    }

    QFile sessionFile(sessionPath + "/session.json");

    if (!sessionFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Couldn't open save file";
        return;
    }

    QJsonObject session;
    session["time"] = creationTime;

    QJsonDocument sessionDocument(session);
    sessionFile.write(sessionDocument.toJson());

    QFile shaderFile(":/templates/default.glsl");
    if (!shaderFile.copy(sessionPath + "/session.glsl")) {
        qDebug() << "Couldn't copy default template to session folder";
        return;
    }

    qDebug() << "Session is created";
}

QString BackEnd::getSessionID() {
    return sessionID;
}
