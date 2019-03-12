#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

class ShaderManager;
class SceneManager;
class WindowManager;

#include <QObject>

class SessionManager : public QObject {
    Q_OBJECT

    public:
        explicit SessionManager(QObject *parent = nullptr);
        void initialize(ShaderManager* shadermanager, SceneManager* scenemanager, WindowManager* windowmanager);

        Q_INVOKABLE void createSession();
        Q_INVOKABLE void loadSession(const QString path);
        Q_INVOKABLE void saveSession();

        ShaderManager* shadermanager;
        SceneManager* scenemanager;
        WindowManager* windowmanager;

        QString sessionID;
        QString sessionPath;

    signals:

    public slots:
};

#endif // SESSIONMANAGER_H
