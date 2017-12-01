#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QString>
#include <QStringList>

#include "shadermanager.h"

class BackEnd : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString performanceInformation MEMBER performanceInformation NOTIFY performanceInformationChanged)
    Q_PROPERTY(QStringList easingNames MEMBER easingNames NOTIFY easingNamesChanged)

    public:
        explicit BackEnd(QObject* parent = nullptr);

        QString getSessionID();

        Q_INVOKABLE void createSession();
        void setPerformanceInformation(const QString &performanceInformation);

        ShaderManager* shadermanager;

    signals:
        void performanceInformationChanged();
        void easingNamesChanged();

    private:
        QString sessionID;
        QString sessionPath;

        QString performanceInformation = "16.66667 ms 60 fps";
        QStringList easingNames;
};

#endif // BACKEND_H
