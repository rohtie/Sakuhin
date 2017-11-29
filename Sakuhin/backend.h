#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QString>

#include "shadermanager.h"

class BackEnd : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString performanceInformation MEMBER performanceInformation NOTIFY performanceInformationChanged)

    public:
        explicit BackEnd(QObject* parent = nullptr);

        QString getSessionID();

        Q_INVOKABLE void createSession();
        void setPerformanceInformation(const QString &performanceInformation);

        ShaderManager* shadermanager;

    signals:
        void performanceInformationChanged();

    private:
        QString sessionID;
        QString sessionPath;

        QString performanceInformation = "16.66667 ms 60 fps";
};

#endif // BACKEND_H
