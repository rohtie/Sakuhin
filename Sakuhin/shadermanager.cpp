#include "shadermanager.h"
#include "shader.h"

ShaderManager::ShaderManager() {
    shaders.append(new Shader(1242424, "qrc:tmp/XdjyR1.jpg"));
    shaders.append(new Shader(12442, "qrc:tmp/XdjyR1.jpg"));
    shaders.append(new Shader(125535424, "qrc:tmp/XtsyWN.jpg"));
    shaders.append(new Shader(12442224, "qrc:tmp/XdjyR1.jpg"));
    shaders.append(new Shader(12442, "qrc:tmp/XdjyR1.jpg"));
    shaders.append(new Shader(125535424, "qrc:tmp/XtsyWN.jpg"));
    shaders.append(new Shader(125535424, "qrc:tmp/XtsyWN.jpg"));
    shaders.append(new Shader(12442224, "qrc:tmp/XdjyR1.jpg"));

    transitionShaders.append(new Shader(1, "qrc:tmp/XtsyWN.jpg"));
    transitionShaders.append(new Shader(2, "qrc:tmp/XdjyR1.jpg"));
    transitionShaders.append(new Shader(3, "qrc:tmp/llsczl.jpg"));
    transitionShaders.append(new Shader(2, "qrc:tmp/XdjyR1.jpg"));
    transitionShaders.append(new Shader(3, "qrc:tmp/llsczl.jpg"));
    transitionShaders.append(new Shader(2, "qrc:tmp/XdjyR1.jpg"));
    transitionShaders.append(new Shader(3, "qrc:tmp/llsczl.jpg"));
    transitionShaders.append(new Shader(2, "qrc:tmp/XdjyR1.jpg"));
    transitionShaders.append(new Shader(3, "qrc:tmp/llsczl.jpg"));
}

void ShaderManager::createShader(QString templateUrl) {
    shaders.append(new Shader(12442224, "qrc:tmp/llsczl.jpg"));
    emit shadersChanged();
}
