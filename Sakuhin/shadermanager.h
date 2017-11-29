#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QQmlContext>
#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QFileSystemWatcher>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include "shader.h"

class ShaderManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> shaders MEMBER shaders NOTIFY shadersChanged)
    Q_PROPERTY(QList<QObject*> transitionShaders MEMBER transitionShaders NOTIFY transitionShadersChanged)

    public:
        ShaderManager();

        void initialize(const QSurfaceFormat &format);

        Shader* currentShader(bool isPreview);
        bool previewIsMain();

        Q_INVOKABLE void createShader(QString templatePath);
        Q_INVOKABLE void selectShader(int index);
        void makeCurrent();

        Q_INVOKABLE void createTransition(QString templatePath);
        Q_INVOKABLE void selectTransition(int index);
        void startTransition();

        QString sessionID;

    public slots:
        void onSessionFileChange(const QString &path);

    signals:
        void shadersChanged();
        void transitionShadersChanged();

    private:
        Shader* mainShader = nullptr;
        Shader* previewShader = nullptr;

        QFileSystemWatcher fileWatcher;
        QByteArray sessionContents;
        QDateTime lastSessionModification;

        QList<QObject*> shaders;
        QList<QObject*> transitionShaders;

        QOpenGLContext* context;
};

#endif // SHADERMANAGER_H
