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

void ShaderManager::onSessionFileChange(const QString &path) {
    QFileInfo fileInfo(path);

    if (fileInfo.lastModified() > lastSessionModification) {
        lastSessionModification = fileInfo.lastModified();

        QFile session(path);
        session.open(QIODevice::ReadOnly);
        QByteArray newSessionContents = session.readAll();

        if (newSessionContents != sessionContents) {
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
