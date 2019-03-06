#include "scenemanager.h"

#include "scene.h"

#include <QFileInfo>

SceneManager::SceneManager(QObject *parent) : QObject(parent) {

}

void SceneManager::initialize() {
    mediaPlayer.setMedia(QUrl::fromLocalFile(QFileInfo("data/audio/BP.mp3").absoluteFilePath()));

    QObject::connect(&mediaPlayer, &QMediaPlayer::durationChanged,
                     this, &SceneManager::onMediaLoaded);

    QObject::connect(&mediaPlayer, &QMediaPlayer::positionChanged,
                     this, &SceneManager::onPositionChanged);

    mediaPlayer.setNotifyInterval(1);


    scenes.append(new Scene("Intro"));
    scenes.append(new Scene("Landscape"));
    scenes.append(new Scene("Rolling trees"));
    scenes.append(new Scene("Geometric balls"));
    scenes.append(new Scene("Outro"));

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
        emit markerPositionChanged();
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
    markerPosition = position;
    emit markerPositionChanged();
}
