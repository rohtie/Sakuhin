#include "backend.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include <QString>

BackEnd::BackEnd(QObject *parent) : QObject(parent) {

}

void BackEnd::setSlider(const int &id, const float &value) {
    slider[id] = value;

    QJsonObject change;
    change["id"] = id;
    change["value"] = value;

    QJsonArray changes;
    changes.append(change);

    controllerLog.insert(
        QString::number(QDateTime::currentMSecsSinceEpoch()),
        changes
    );
}

float* BackEnd::getSliders() {
    return slider;
}

void BackEnd::setPerformanceInformation(const QString &performanceInformation) {
    this->performanceInformation = performanceInformation;

    emit performanceInformationChanged();
}

void BackEnd::onShaderRecompile() {
    if (!controllerLogFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Couldn't open controller log file";
        return;
    }

    QJsonDocument document(controllerLog);
    controllerLogFile.write(document.toJson());
    controllerLogFile.close();
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


    controllerLogFile.setFileName(sessionPath + "/controller.json");
    if (!controllerLogFile.open(QIODevice::ReadWrite)) {
        qDebug() << "Couldn't open controller log file";
        return;
    }

    controllerLog = QJsonDocument::fromJson(controllerLogFile.readAll()).object();
    controllerLogFile.close();
}

QString BackEnd::getSessionID() {
    return sessionID;
}

void BackEnd::setChannel(const int &channelID, ChannelType channelType, const QString &fileUrl) {
    emit channelChanged(channelID, channelType, fileUrl);
}

