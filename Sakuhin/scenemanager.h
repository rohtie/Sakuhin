#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QDateTime>

#include "scene.h"

class SceneManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> scenes MEMBER scenes NOTIFY scenesChanged)
    Q_PROPERTY(double audioDuration MEMBER audioDuration NOTIFY audioDurationChanged)
    Q_PROPERTY(double markerPosition MEMBER markerPosition NOTIFY markerPositionChanged)
    Q_PROPERTY(double loopAreaStart MEMBER loopAreaStart NOTIFY loopAreaStartChanged)
    Q_PROPERTY(double loopAreaLength MEMBER loopAreaLength NOTIFY loopAreaLengthChanged)

    public:
        explicit SceneManager(QObject *parent = nullptr);
        void initialize();
        Q_INVOKABLE void swap(int indexA, int indexB);
        Q_INVOKABLE void togglePlay();
        Q_INVOKABLE void skipTo(double position);
        Q_INVOKABLE void newScene();

        double audioDuration = 0;
        double markerPosition = 0;
        double loopAreaStart = 0;
        double loopAreaLength = 0;


    signals:
        void scenesChanged();
        void audioDurationChanged();
        void markerPositionChanged();
        void loopAreaStartChanged();
        void loopAreaLengthChanged();

    public slots:
        void onMediaLoaded();
        void onPositionChanged();

    private:
        QList<QObject*> scenes;

        QMediaPlayer mediaPlayer;

};

#endif // SCENEMANAGER_H
