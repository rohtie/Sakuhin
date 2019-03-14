#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

class ShaderManager;
class Scene;

#include <QObject>
#include <QMediaPlayer>
#include <QDateTime>
#include <QJsonArray>

class SceneManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> scenes MEMBER scenes NOTIFY scenesChanged)
    Q_PROPERTY(double audioDuration MEMBER audioDuration NOTIFY audioDurationChanged)
    Q_PROPERTY(double markerPosition MEMBER markerPosition NOTIFY markerPositionChanged)
    Q_PROPERTY(double loopAreaStart MEMBER loopAreaStart NOTIFY loopAreaStartChanged)
    Q_PROPERTY(double loopAreaLength MEMBER loopAreaLength NOTIFY loopAreaLengthChanged)

    public:
        explicit SceneManager(QObject *parent = nullptr);
        void initialize(ShaderManager* shadermanager, const QJsonArray &jsonScenes);
        Q_INVOKABLE void swap(int indexA, int indexB);
        Q_INVOKABLE void togglePlay();
        Q_INVOKABLE void skipTo(double position);
        Q_INVOKABLE void newScene();
        void selectScene();
        Q_INVOKABLE void recalculateScenes();

        double audioDuration = 0;
        double markerPosition = 0;
        double loopAreaStart = 0;
        double loopAreaLength = 0;

        int previousSceneIndex = 0;
        int currentSceneIndex = 0;

        double currentSceneMin = 0;
        double currentSceneMax = 0;

        QList<QObject*> scenes;

    signals:
        void scenesChanged();
        void audioDurationChanged();
        void markerPositionChanged();
        void loopAreaStartChanged();
        void loopAreaLengthChanged();

    public slots:
        void onMediaLoaded();
        void onPositionChanged();
        void onSceneShaderChanged();

    private:
        QMediaPlayer mediaPlayer;
        ShaderManager* shadermanager;

};

#endif // SCENEMANAGER_H
