#include "scenemanager.h"

#include <QFileInfo>
#include "shadermanager.h"
#include "scene.h"

SceneManager::SceneManager(QObject *parent) : QObject(parent) {

}

void SceneManager::initialize(ShaderManager* shadermanager) {
    this->shadermanager = shadermanager;

    mediaPlayer.setMedia(QUrl::fromLocalFile(QFileInfo("data/audio/BP.mp3").absoluteFilePath()));

    QObject::connect(&mediaPlayer, &QMediaPlayer::durationChanged,
                     this, &SceneManager::onMediaLoaded);

    QObject::connect(&mediaPlayer, &QMediaPlayer::positionChanged,
                     this, &SceneManager::onPositionChanged);

    QObject::connect(shadermanager, &ShaderManager::mainIndexChanged,
                     this, &SceneManager::onSceneShaderChanged);


    // Setup mediaplayer to update marker position every milisec
    mediaPlayer.setNotifyInterval(1);

    emit scenesChanged();
}

void SceneManager::onMediaLoaded() {
    audioDuration = (double) mediaPlayer.duration();
    audioDurationChanged();
}

void SceneManager::onPositionChanged() {
    markerPosition = (double) mediaPlayer.position();

    if (loopAreaLength > 0 && markerPosition > loopAreaStart + loopAreaLength) {
        skipTo(loopAreaStart);
    }
    else {
        selectScene();

        emit markerPositionChanged();
    }
}

void SceneManager::onSceneShaderChanged() {
    if (scenes.count() > 0) {
        Scene* currentScene = (Scene*) scenes.at(currentSceneIndex);

        if (currentScene->shaderIndex != shadermanager->mainIndex) {
            currentScene->shaderIndex = shadermanager->mainIndex;
        }
    }
}

void SceneManager::swap(int indexA, int indexB) {
    scenes.swap(indexA, indexB);

    emit scenesChanged();
}

void SceneManager::togglePlay() {
    if (mediaPlayer.state() != QMediaPlayer::PlayingState) {
        mediaPlayer.play();
    }
    else {
        mediaPlayer.pause();
    }
}

void SceneManager::skipTo(double position) {
    mediaPlayer.setPosition((qint64) position);
}

void SceneManager::newScene() {
    scenes.append(new Scene(QString::number(QDateTime::currentMSecsSinceEpoch()), 0));
    recalculateScenes();
    emit scenesChanged();
}

void SceneManager::selectScene() {
    if (scenes.count() == 0) {
        return;
    }
    else if (scenes.count() == 1) {
        Scene* currentScene = (Scene*) scenes.at(0);

        currentSceneIndex = 0;
        currentSceneMin = 0;
        currentSceneMax = currentScene->length;
    }
    else if (markerPosition <= currentSceneMin) {
        for (int i=currentSceneIndex - 1; i>=0; i--) {
            Scene* currentScene = (Scene*) scenes.at(i);

            currentSceneMax = currentSceneMin;
            currentSceneMin -= currentScene->length;

            if (markerPosition > currentSceneMin) {
                currentSceneIndex = i;
                break;
            }
        }
    }
    else if (markerPosition >= currentSceneMax) {
        for (int i=currentSceneIndex + 1; i<scenes.count(); i++) {
            currentSceneMin = currentSceneMax;
            currentSceneMax += ((Scene*) scenes.at(i))->length;

            if (markerPosition < currentSceneMax) {
                currentSceneIndex = i;
                break;
            }
        }
    }
    else {
        // We don't need to change the scene
        // because the marker is on the scene
        return;
    }

    Scene* currentScene = (Scene*) scenes.at(currentSceneIndex);
    int currentShaderIndex = currentScene->shaderIndex;

    shadermanager->selectShader(currentShaderIndex);
    shadermanager->makeCurrent(currentShaderIndex);
}

void SceneManager::recalculateScenes() {
    currentSceneMin = 0;

    if (scenes.count() > 0) {
        for (int i=0; i<currentSceneIndex; i++) {
            currentSceneMin += ((Scene*) scenes.at(i))->length;
        }

        currentSceneMax = currentSceneMin + ((Scene*) scenes.at(currentSceneIndex))->length;
    }

    selectScene();
}
