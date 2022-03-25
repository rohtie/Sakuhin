#include "timelinewindow.h"

#include "scenemanager.h"

TimelineWindow::TimelineWindow() {

}

void TimelineWindow::processTime() {
    currentTime = scenemanager->markerPosition;
    performanceTime = time.elapsed();
}

void TimelineWindow::setupRecordingTime() {
    scenemanager->skipTo(0.0f);
    recordingFrame = 0.0;
    recordingStartTime = 0.0f;
}

void TimelineWindow::handleRecordingTime() {
    recordingFrame += 1;

    float msPerFrame = (1.0f / recordingFramerate) * 1000.0f;

    scenemanager->skipTo(recordingFrame * msPerFrame);

    // Record until end of demo
    if (recordingFrame * msPerFrame > scenemanager->audioDuration) {
        isRecording = false;
        // videoRecorder.close();
    }
}
