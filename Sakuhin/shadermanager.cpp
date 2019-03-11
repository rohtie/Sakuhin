#include "shadermanager.h"
#include "shader.h"
#include "channel.h"

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
    QString creationTime = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString shaderPath = "sessions/" + sessionID + "/shaders/" + creationTime + ".glsl";

    QFile::copy(templatePath, shaderPath);
    shaders.append(new Shader(shaderPath));
    selectShader(shaders.length() - 1);

    emit shadersChanged();
}

void ShaderManager::selectShader(int index) {
    // TODO: DRY this up together with selectTransition
    Shader* selectedShader = (Shader*) shaders.at(index);

    if (previewShader != selectedShader) {
        if (previewShader != nullptr) {
            QString creationTime = QString::number(QDateTime::currentMSecsSinceEpoch());
            previewShader->createThumbnail("sessions/" + sessionID + "/shaders/thumbnails/" + creationTime + ".jpg");
        }

        QString sessionFilepath = "sessions/" + sessionID + "/session.glsl";

        // Replacing the session file with the selected shader file causes
        // a recompile of the shader. To prevent this, we must temporarly
        // remove the file from the filewatcher
        fileWatcher.removePath(sessionFilepath);

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

        fileWatcher.addPath(sessionFilepath);

        previewShader = selectedShader;
    }

    isPreviewingShader = true;
    emit isPreviewingShaderChanged();
}

void ShaderManager::makeCurrent(int index) {
    if (isPreviewingShader) {
        mainIndex = index;
        mainShader = previewShader;
        emit mainIndexChanged();
    }
}

void ShaderManager::createTransition(QString templatePath) {
    // TODO: DRY this up together with createShader
    QString creationTime = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString shaderPath = "sessions/" + sessionID + "/transitions/" + creationTime + ".glsl";

    QFile::copy(templatePath, shaderPath);
    transitionShaders.append(new Shader(shaderPath));
    selectTransition(transitionShaders.length() - 1);

    emit transitionShadersChanged();
}

void ShaderManager::selectTransition(int index) {
    // TODO: DRY this up together with selectShader
    Shader* selectedShader = (Shader*) transitionShaders.at(index);

    if (previewShader != selectedShader) {
        if (previewShader != nullptr) {
            QString creationTime = QString::number(QDateTime::currentMSecsSinceEpoch());
            previewShader->createThumbnail("sessions/" + sessionID + "/transitions/thumbnails/" + creationTime + ".jpg");
        }

        ((Channel*) selectedShader->channels[0])->setShader(mainShader);
        ((Channel*) selectedShader->channels[1])->setShader(previewShader);

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
    }

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
