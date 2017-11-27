#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>

bool loadMesh(const QString &filePath, QVector<GLfloat> &openglVertices, QVector<GLfloat> &openglUVs) {
    QFile meshFile(filePath);
    meshFile.open(QIODevice::ReadOnly);

    QVector<QVector3D*> vertices;
    QVector<QVector2D*> UVs;
    QVector<QVector<int>> vertexFaces;
    QVector<QVector<int>> UVFaces;

    // Assume that the obj file only has vertices and texture coordinates
    while (!meshFile.atEnd()) {
        QTextStream stream(meshFile.readLine());

        char command = 0;
        stream >> command;

        if (command == 'v') {
            stream >> command;

            if (command == 't') {
                double t1 = 0.0, t2 = 0.0;
                stream >> t1 >> t2;

                UVs.append(new QVector2D(t1, t2));
            }
            else {
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

    meshFile.close();
}

#endif // OBJLOADER_H
