#ifndef MAPPINGWINDOW_H
#define MAPPINGWINDOW_H

#include <QVector>
#include <QMatrix4x4>
#include <QOpenGLTexture>

#include "window.h"

class MappingWindow : public Window {
    Q_OBJECT

    Q_PROPERTY(bool isProjectionMapping MEMBER isProjectionMapping NOTIFY isProjectionMappingChanged)
    Q_PROPERTY(bool isVertical MEMBER isVertical NOTIFY isVerticalChanged)
    Q_PROPERTY(float distanceFromObject MEMBER distanceFromObject NOTIFY distanceFromObjectChanged)
    Q_PROPERTY(float projectorHeight MEMBER projectorHeight NOTIFY projectorHeightChanged)
    Q_PROPERTY(float objectHeightTarget MEMBER objectHeightTarget NOTIFY objectHeightTargetChanged)
    Q_PROPERTY(float fieldOfView MEMBER fieldOfView NOTIFY fieldOfViewChanged)

    public:
        MappingWindow();

        void initializeGL();

        void renderScreen(Shader* shader);
        void setupMapping(const QString &configPath);
        void updateCalibrationPoints();
        void updateMesh();
        void drawBillboard(float x, float y);
        void saveProjectionMappingConfig();
        QVector2D* normalizeCoordinates(float x, float y);
        QVector2D* unNormalizeCoordinates(float x, float y);
        void applyVertexCalibration(QVector3D* vertex, float x, float y);

        void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void keyPressEvent(QKeyEvent* event);

    public slots:
        void updateProjectionMapping();
        void updateProjectionMappingSettings();
        void updateMVPmatrix();

    signals:
        void isProjectionMappingChanged();
        void isVerticalChanged();
        void distanceFromObjectChanged();
        void projectorHeightChanged();
        void objectHeightTargetChanged();
        void fieldOfViewChanged();

    protected:
        bool isProjectionMapping = false;
        bool isCalibrating = false;
        bool hasLoadedProjectionObject = false;

        // No calibration point selected if -1
        int selectedCalibrationPoint = -1;

        float distanceFromObject = -400.0;
        float projectorHeight = 125.;
        float objectHeightTarget = 125.;
        float fieldOfView = 45.2397;

        float cameraNear = 1.0;
        float cameraFar = 1000.0;

        bool isVertical = false;
        QString modelPath;
        QString configPath;

        QOpenGLVertexArrayObject calibrationVao;
        QOpenGLBuffer calibrationVertexBuffer;

        QOpenGLShaderProgram meshShader;
        QOpenGLVertexArrayObject meshVao;
        QOpenGLBuffer meshVertexBuffer;
        QOpenGLBuffer meshUVbuffer;

        QVector<QVector2D*> calibrationPoints;
        QVector<QVector3D*> originalVertices;
        QVector<QVector3D*> vertices;
        QVector<QVector2D*> UVs;
        QVector<QVector<int>> vertexFaces;
        QVector<QVector<int>> UVFaces;

        QVector<GLfloat> meshVertices;
        QVector<GLfloat> meshUVs;


        QMatrix4x4 modelMatrix;
        QMatrix4x4 viewMatrix;
        QMatrix4x4 projectionMatrix;
        QMatrix4x4 mvpMatrix;

        QVector3D cameraForward;
        QVector3D cameraRight;
        QVector3D cameraUp;

        QOpenGLTexture* calibrationTexture;
        QMatrix4x4 calibrationMatrix;


};

#endif // MAPPINGWINDOW_H
