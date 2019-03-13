#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

class Backend;
class AudioManager;
class ShaderManager;
class SceneManager;
class WindowManager;

#include <QObject>
#include <QSurfaceFormat>

class SessionManager : public QObject {
    Q_OBJECT

    public:
        explicit SessionManager(QObject *parent = nullptr);
        void initialize(const QSurfaceFormat &format, QObject* qmlRoot);

        Q_INVOKABLE void createSession();
        Q_INVOKABLE void loadSession(const QString path);
        Q_INVOKABLE void saveSession();

        Backend* backend;
        AudioManager* audiomanager;
        ShaderManager* shadermanager;
        SceneManager* scenemanager;
        WindowManager* windowmanager;

        QString sessionID;
        QString sessionPath;
        QSurfaceFormat format;

    signals:

    public slots:
};

#endif // SESSIONMANAGER_H
