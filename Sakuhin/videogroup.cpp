#include "videogroup.h"
#include "videoplayer.h"
#include "videomanager.h"

#include <algorithm>
#include <QRegExp>
#include <QDir>
#include <QDebug>


VideoGroup::VideoGroup (VideoManager* videomanager, VideoGroup* parent, VideoGroupMode mode, const QString path) {
    this->videomanager = videomanager;
    this->parent = parent;
    this->mode = mode;

    // Grab children from path
    QDir directory(path);
    QFileInfoList files = directory.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

    for (int i=0; i<files.count(); i++) {
        QFileInfo file = files[i];

        if (file.isDir()) {
            QRegExp modeExtractor("(\\d+)_([A-Z_]+)");
            modeExtractor.indexIn(file.fileName());

            VideoGroupMode childMode = NoMode;

            QString modeID = modeExtractor.cap(2);

            if (modeID == "SEQUENCE") {
                childMode = SequenceMode;
            }
            else if (modeID == "SEQUENCE_LOOP") {
                childMode = SequenceLoopMode;
            }
            else if (modeID == "RANDOM") {
                childMode = RandomMode;
            }
            else if (modeID == "RANDOM_LOOP") {
                childMode = RandomLoopMode;
            }

            children.append(new VideoGroup(videomanager, this, childMode, file.filePath()));
        }
        else {
            // Assume video file
            children.append(new VideoPlayer(this, file.filePath()));
        }
    }

    if (mode == RandomMode || mode == RandomLoopMode) {
        std::random_shuffle(children.begin(), children.end());
    }
}

// Constructor meant for single videos not contained in a folder
VideoGroup::VideoGroup(VideoManager* videomanager, const QString path) {
    this->videomanager = videomanager;

    // Assume video
    children.append(new VideoPlayer(this, path));
}

VideoGroup* VideoGroup::next(bool isInterrupt) {

    if (currentVideo != nullptr) {
        currentVideo->stop();
        currentVideo->destroy();
        currentVideo = nullptr;
    }

    index++;

    if (index >= children.count()) {
        switch (mode) {
            case NoMode:
            case SequenceMode:
            case RandomMode:
                // Last child has been played.
                // So we must go up one level
                if (parent != nullptr) {
                    return parent->next(false);
                }
                return nullptr;
                break;

            case RandomLoopMode:
                std::random_shuffle(children.begin(), children.end());

            case SequenceLoopMode:
                index = 0;

                if (isInterrupt) {
                    // Next group is requested, but we are in an infinite loop
                    // So we need to step up one level
                    if (parent != nullptr) {
                        return parent->next(false);
                    }
                    return nullptr;
                }
                break;
        }
    }


    VideoGroup* videogroup = qobject_cast<VideoGroup*>(children.at(index));

    if (videogroup != nullptr) {
        return videogroup->next(false);
    }
    else {
        // Assume video
        VideoPlayer* videoplayer = (VideoPlayer*) children.at(index);
        videoplayer->create();
        videoplayer->start();

        currentVideo = videoplayer;
    }

    // Continue using this group for playback
    return this;
}

VideoGroup* VideoGroup::previous() {
    index--;

    if (currentVideo != nullptr) {
        currentVideo->stop();
        currentVideo->destroy();
        currentVideo = nullptr;
    }

    if (index == -1) {
        // There are no videos / groups to be used
        // So we must go up one level
        return parent;
    }


    VideoGroup* videogroup = qobject_cast<VideoGroup*>(children.at(index));

    if (videogroup != NULL) {
        return videogroup->previous();
    }
    else {
        // Assume video
        VideoPlayer* videoplayer = (VideoPlayer*) children.at(index);
        videoplayer->create();
        videoplayer->start();

        currentVideo = videoplayer;
    }

    // Continue using this group for playback
    return this;
}

void VideoGroup::childHasFinishedPlaying() {
    // The video has finished playing.
    // Tell videomanager to grab the next video or group
    videomanager->next(false);
}

QOpenGLTexture* VideoGroup::currentFrame() {
    return currentVideo->currentFrame();
}
