#ifndef SHADER_H
#define SHADER_H

#include <QObject>

class Shader : public QObject {
    Q_OBJECT

    Q_PROPERTY(int id MEMBER id NOTIFY idChanged)
    Q_PROPERTY(QString thumbnail MEMBER thumbnail NOTIFY thumbnailChanged)

    public:
        Shader(int id, QString thumbnail);

        QString thumbnail;

    signals:
        void idChanged();
        void thumbnailChanged();

    private:
        int id;
};

#endif // SHADER_H
