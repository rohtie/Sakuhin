#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class BackEnd : public QObject {
    Q_OBJECT

    public:
        explicit BackEnd(QObject *parent = nullptr);

        QString getSessionID();


        Q_INVOKABLE void setSlider(const int &id, const float &value);
        float* getSliders();

        Q_INVOKABLE void createSession();

    private:
        float slider[4];
        QString sessionID;
};

#endif // BACKEND_H
