#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QQmlContext>
#include <QObject>

#include "shader.h"

class ShaderManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> shaders MEMBER shaders NOTIFY shadersChanged)
    Q_PROPERTY(QList<QObject*> transitionShaders MEMBER transitionShaders NOTIFY transitionShadersChanged)

    public:
        ShaderManager();

        void initializeGL();

        Q_INVOKABLE void createShader(QString templateUrl);
        void selectShader();
        void makeCurrent();

        Q_INVOKABLE void createTransition(QString templateUrl);
        void selectTransition();
        void startTransition();

        QString sessionID;

        Shader* mainShader;
        Shader* previewShader;

    signals:
        void shadersChanged();
        void transitionShadersChanged();

    private:
        QList<QObject*> shaders;
        QList<QObject*> transitionShaders;

};

#endif // SHADERMANAGER_H
