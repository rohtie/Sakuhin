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

        Q_INVOKABLE void createShader(QString templateUrl);
        void selectShader();
        void makeCurrent();

        Q_INVOKABLE void createTransition(QString templateUrl);
        void selectTransition();
        void startTransition();

    signals:
        void shadersChanged();
        void transitionShadersChanged();

    private:
        Shader *mainShader;
        Shader *previewShader;

        QList<QObject*> shaders;
        QList<QObject*> transitionShaders;

};

#endif // SHADERMANAGER_H
