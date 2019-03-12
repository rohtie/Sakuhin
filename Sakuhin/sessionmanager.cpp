#include "sessionmanager.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include <QString>

#include "shadermanager.h"

SessionManager::SessionManager(QObject *parent) : QObject(parent) {

}

void SessionManager::initialize(ShaderManager* shadermanager, SceneManager* scenemanager, WindowManager* windowmanager) {
    this->shadermanager = shadermanager;
    this->scenemanager = scenemanager;
    this->windowmanager = windowmanager;
}

void SessionManager::createSession() {
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

    // TODO: Use initialize?
    shadermanager->sessionID = sessionID;
}

void SessionManager::loadSession(const QString path) {

}

void SessionManager::saveSession() {

}
