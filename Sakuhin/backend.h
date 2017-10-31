#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QString>

class BackEnd : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QString performanceInformation
        READ performanceInformation
        WRITE setPerformanceInformation
        NOTIFY performanceInformationChanged
    )

    public:
        explicit BackEnd(QObject *parent = nullptr);

        enum ChannelType {
            Texture,
            Shader,
            Hardware
        };
        Q_ENUM(ChannelType)

        QString getSessionID();

        QString performanceInformation();
        void setPerformanceInformation(const QString &performanceInformation);


        Q_INVOKABLE void setSlider(const int &id, const float &value);
        float* getSliders();

        Q_INVOKABLE void createSession();
        Q_INVOKABLE void setChannel(const int &channelID, ChannelType channelType, const QString &fileUrl);

    public slots:
        void onShaderRecompile();

    signals:
        void performanceInformationChanged();
        void channelChanged(const int &channelID, ChannelType &channelType, const QString &fileUrl);

    private:
        float slider[4] = {};
        QString sessionID;
        QString sessionPath;

        QString m_performanceInformation = "16.66667 ms 60 fps";

        QJsonObject controllerLog;
        QFile controllerLogFile;
};

#endif // BACKEND_H
