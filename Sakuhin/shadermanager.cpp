#include "shadermanager.h"
#include "shader.h"

#include <QOffscreenSurface>

ShaderManager::ShaderManager() {
}

void ShaderManager::createContext(const QSurfaceFormat &format) {
    QOffscreenSurface surface;
    surface.setFormat(format);
    surface.create();

    context = QOpenGLContext::globalShareContext();
    context->setFormat(format);
    context->create();
    context->makeCurrent(&surface);

    mainShader = new Shader(
        1242424,
        "qrc:tmp/XdjyR1.jpg",
        "sessions/" + sessionID + "/session.glsl"
    );
    previewShader = mainShader;
}

void ShaderManager::createShader(QString templateUrl) {
    emit shadersChanged();
}

void ShaderManager::createTransition(QString templateUrl) {
    emit transitionShadersChanged();
}
