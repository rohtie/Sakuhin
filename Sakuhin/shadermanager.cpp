#include "shadermanager.h"
#include "shader.h"

#include <QOffscreenSurface>

ShaderManager::ShaderManager() {

}

void ShaderManager::initialize(const QSurfaceFormat &format) {
    fileWatcher.addPath("sessions/" + sessionID + "/session.glsl");

    QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged,
                     this, &ShaderManager::onSessionFileChange);

    QOffscreenSurface surface;
    surface.setFormat(format);
    surface.create();

    context = QOpenGLContext::globalShareContext();
    context->setFormat(format);
    context->create();
    context->makeCurrent(&surface);

    createShader("data/shader_templates/minimal.glsl");
    mainShader = previewShader;

    emit shadersChanged();
}

Shader* ShaderManager::currentShader(bool isPreview) {
    if (isPreview) {
        return previewShader;
    }
    else {
        return mainShader;
    }
}

bool ShaderManager::previewIsMain() {
    return previewShader == mainShader;
}

void ShaderManager::createShader(QString templatePath) {
    // TODO: DRY this up together with createTransition
    QString creationTime = QString::number(QDateTime::currentSecsSinceEpoch());
    QString shaderPath = "sessions/" + sessionID + "/shaders/" + creationTime + ".glsl";

    QFile::copy(templatePath, shaderPath);
    shaders.append(new Shader("qrc:tmp/llsczl.jpg", shaderPath));
    selectShader(shaders.length() - 1);

    emit shadersChanged();
}

void ShaderManager::selectShader(int index) {
    // TODO: DRY this up together with selectTransition
    Shader* selectedShader = (Shader*) shaders.at(index);

    QString sessionFilepath = "sessions/" + sessionID + "/session.glsl";

    if (previewShader != nullptr) {
        if (QFile::exists(previewShader->filepath)) {
            QFile::remove(previewShader->filepath);
        }
        QFile::copy(sessionFilepath, previewShader->filepath);
    }

    if (QFile::exists(sessionFilepath)) {
        QFile::remove(sessionFilepath);
    }
    QFile::copy(selectedShader->filepath, sessionFilepath);

    previewShader = selectedShader;

    isPreviewingShader = true;
    emit isPreviewingShaderChanged();
}

void ShaderManager::createTransition(QString templatePath) {
    // TODO: DRY this up together with createShader
    QString creationTime = QString::number(QDateTime::currentSecsSinceEpoch());
    QString shaderPath = "sessions/" + sessionID + "/transitions/" + creationTime + ".glsl";

    QFile::copy(templatePath, shaderPath);
    transitionShaders.append(new Shader("qrc:tmp/llsczl.jpg", shaderPath));
    selectTransition(transitionShaders.length() - 1);

    emit transitionShadersChanged();
}

void ShaderManager::selectTransition(int index) {
    // TODO: DRY this up together with selectShader
    Shader* selectedShader = (Shader*) transitionShaders.at(index);

    QString sessionFilepath = "sessions/" + sessionID + "/session.glsl";

    if (previewShader != nullptr) {
        if (QFile::exists(previewShader->filepath)) {
            QFile::remove(previewShader->filepath);
        }
        QFile::copy(sessionFilepath, previewShader->filepath);
    }

    if (QFile::exists(sessionFilepath)) {
        QFile::remove(sessionFilepath);
    }
    QFile::copy(selectedShader->filepath, sessionFilepath);

    previewShader = selectedShader;

    isPreviewingShader = false;
    emit isPreviewingShaderChanged();
}

void ShaderManager::onSessionFileChange(const QString &path) {
    QFileInfo fileInfo(path);

    if (fileInfo.lastModified() > lastSessionModification) {
        lastSessionModification = fileInfo.lastModified();

        QFile session(path);
        session.open(QIODevice::ReadOnly);
        QByteArray newSessionContents = session.readAll();
        session.close();

        if (newSessionContents != sessionContents && newSessionContents.size() > 0) {
            sessionContents = newSessionContents;

            previewShader->recompile(sessionContents);
        }
    }

    // On some systems, files are replaced by an entirely new file when
    // modified. thus we need to add it to the file watcher again.
    if (!fileWatcher.files().contains(path) && fileInfo.exists()) {
        fileWatcher.addPath(path);
    }
}
