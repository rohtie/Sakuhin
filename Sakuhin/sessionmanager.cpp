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
#include <QRegExp>

#include "shadermanager.h"
#include "shader.h"
#include "scenemanager.h"
#include "scene.h"
#include "channel.h"
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

    QJsonArray emptyJsonArray;
    shadermanager->initialize(format, emptyJsonArray);
    scenemanager->initialize(shadermanager, emptyJsonArray);

    windowmanager->initialize(format, backend, shadermanager, scenemanager);
}

void SessionManager::loadSession(const QString path) {
    sessionPath = path;
    shadermanager->sessionPath = path;

    QFile sessionFile(sessionPath + "/session.json");
    sessionFile.open(QIODevice::ReadOnly);
    QByteArray data = sessionFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data);
    sessionFile.close();

    // Load shaders
    QJsonArray shaders = document["shaders"].toArray();
    shadermanager->initialize(format, shaders);

    // Load scenes
    QJsonArray scenes = document["scenes"].toArray();
    scenemanager->initialize(shadermanager, scenes);

    // TODO: Load windows
    windowmanager->initialize(format, backend, shadermanager, scenemanager);
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

void SessionManager::exportSession() {
    QDir sessionDir(sessionPath);
    if (!sessionDir.mkpath("export/")) {
        qDebug() << "Couldn't make path";
        return;
    }

    // Export shader code
    QFile shaderCodeFile(sessionPath + "/export/shader_code.asm");
    shaderCodeFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream shaderCodeStream(&shaderCodeFile);

    int biggestShaderSize = 0;

    for (int i=0; i<shadermanager->shaders.count(); i++) {
        Shader* currentShader = (Shader*) shadermanager->shaders.at(i);

        // Section header
        shaderCodeStream << "section shader" << i << "_code data align=1\n";
        shaderCodeStream << "    shader" << i << "_code db \"\"\n";

        // Main shader code
        QFile currentShaderFile(currentShader->filepath);
        currentShaderFile.open(QIODevice::ReadOnly);
        QTextStream inStream(&currentShaderFile);

        int currentShaderSize = 0;

        while (!inStream.atEnd()) {
            QString line = inStream.readLine();

            QRegExp indentation("(^\\s+)");
            indentation.indexIn(line);


            line = line.simplified();

            line.replace(" > ", ">");
            line.replace(" < ", "<");
            line.replace(" <= ", "<=");
            line.replace(" >= ", ">=");
            line.replace(" == ", "==");

            line.replace(" = ", "=");

            line.replace(" + ", "+");
            line.replace(" - ", "-");
            line.replace(" * ", "*");
            line.replace(" / ", "/");

            line.replace(" += ", "+=");
            line.replace(" -= ", "-=");
            line.replace(" *= ", "*=");
            line.replace(" /= ", "/=");

            line.replace(", ", ",");
            line.replace("; ", ";");

            // Minimize floats
            line.replace(QRegExp("(\\d+)\\.0(?!\\d)"), "\\1");
            line.replace(QRegExp("([^\\d])0(\\.\\d+)"), "\\1\\2");

            // Minimize uniforms
            line.replace(QRegExp("channel(\\d)"), "c\\1");
            line.replace("time", "it");
            line.replace("resolution", "ir");
            line.replace("pixel", "pix");

            if (line.isEmpty()) {
                shaderCodeStream << "\n";
            }
            else if (line.leftRef(2) != "//") {
                currentShaderSize += line.size();
                shaderCodeStream << "        db " << indentation.cap(0) << "\"" << line << "\"\n";
            }
        }
        currentShaderFile.close();

        biggestShaderSize = qMax(biggestShaderSize, currentShaderSize);

        // Section footer
        shaderCodeStream << "        db \"\", 0\n\n";
        shaderCodeStream << "    shader" << i << "_code_size equ $-shader" << i << "_code\n\n";
    }

    shaderCodeFile.close();


    // Allocate variables
    QFile allocationFile(sessionPath + "/export/allocate_shaders_and_scenes.asm");
    allocationFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream allocationStream(&allocationFile);

    allocationStream << "shaders            resd SHADER_size * " << shadermanager->shaders.count() << "\n";
    allocationStream << "scenes             resd SCENE_size * " << scenemanager->scenes.count() << "\n";
    allocationStream << "shadercode_buffer  resb shaderheader_size + " << biggestShaderSize << " + shaderfooter_size\n";

    allocationFile.close();


    // Setup shader channels
    QFile setupShaderFile(sessionPath + "/export/setup_shaders.asm");
    setupShaderFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream setupShaderStream(&setupShaderFile);

    for (int i=0; i<shadermanager->shaders.count(); i++) {
        Shader* currentShader = (Shader*) shadermanager->shaders.at(i);

        if (i == 0) {
            setupShaderStream << "mov esi, shaders\n";
        }
        else {
            setupShaderStream << "add esi, SHADER_size\n";
        }

        for (int channelIndex=0; channelIndex<currentShader->channels.count(); channelIndex++) {
            Channel* currentChannel = (Channel*) (currentShader->channels.at(channelIndex));

            if (currentChannel->channelType == Channel::ShaderType) {
                setupShaderStream << ""
                    << "mov dword [esi + SHADER.channel" << channelIndex << "],";

                if (currentChannel->shader->index == i) {
                    setupShaderStream << " esi\n";
                }
                else {
                    setupShaderStream << " shaders + SHADER_size * " << currentChannel->shader->index << "\n";
                }
            }
        }

        setupShaderStream << "push dword shader" << i << "_code\n";
        setupShaderStream << "call process_shader\n\n";
    }

    setupShaderFile.close();


    // Setup scenes
    QFile setupSceneFile(sessionPath + "/export/setup_scenes.asm");
    setupSceneFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream setupSceneStream(&setupSceneFile);

    int endTime = 0;

    for (int i=0; i<scenemanager->scenes.count(); i++) {
        Scene* currentScene = (Scene*) scenemanager->scenes.at(i);

        if (i == 0) {
            setupSceneStream << "mov esi, scenes\n";
        }
        else {
            setupSceneStream << "add esi, SCENE_size\n";
        }

        endTime += currentScene->length;

        setupSceneStream << "mov dword [esi + SCENE.endTime], " << endTime << "\n";
        setupSceneStream << "mov dword [esi + SCENE.shader], shaders + SHADER_size * " << currentScene->shaderIndex  << "\n\n";
    }

    setupSceneFile.close();
}
