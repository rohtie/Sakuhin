#include "backend.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include <QString>

BackEnd::BackEnd(QObject* parent) : QObject(parent) {
    easingNames << "Linear" << "QuadraticIn" << "QuadraticOut" << "QuadraticInOut" << "CubicIn" << "CubicOut" << "CubicInOut" << "QuarticIn" << "QuarticOut" << "QuarticInOut" << "QuinticIn" << "QuinticOut" << "QuinticInOut" << "SineIn" << "SineOut" << "SineInOut" << "CircularIn" << "CircularOut" << "CircularInOut" << "ExponentialIn" << "ExponentialOut" << "ExponentialInOut" << "ElasticIn" << "ElasticOut" << "ElasticInOut" << "BackIn" << "BackOut" << "BackInOut" << "BounceOut" << "BounceIn" << "BounceInOut";
}

void BackEnd::setPerformanceInformation(const QString &performanceInformation) {
    this->performanceInformation = performanceInformation;

    emit performanceInformationChanged();
}

void BackEnd::createSession() {
    QString creationTime = QString::number(QDateTime::currentSecsSinceEpoch());
    sessionID = creationTime;
    sessionPath = "sessions/" + sessionID;

    QDir rootDirectory(QDir::currentPath());

    if (!rootDirectory.mkpath(sessionPath)) {
        qDebug() << "Couldn't make path";
        return;
    }

    if (!rootDirectory.mkpath(sessionPath + "/shaders/thumbnails")) {
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
    sessionFile.close();


    QFile shaderFile(":/templates/default.glsl");
    if (!shaderFile.copy(sessionPath + "/session.glsl")) {
        qDebug() << "Couldn't copy default template to session folder";
        return;
    }
    shaderFile.close();
}

QString BackEnd::getSessionID() {
    return sessionID;
}
