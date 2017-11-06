#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QQmlContext>
#include <QObject>

class ShaderManager : public QObject {
    Q_OBJECT

    public:
        ShaderManager(QQmlContext *qmlContext);

        void createShader();
        void updateContext();

    private:
        QQmlContext *qmlContext;

        QList<QObject*> shaders;
        QList<QObject*> transitionShaders;

};

#endif // SHADERMANAGER_H
