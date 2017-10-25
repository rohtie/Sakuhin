#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class BackEnd : public QObject {
    Q_OBJECT

    public:
        explicit BackEnd(QObject *parent = nullptr);

        QString getSessionID();


        Q_INVOKABLE void setSlider(const int &id, const float &value);
        float* getSliders();

        Q_INVOKABLE void createSession();

    public slots:
        void onShaderRecompile();

    private:
        float slider[4] = {};
        QString sessionID;
        QString sessionPath;

        QJsonObject controllerLog;
        QFile controllerLogFile;
};

#endif // BACKEND_H
