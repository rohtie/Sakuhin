#include "sessionmanager.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include <QString>
#include <QSurfaceFormat>

#include "shadermanager.h"
#include "shader.h"
#include "scenemanager.h"
#include "scene.h"
#include "backend.h"
#include "audiomanager.h"
#include "windowmanager.h"

SessionManager::SessionManager(QObject *parent) : QObject(parent) {

}

void SessionManager::initialize(const QSurfaceFormat &format, QObject* qmlRoot) {
    backend = qmlRoot->findChild<Backend*>();
    audiomanager = qmlRoot->findChild<AudioManager*>();
    shadermanager = qmlRoot->findChild<ShaderManager*>();
    windowmanager = qmlRoot->findChild<WindowManager*>();
    scenemanager = qmlRoot->findChild<SceneManager*>();

    this->format = format;

    backend->initialize();
    audiomanager->initialize();

    // createSession();

    loadSession("/home/thorml/Projects/BlackPurple");

}

void SessionManager::createSession() {
    QString creationTime = QString::number(QDateTime::currentSecsSinceEpoch());
    sessionID = creationTime;
    sessionPath = "sessions/" + sessionID;
    shadermanager->sessionPath = sessionPath;

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
    session["time"] = sessionID;

    QJsonDocument sessionDocument(session);
    sessionFile.write(sessionDocument.toJson());
    sessionFile.close();


    QFile shaderFile(":/templates/default.glsl");
    if (!shaderFile.copy(sessionPath + "/session.glsl")) {
        qDebug() << "Couldn't copy default template to session folder";
        return;
    }
    shaderFile.close();

    QJsonArray shaders;
    shadermanager->initialize(format, shaders);
    windowmanager->initialize(format, backend, shadermanager, scenemanager);
    scenemanager->initialize(shadermanager);
}

void SessionManager::loadSession(const QString path) {
    sessionPath = path;
    shadermanager->sessionPath = path;

    QFile sessionFile(sessionPath + "/session.json");
    sessionFile.open(QIODevice::ReadOnly);
    QByteArray data = sessionFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data);
    sessionFile.close();

    // Init shaders
    QJsonArray shaders = document["shaders"].toArray();

    shadermanager->initialize(format, shaders);
    windowmanager->initialize(format, backend, shadermanager, scenemanager);
    scenemanager->initialize(shadermanager);
}

void SessionManager::saveSession() {
    QJsonObject session;

    // Save shaders
    QJsonArray shaders;
    for (int i=0; i<(shadermanager->shaders).count(); i++) {
        Shader* currentShader = (Shader*) shadermanager->shaders.at(i);
        shaders.append(*(currentShader->toJson()));
    }
    session["shaders"] = shaders;

    // Save scenes
    QJsonArray scenes;
    for (int i=0; i<(scenemanager->scenes).count(); i++) {
        Scene* currentScene = (Scene*) scenemanager->scenes.at(i);
        scenes.append(*(currentScene->toJson()));
    }
    session["scenes"] = scenes;

    // Write to json file
    QFile sessionFile(sessionPath + "/session.json");
    sessionFile.open(QIODevice::WriteOnly);
    QJsonDocument sessionDocument(session);
    sessionFile.write(sessionDocument.toJson());
    sessionFile.close();
}
