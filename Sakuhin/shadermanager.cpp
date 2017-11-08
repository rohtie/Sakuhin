#include "shadermanager.h"
#include "shader.h"

ShaderManager::ShaderManager() {

}

void ShaderManager::initializeGL() {
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
