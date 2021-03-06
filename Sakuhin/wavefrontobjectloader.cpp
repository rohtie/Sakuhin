#include "wavefrontobjectloader.h"

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QFile>
#include <QDebug>


WavefrontObjectLoader::WavefrontObjectLoader() {

}


// Expand abstract model to opengl triangles
void WavefrontObjectLoader::expandModel(
    QVector<GLfloat> &openglVertices,
    QVector<GLfloat> &openglUVs,
    QVector<QVector3D*> &vertices,
    QVector<QVector2D*> &UVs,
    QVector<QVector<int>> &vertexFaces,
    QVector<QVector<int>> &UVFaces
) {
    for (int i = 0; i < vertexFaces.length(); i++) {
        for (int j = 0; j < 3; j++) {
            QVector3D* currentVertex = vertices[vertexFaces[i][j]];
            QVector2D* currentUV = UVs[UVFaces[i][j]];

            openglVertices.append(currentVertex->x());
            openglVertices.append(currentVertex->y());
            openglVertices.append(currentVertex->z());

            openglUVs.append(currentUV->x());
            openglUVs.append(currentUV->y());
        }
    }
}


bool WavefrontObjectLoader::loadMesh(
    const QString &filePath,
    QVector<GLfloat> &openglVertices,
    QVector<GLfloat> &openglUVs,
    QVector<QVector3D*> &vertices,
    QVector<QVector2D*> &UVs,
    QVector<QVector<int>> &vertexFaces,
    QVector<QVector<int>> &UVFaces
) {
    QFile meshFile(filePath);

    if (!meshFile.open(QIODevice::ReadOnly)) {
        qDebug() << filePath << " could not be loaded";
        return false;
    }

    // Assume that the obj file only has vertices and texture coordinates
    while (!meshFile.atEnd()) {
        QTextStream stream(meshFile.readLine());

        char command = 0;
        stream >> command;

        if (command == 'v') {
            stream >> command;

            if (command == 't') {
                // UV
                double vt1 = 0.0, vt2 = 0.0;
                stream >> vt1 >> vt2;

                UVs.append(new QVector2D(vt1, vt2));
            }
            else if (command == 'n') {
                // TODO: support normals
                // double vn1 = 0.0, vn2 = 0.0, vn3 = 0.0;
                // stream >> vn1 >> vn2 >> vn3;

                // normals.append(new QVector3D(vn1, vn2, vn3));
            }
            else {
                // Verticies
                double v1 = 0.0, v2 = 0.0, v3 = 0.0;
                stream >> v1 >> v2 >> v3;

                vertices.append(new QVector3D(v1, v2, v3));
            }
        }
        else if (command == 'f') {
            char slash = 0;
            int v1 = 0, v2 = 0, v3 = 0;
            int t1 = 0, t2 = 0, t3 = 0;

            stream >> v1 >> slash >> t1 >> v2 >> slash >> t2 >> v3 >> slash >> t3;

            // .obj indexing starts at 1 instead of 0
            // so we must decrease the index we get by 1
            // to get the correct index for our indices
            QVector<int> vertexFace = {v1 - 1, v2 - 1, v3 - 1};
            QVector<int> textureFace = {t1 - 1, t2 - 1, t3 - 1};

            vertexFaces.append(vertexFace);
            UVFaces.append(textureFace);
        }
    }

    expandModel(openglVertices, openglUVs, vertices, UVs, vertexFaces, UVFaces);

    meshFile.close();

    return true;
}

bool WavefrontObjectLoader::loadMesh(
    const QString &filePath,
    QVector<GLfloat> &openglVertices,
    QVector<GLfloat> &openglUVs
) {

    QVector<QVector3D*> vertices;
    QVector<QVector2D*> UVs;
    QVector<QVector<int>> vertexFaces;
    QVector<QVector<int>> UVFaces;

    return loadMesh(filePath, openglVertices, openglUVs, vertices, UVs, vertexFaces, UVFaces);
}
