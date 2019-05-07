#include "videomanager.h"
#include "videogroup.h"

#include <QDebug>
#include <QFileInfo>

VideoManager::VideoManager(const QString &fileUrl) {
    QFileInfo file(fileUrl);

    if (file.isDir()) {
        rootGroup = new VideoGroup(this, nullptr, VideoGroup::SequenceMode, fileUrl);
    }
    else {
        rootGroup = new VideoGroup(this, fileUrl);
    }

    currentGroup = rootGroup;
}

void VideoManager::next(bool isInterrupting) {
    VideoGroup* newGroup = currentGroup->next(isInterrupting);

    if (newGroup != nullptr) {
        currentGroup = newGroup;
    }
    else {
        isPlaying = false;
    }
}

void VideoManager::previous() {
    VideoGroup* newGroup = currentGroup->previous();

    if (newGroup != nullptr) {
        currentGroup = newGroup;
    }
    else {
        qDebug() << "Previous has become too previous...";
    }
}

void VideoManager::interruptNext() {
    next(true);
}

QOpenGLTexture* VideoManager::currentFrame() {
    return currentGroup->currentFrame();
}

void VideoManager::destroy() {
    qDebug() << "TODO: Destroy all groups";
}
