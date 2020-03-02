#ifndef WAVEFRONTOBJECTLOADER_H
#define WAVEFRONTOBJECTLOADER_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLFunctions>

class WavefrontObjectLoader {
    public:
        WavefrontObjectLoader();

        static void expandModel(
            QVector<GLfloat> &openglVertices,
            QVector<GLfloat> &openglUVs,
            QVector<QVector3D*> &vertices,
            QVector<QVector2D*> &UVs,
            QVector<QVector<int>> &vertexFaces,
            QVector<QVector<int>> &UVFaces
        );

        static bool loadMesh(
            const QString &filePath,
            QVector<GLfloat> &openglVertices,
            QVector<GLfloat> &openglUVs,
            QVector<QVector3D*> &vertices,
            QVector<QVector2D*> &UVs,
            QVector<QVector<int>> &vertexFaces,
            QVector<QVector<int>> &UVFaces
        );

        static bool loadMesh(
            const QString &filePath,
            QVector<GLfloat> &openglVertices,
            QVector<GLfloat> &openglUVs
        );
};

#endif // WAVEFRONTOBJECTLOADER_H
