#include <QQmlApplicationEngine>

#ifndef LIVEQMLAPPLICATIONENGINE_H
#define LIVEQMLAPPLICATIONENGINE_H


class EnhancedQmlApplicationEngine : public QQmlApplicationEngine {
    Q_OBJECT

    public:
        explicit EnhancedQmlApplicationEngine(QObject *parent = nullptr);

    Q_INVOKABLE void clearCache();
};

#endif // LIVEQMLAPPLICATIONENGINE_H
