#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class Backend : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString performanceInformation MEMBER performanceInformation NOTIFY performanceInformationChanged)
    Q_PROPERTY(QStringList easingNames MEMBER easingNames NOTIFY easingNamesChanged)

    public:
        explicit Backend(QObject* parent = nullptr);
        void setPerformanceInformation(const QString &performanceInformation);
        void initialize();

    signals:
        void performanceInformationChanged();
        void easingNamesChanged();

    private:
        QString performanceInformation = "16.66667 ms 60 fps";
        QStringList easingNames;
};

#endif // BACKEND_H
